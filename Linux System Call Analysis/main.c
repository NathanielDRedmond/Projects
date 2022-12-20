#include <stdio.h>

void f()
{
  int i, x = 0;
  for (i = 0; i < 10; i++)
    x++;

  printf("Value of x: %d\n", x);
}

int main(int argc, char* argv[])
{
  f();
  printf("Hello, world!\n");
}

