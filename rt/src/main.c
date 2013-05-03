/* 	
*	Francisco Rojas
* 	say Something funny here
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
//#include <unistd.h>
#include <time.h>
//OS X
#include <GLUT/glut.h>
//LINUX
//#include <GL/glut.h>

//#define HRES 400
//#define VRES 400

#include "definitions.h" 
/*****************************************
	Variable Definition
******************************************/
int 				NTHREADS;
int 				pIndex;
int*** 				frame;
GLubyte*			outputFrame;
int					outputIndex;
int 				renderFrame;
ViewPlane_t 		vp;
Camera_t			camera;
pthread_t*			threads;
pthread_attr_t 		pthread_custom_attr;
pthread_mutex_t 	mutexFinished;
int 				nThreadsFinished;
int 				endRayTrace;
long 				threadIndex;
FLOAT_T				totalTime;
FLOAT_T				nFrame;
Object_t** 			objects;
Light_t**			lights;
int 				nObjects;
int					nLights;
RGBColor_t			backColor;
int 				drawPixelsFlag;
int 				nThreadUnit;
Point3D_t*			samples;	//hemisfere samples
Point3D_t*			hSamples;	//hemisfere samples
pthread_mutex_t 	count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t 	condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  	condition_cond  = PTHREAD_COND_INITIALIZER;
struct timespec 	timeToWait;

struct timeval 		earlier;
struct timeval 		later;
struct timeval 		interval;
int 				activado;
#include "lcfg/lcfg_static.h"
#include "util/util.h"
#include "color/displayPixel.h"
#include "color/shade.h"
#include "traceray/traceray.h"
#include "configuration/configure.h"
#include "camera/camera.h"
#include "BRDF/brdf.h"
#include "light/light.h"
#include "object/object.h"
#include "time/time.h"

#include "lcfg/lcfg_static.c"
#include "util/util.c"
#include "color/displayPixel.c"
#include "color/shade.c"
#include "traceray/traceray.c"
#include "configuration/configure.c"
#include "camera/camera.c"
#include "BRDF/brdf.c"
#include "light/light.c"
#include "object/object.c"
#include "time/time.c"

/******************************************
	render (draws pixels on screen)
*******************************************/
void render(void)
{
	if(renderFrame)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode ( GL_PROJECTION );
		glLoadIdentity ();
		gluOrtho2D ( 0, vp.vres, 0, vp.hres );
		glRasterPos2i(0, 0);
		glDrawPixels(vp.hres,vp.vres,GL_RGB,GL_UNSIGNED_BYTE,outputFrame);
		glFlush();
		glutSwapBuffers();
		renderFrame = 0;
	}
	else
	{
		nanosleep(&timeToWait,NULL);	
	}
	if(endRayTrace)
	{
		//salir y jointhreads y todo eso
	}
}

void processNormalKeys(unsigned char key, int x, int y) 
{

	if (key == 27) 
		exit(0);
		
	switch(key) 
	{
		case 'a' : 
				camera.dx = 1.0;	
				break;
		case 'd' :
				camera.dx = -1.0;	
				break;	
		case 'w' : 
				camera.dz = -10.0;
				break;
		case 's' : 
				camera.dz = 10.0;	
				break;
		/*
		case 'e' : 
				camera.dy = 1.0;	
				break;
		case 'q' : 
				camera.dy = -1.0;	
				break;
		*/
	}
}

void processSpecialKeys(int key, int x, int y) 
{

}

void prepareVisualization(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(vp.hres, vp.vres);
	glutInitWindowPosition(200, 200);
	glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGBA | GLUT_DEPTH);	
	glutCreateWindow("Raytracer");
	glutDisplayFunc(render);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutIdleFunc(render);
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	outputFrame = (GLubyte*)malloc(vp.hres*vp.vres*3*sizeof(GLubyte));
	outputIndex = 0;
	renderFrame = 0;
}

int main(int argc,char** argv)
{
	int i,j,k,c;
	gettimeofday(&earlier,NULL);

    //Read the input file and parse the options
    if(configure() == -1)
    	return -1;
    
	//IF WERE PRINTING ONE FRAME TO A PPM FILE USE THIS:
	//printf("P6\n%i %i\n255\n", vp.hres, vp.vres);				
	//prepare visualization system
	prepareVisualization(argc,argv);
	//enoguh memory for each thread
	threads=(pthread_t *)malloc(NTHREADS*sizeof(*threads));
	//custom attr init
	pthread_attr_init(&pthread_custom_attr);
	//init mutex
	pthread_mutex_init(&mutexFinished, NULL);
	
	//init number of threads done
	nThreadsFinished = 0;
	//don't end rayTrace until this value turns to 1
	endRayTrace = 0;
	
	//Dymamic memory allocation for 3D array frame
	frame = (int ***)malloc(vp.hres * sizeof(int **));
	for( i = 0 ; i < vp.hres; i++)
	{
		frame[i] = (int **)malloc(vp.vres * sizeof(int *));
		for( j = 0; j < vp.vres; j++)
			frame[i][j] = (int *)malloc(3 * sizeof(int)); //3 por R G B

	}
	
	//Filling the frame with a degradated color, just for test.	
	for(i = 0 ; i < vp.hres ; i++)
		for( j = 0; j < vp.vres ; j++)
			for( k = 0 ; k< 3 ; k++)
				frame[i][j][k] = 255-i;
	
	//This variable will help to create the threads and start them when the main program is ready	
	threadIndex = 0;
	for( c = 0 ; c < vp.hres; c += nThreadUnit) //across
	{
		pthread_create(&threads[threadIndex], &pthread_custom_attr, traceRay, (void *)threadIndex);
		threadIndex++;
	}
	
	//Create the objects
	buildObjects();
	//Create the lights
	buildLights();
	
	//threads start 
	threadIndex++;
	
	glutMainLoop(); 
	//Loop Begins
	/*while(!endRayTrace)
	{
		nanosleep(&timeToWait,NULL);	
	}*/
	//Join all threads
	for (threadIndex=0; threadIndex<NTHREADS; threadIndex++)
	{
		pthread_join(threads[threadIndex],NULL);
	}

	return 0;
}
