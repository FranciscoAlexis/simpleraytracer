simpleraytracer
===============

Simple ray tracer in C

* rt is a ray tracer implemented with posix threads for multicore processors
* nrt is a ray tracer without threads (same algorithm)


In both projects you can configure the raytracer with the .cfg file.

Tested on Mac OS X and Ubuntu Linux, to swap between platforms you should check this lines in main.c:

	//OS X
	#include <GLUT/glut.h>
	//LINUX
	//#include <GL/glut.h>

The code is highly commented so you can understand where i mess everything up.

Compiling
---------
	$ make

Running
-------
	$ ./bin/raytracer raytracer.cfg