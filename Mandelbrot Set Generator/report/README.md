@name: Nathan Redmond
@email: nathaniel.d.redmond@vanderbilt.edu
@vunetid: redmonnd

Purpose: 
The purpose of this experiment was to evenly distribute the amount of work done in creating a mandelbrot image among different threads so as to speed up the process of creating said image. Two different images from the mandelbrot set were generated on a varying number of threads (1, 2, 3, 4, 5, 10, 50, and 100). Each measurement was taken 5 times and the minimum value from each was used in the report, so as to limit the effects of external factors on the runtime of the program. This information was used in determining the optimal number of threads in minimizing the runtime of generating the image. The experiments were run using the UTM Linux VM with 5 CPU cores. Command line arguments for image A were "./mandel -x -.5 -y .5 -s 1 -m 2000 -n (# threads)" and the arguments for image B were "./mandel -x 0.2869325 -y 0.0142905 -s .000001 -W 1024 -H 1024 -m 1000 -n (# threads)". 

Analysis:
For image A, increasing the number of threads did not decrease the runtime by a consistent amount. For example, the difference in runtime when the program was run with 1 thread and 2 threads was merely 1.5 seconds, while running the program with 3 threads as opposed to 2 resulted in a 4 second decrease in runtime. Generally, as thread number increased, the runtime decreased, albeit sporadically. Increasing the number of threads past 10 hardly had any effect on the time required to generate the image, and at a certain point, time required even began to increase (likely because of the overhead required in instantiating a new thread_args struct for each thread).

For image B, increasing the number of threads resulted in a much more predictable decrease in runtime. The shape of the runtime v. num_threads graph strongly resembles a logarithmic curve. In fact, changing from 1 to 2 threads cut the runtime in half. This is likely because of the fact that image B is a very large image file when compared to image A, and therefore involves much more computation. The sole purpose of multiple threads in this case is to deal with computationally intensive processes, so we should expect computationally expensive processes such as generating image B to be greatly affected by the number of threads. 

From the data collected for both images, it appears that using between 10 and 50 threads in the generation of these mandelbrot images is optimal. Using a number of threads greater than this results in very marginal decreases in runtime and can even cause increases in runtime due to the overhead involved in creating such a large number of threads. 
 
