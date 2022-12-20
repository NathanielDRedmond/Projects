#include "bitmap.h"
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>


int iteration_to_color(int i, int max);
int iterations_at_point(double x, double y, int max);
int num_threads;
double y_min;
double y_max;

struct thread_args {
	struct bitmap *b;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int prev_height;
	int new_height;
	int true_height;
	int width;
	double itermax;
};


void* compute_image(struct thread_args* args);
void show_help()
{
    printf("Use: mandel [options]\n");
    printf("Where options are:\n");
    printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
    printf("-x <coord>  X coordinate of image center point. (default=0)\n");
    printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
    printf("-n <threads> The number of threads used in computing image. (default=1)\n");
    printf("-s <scale>  Scale of the image in Mandlebrot coordinates. (default=4)\n");
    printf("-W <pixels> Width of the image in pixels. (default=500)\n");
    printf("-H <pixels> Height of the image in pixels. (default=500)\n");
    printf("-o <file>   Set output file. (default=mandel.bmp)\n");
    printf("-h          Show this help text.\n");
    printf("\nSome examples are:\n");
    printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
    printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
    printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}

int main(int argc, char* argv[])
{
    char c;

    // These are the default configuration values used
    // if no command line arguments are given.

    const char* outfile = "mandel.bmp";
    double xcenter = 0;
    double ycenter = 0;
    double scale = 4;
    int image_width = 500;
    int image_height = 500;
    int max = 1000;
    num_threads = 1;

    // For each command line argument given,
    // override the appropriate configuration value.

    while ((c = getopt(argc, argv, "x:y:n:s:W:H:m:o:h")) != -1) {
        switch (c) {
        case 'x':
            xcenter = atof(optarg);
            break;
        case 'y':
            ycenter = atof(optarg);
            break;
        case 'n':
        	num_threads = atoi(optarg);
        	break;
        case 's':
            scale = atof(optarg);
            break;
        case 'W':
            image_width = atoi(optarg);
            break;
        case 'H':
            image_height = atoi(optarg);
            break;
        case 'm':
            max = atoi(optarg);
            break;
        case 'o':
            outfile = optarg;
            break;
        case 'h':
            show_help();
            exit(1);
            break;
        }
    }

    // Display the configuration of the image.
    printf("mandel: x=%lf y=%lf scale=%lf max=%d outfile=%s\n", xcenter, ycenter, scale, max, outfile);

    // Create a bitmap of the appropriate size.
    struct bitmap* bm = bitmap_create(image_width, image_height);

    // Fill it with a dark blue, for debugging
    bitmap_reset(bm, MAKE_RGBA(0, 0, 255, 0));
   
    // Get runtime for all threads
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    
    // Set up for threading
    pthread_t thread_addrs [num_threads];
    int prev_height = 0;
    int new_height = (image_height / num_threads) + (image_height % num_threads);    	
    
    
    
    // Creates num_threads number of threads and computes image
    for (int i=0; i < num_threads; ++i) {
    
    	// Allocate space for instance of thread_args, which contains
    	// necessary information for compute_image (for each thread)
		struct thread_args *new_thread;
    	new_thread = (struct thread_args*)malloc(sizeof *new_thread);
    	new_thread->b = bm;
    	new_thread->true_height = image_height;
    	new_thread->width = image_width;
    	new_thread->xmin = xcenter - scale;
    	new_thread->xmax = xcenter + scale;
    	new_thread->ymin = ycenter - scale;
    	new_thread->ymax = ycenter + scale;
    	new_thread->itermax = max;
    	new_thread->prev_height = prev_height;
    	new_thread->new_height = new_height;
   
    	
    	// Create thread - upon failure generates an error message
    	// Stores thread information in thread_addrs and passes new_thread
    	// to compute_image via a function pointer
    	if(pthread_create(&(thread_addrs[i]), NULL,(void*) &compute_image, (void*) new_thread)) perror("Error creating thread\n");
    	
    	// Sets up height for next thread
    	prev_height = new_height;
    	new_height = new_height + (image_height / num_threads);
    }
    
    // Halts calling process until each thread has exited
    for (int i=0; i<num_threads; i++) {
    	pthread_join(thread_addrs[i], NULL);
    }
	
	
	// Does further calculations to determine runtime of all threads
   	gettimeofday(&end, NULL);
   	long seconds = (end.tv_sec - start.tv_sec);
   	long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec); 	
    printf("Time taken for %d thread(s): %ld seconds and %ld micros\n", num_threads, seconds, micros);

    // Save the image in the stated file.
    if (!bitmap_save(bm, outfile)) {
        fprintf(stderr, "mandel: couldn't write to %s: %s\n", outfile, strerror(errno));
        return 1;
    }

    return 0;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

//void compute_image(struct bitmap* bm, double xmin, double xmax, double ymin, double ymax, int max)
void* compute_image(struct thread_args* args)
{
    int i, j;
    int width = bitmap_width(args->b);

	// Transfer data from thread_args struct to vars for readability
    int height = args->new_height;
    j = args->prev_height;
    double xmin = args->xmin;
    double xmax = args->xmax;
    double ymin = args->ymin;
    double ymax = args->ymax;
    double max = args->itermax;
	int count;
    // For every pixel in the image...

    for (; j < height; j++) {
        for (i = 0; i < width; i++) {

            // Determine the point in x,y space for that pixel.
            double x = xmin + i * (xmax - xmin) / args->width;
            double y = ymin + j * (ymax - ymin) / args->true_height;

            // Compute the iterations at that point.
            int iters = iterations_at_point(x, y, max);

            // Set the pixel in the bitmap.

            bitmap_set(args->b, i, j, iters);
        }
    }
    return 0;
}

/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point(double x, double y, int max)
{
    double x0 = x;
    double y0 = y;

    int iter = 0;

    while ((x * x + y * y <= 4) && iter < max) {

        double xt = x * x - y * y + x0;
        double yt = 2 * x * y + y0;

        x = xt;
        y = yt;

        iter++;
    }

    return iteration_to_color(iter, max);
}

/*
Convert a iteration number to an RGBA color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/

int iteration_to_color(int i, int max)
{
    int gray = 255 * i / max;
    return MAKE_RGBA(gray, gray, gray, 0);
}
