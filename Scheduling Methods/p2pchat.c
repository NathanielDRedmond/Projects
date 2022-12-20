#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"
#include "ui.h"


#define MAX_CONNECTIONS 100
#define MAX_MESSAGE 1000
#define USER_MAX 20
#define BACKLOG 1

// Keep the username in a global so we can access it from the callback
const char* username;
int num_connections = 0;
int connections_arr[MAX_CONNECTIONS] = {-1};
//lock to protect shared arr
pthread_mutex_t arr_lock;

// Helper function that writes messages to the fds of all peers connected to sender
// Note that we do not write to sender, would result in infinite loop
void send_message(const char* username, const char* message, int sender) {

	// Contains the max length of an entire message (user + msg)
	// + 2 to account for null characters
	int max_length = USER_MAX + MAX_MESSAGE + 2;
	char new_message[max_length];
	
	// Set all characters of the new message to null value
	memset(new_message, '\0', sizeof(new_message));
	
	// Copy username and message into one message so that it can be written to fds using one call to write
	// Not necessary, but cleaner
	strncpy(new_message, username, USER_MAX + 1);
	strncpy(new_message + USER_MAX + 1, message, MAX_MESSAGE + 1);
	
	// Iterate through this server's peers. We write to every single connection except the server itself
	pthread_mutex_lock(&arr_lock);
	for (int i = 0; i < num_connections; ++i) {
		if (connections_arr[i] == -1) break;
		if (connections_arr[i] == sender) continue;
		write(connections_arr[i], new_message, max_length);
	}
	pthread_mutex_unlock(&arr_lock);
}

// Helper function that takes a message that was read from a file descriptor.
// The message is dissected into two separate strings (username and message)
// that way UI_display can be called
void display_read(const char* read_message, int sender) {

	// Set up dissection of message
	char new_user[USER_MAX + 1];
	char new_message[MAX_MESSAGE + 1];
	
	// Set all chars to null (sets up strncpy)
	memset(new_user, '\0', USER_MAX + 1);
	memset(new_message, '\0', MAX_MESSAGE + 1);
	// Copy all 20 bytes of username portion of the obtained message (+ null character)
	strncpy(new_user, read_message, USER_MAX + 1);
	// Copy all 1000 bytes of message portion of the obtained message (+ null character)
	strncpy(new_message, read_message + USER_MAX + 1, MAX_MESSAGE + 1);
	
	// Need these to satisfy types of parameters in ui_display
	const char* display_user = new_user;
	const char* display_message = new_message;
	
	// Display the message
	ui_display(display_user, display_message);
	
	// Send this message to all peers connected to this server
	send_message(display_user, display_message, sender);
}

// Thread function that is used to check if a server is written to
void* receiver_thread(void* peer) {
	// Total length of a message (username + message portion + two null chars)
	int max_length = USER_MAX + MAX_MESSAGE + 2;
	// Type conversion
	int peer_fd = * (int*) peer;
	while(1) {
		// Set up string for read
		char read_message[max_length];
		int bytes_read;
		// If there is data to be read from this file descriptor, we will dissect it and
		// display it using display_read helper function. display_read will then call
		// send_message to write to all peers connected to this server
		if ((bytes_read = read(peer_fd, read_message, max_length)) != 0) {
			const char* display_read_message = read_message;
			display_read(display_read_message, peer_fd);
		}
	}
}

// Thread function that makes a server a listening server. 
// If a client attempts to connect, we accept that connection, and then
// call receiver_thread on that client so we can check to see if it 
// receives any messages
void* accept_thread(void* fd) {
	// Type conversion
	int server_fd = *(int*) fd;
	
	// Make listening server
	if (listen(server_fd, 1)) {
		perror("listen failed");
		exit(1);
	}
	
	// Constantly check to see if server has an incoming client connection
	while (1) {
		int peer_fd;
		// accept connection if available
		if ((peer_fd = server_socket_accept(server_fd)) == -1) {
			perror("accepting incoming connection failed");
			exit(1);
		}
		else if (num_connections >= MAX_CONNECTIONS) {
			perror("maximum number of connections already reached");
			exit(1);
		}
		//lock to protect shared arr
		pthread_mutex_lock(&arr_lock);
		// add peer to our connection array
		connections_arr[num_connections++] = peer_fd;
		pthread_mutex_unlock(&arr_lock);
		pthread_t peer_thread;
		
		// Create a thread for the newly connected peer that checks for incoming messages
		if (pthread_create(&peer_thread, NULL, receiver_thread, &peer_fd) != 0) {
			perror("error creating peer specific thread");
			exit(1);
		}
	}

}

// This function is run whenever the user hits enter after typing a message
void input_callback(const char* message) {
  if(strcmp(message, ":quit") == 0 || strcmp(message, ":q") == 0) {
    ui_exit();
  } else {
  	// display the entered message
    ui_display(username, message);
    // Then send this message to all peers
    send_message(username, message, -1);
  }
}

	

int main(int argc, char** argv) {
  // Make sure the arguments include a username
  if(argc != 2 && argc != 4) {
    fprintf(stderr, "Usage: %s <username> [<peer> <port number>]\n", argv[0]);
    exit(1);
  }
  
  // Save the username in a global
  username = argv[1];
  
  // TODO: Set up a server socket to accept incoming connections
  int sfd;
  unsigned short temp = 0;
  if ((sfd = server_socket_open(&temp)) == -1) {
  	perror("server creation failed");
  	exit(1);
  }
  
  
  // Did the user specify a peer we should connect to?
  if(argc == 4) {
    // Unpack arguments
    char* peer_hostname = argv[2];
    unsigned short peer_port = atoi(argv[3]);
    
    // TODO: Connect to another peer in the chat network
    int peer_sfd;
    if ((peer_sfd = socket_connect(peer_hostname, peer_port)) == -1) {
    	perror("connection to peer failed");
    	exit(1);
    }
    
    // use lock to prevent race conditions
    pthread_mutex_lock(&arr_lock);
    connections_arr[num_connections] = peer_sfd;
    num_connections++;
    pthread_mutex_unlock(&arr_lock);
    
    pthread_t receiver;
    // calls receiver thread function to check for incoming messages
    if (pthread_create(&receiver, NULL, receiver_thread, &peer_sfd) != 0) {
    	perror("error creating thread");
    	exit(1);
    }
  }
  
  // call thread function that checks for incoming connections (must be done for every server)
  pthread_t accept_checker;
  if (pthread_create(&accept_checker, NULL, accept_thread, &sfd) != 0) {
  	perror("creating thread1 failed");
  	exit(1);
  }

  
  
  
  // Set up the user interface. The input_callback function will be called
  // each time the user hits enter to send a message.
  ui_init(input_callback);
  
  // Once the UI is running, you can use it to display log messages
  char* port_str = malloc(sizeof(int) * 10 + 1);
  sprintf(port_str, "%d", temp);
  ui_display("server listening on", port_str);
  
  // Run the UI loop. This function only returns once we call ui_stop() somewhere in the program.
  ui_run();
  
  return 0;
}
