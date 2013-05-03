/**************************
	traceray.c
**************************/

void traceRay()
{
	long index;
	index =  0;
	ThreadData_t tData;

	tData.c = (int)index * nThreadUnit;
	int lastCol = tData.c + nThreadUnit;
	int i,j,k,col,row;
	int p,q; 
	RGBColor_t auxColor;
	RGBColor_t returnColor;
	Point3D_t pp;
	auxColor.r = 0;
	auxColor.g = 0;
	auxColor.b = 0;
	int samplesSqrt;
	//int sampleIndex;
	samplesSqrt = (int)sqrt(vp.numSamples);
	ShadeRec_t sr;
	sr.hitAnObject = 0;
	sr.material = NULL;
	sr.hitPoint.x = 0.0;
	sr.hitPoint.y = 0.0;
	sr.hitPoint.z = 0.0;
	sr.normal.x = 0.0;
	sr.normal.y = 0.0;
	sr.normal.z = 0.0;
	sr.ray.o.x = 0.0;
	sr.ray.o.y = 0.0;
	sr.ray.o.z = 0.0;
	sr.ray.d.x = 0.0;
	sr.ray.d.y = 0.0;
	sr.ray.d.z = 0.0;
	sr.depth = 0;
	//sr.color.r = sr.color.g = sr.color.b = 0.0;
	sr.t = 0.0;
	sr.u = 0.0;
	sr.v = 0.0;
		//FLOAT_T zw = 100.0; //hardwired in
		tData.ray.o.x = camera.eye.x;
		tData.ray.o.y = camera.eye.y;
		tData.ray.o.z = camera.eye.z;
		//printf("Entrando For con datos: col =%d lastCol=%d \n",tData.c,lastCol);
		for( row = 0; row < vp.vres ; row++)
		{
			for(col = tData.c; col < lastCol ; col++)
			{
				//tData.ray.o.y = vp.s * (row - vp.vres / 2.0 + 0.5);
				//tData.ray.o.y = vp.s * (row - 0.5 *(vp.vres - 1.0));
				/******************************
				Regular Antialiasing, for jittered antialiasing
				change q + 0.5 with q + rand_FLOAT_T, 
				the same for p + 0.5 
				******************************/
				for(p = 0; p < samplesSqrt ; p++)
				{
					for(q = 0; q < samplesSqrt;q++)
					{
						//Regular sampling
						pp.x = vp.s * (col - 0.5*vp.hres + (q + 0.5) / samplesSqrt);
						pp.y = vp.s * (row - 0.5*vp.vres + (p + 0.5) / samplesSqrt);
						
						//Hammersey Sampling
						/*sampleIndex = getRandomSample();
						pp.x = samples[sampleIndex].x;
						pp.y = samples[sampleIndex].y;
						pp.x = vp.s * (col - 0.5*vp.hres + pp.x);
						pp.y = vp.s * (row - 0.5*vp.vres + pp.y);
						*/
						
						tData.ray.d = rayDirection(pp.x,pp.y);
						hitObjects(tData.ray.o, tData.ray.d, &sr);
						if(sr.hitAnObject)
						{
							//For specular reflection
							sr.ray.o.x = camera.eye.x;
							sr.ray.o.y = camera.eye.y;
							sr.ray.o.z = camera.eye.z;
							sr.ray.d.x = tData.ray.d.x;
							sr.ray.d.y = tData.ray.d.y;
							sr.ray.d.z = tData.ray.d.z;
							returnColor = shade(sr);
							auxColor.r += returnColor.r;
							auxColor.g += returnColor.g;
							auxColor.b += returnColor.b;
						}
						else
						{
							auxColor.r += backColor.r;
							auxColor.g += backColor.g;
							auxColor.b += backColor.b;
						}
						sr.hitAnObject = 0;
						sr.material = NULL;
						sr.hitPoint.x = 0.0;
						sr.hitPoint.y = 0.0;
						sr.hitPoint.z = 0.0;
						sr.normal.x = 0.0;
						sr.normal.y = 0.0;
						sr.normal.z = 0.0;
						sr.ray.o.x = 0.0;
						sr.ray.o.y = 0.0;
						sr.ray.o.z = 0.0;
						sr.ray.d.x = 0.0;
						sr.ray.d.y = 0.0;
						sr.ray.d.z = 0.0;
						sr.depth = 0;
						//sr.color.r = sr.color.g = sr.color.b = 0.0;
						sr.t = 0.0;
						sr.u = 0.0;
						sr.v = 0.0;
					}
				}
				auxColor.r /= vp.numSamples;
				auxColor.g /= vp.numSamples;
				auxColor.b /= vp.numSamples;	
				//displayPixel(row, col,auxColor);
				displayPixel(row, col,auxColor);
				auxColor.r = 0.0;
				auxColor.g = 0.0;
				auxColor.b = 0.0;	
			}
		}

			//outputIndex reset
			outputIndex = 0;
			//Uptade the frame
			for( i = 0 ; i < vp.hres ; i++)
			{
				for( j = 0; j < vp.vres ; j++)
				{
					for( k = 0 ; k< 3 ; k++)
					{
						outputFrame[outputIndex]= (GLubyte)(frame[j][i][k]);
						outputIndex++;
					}
				}
			}
			updateObjects();
			updateLights();
			updateCamera();
			//endRayTrace = 1;
			gettimeofday(&later,NULL);
			totalTime += (FLOAT_T)timeval_diff(NULL,&later,&earlier);
			nFrame += 1.0;
			printf("\rfps: %f ",nFrame/(totalTime/1000000.0));
			fflush(stdout);
			gettimeofday(&earlier,NULL);
			
	return;	
}
