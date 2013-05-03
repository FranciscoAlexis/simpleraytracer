/*******************

configure.c

*******************/

enum lcfg_status assignValues(const char *key, void *data, size_t len, void *user_data)
{
	//Number of objects on the scene
	if(strcmp(key,"NOBJECTS") == 0)
	{
		nObjects = atoi((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	
	//Number of objects on the scene
	if(strcmp(key,"NLIGHTS") == 0)
	{
		nLights = atoi((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	
	//Number of threads
	if(strcmp(key,"NTHREADS") == 0)
	{
		NTHREADS = atoi((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	
	/***************************
		VIEWPLANE SET
	****************************/
	if(strcmp(key,"HRES") == 0)
	{
		vp.hres = atoi((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"VRES") == 0)
	{
		vp.vres = atoi((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"NUM_SAMPLES") == 0)
	{
		vp.numSamples = atoi((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"VP_S") == 0)
	{
		vp.s = atof((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"VP_GAMMA") == 0)
	{
		vp.gamma = atof((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"VP_INVGAMMA") == 0)
	{
		vp.invGamma = atof((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"VP_SHOWOUTOFGAMUT") == 0)
	{
		vp.showOutOfGamut = atoi((const char *)data);
		//printf("Vres : %s\n",(const char *)data);	
	}
	/***************************
		CAMERA SET
	****************************/
	if(strcmp(key,"CAMERA_EYE_X") == 0)
	{
		camera.eye.x = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_EYE_Y") == 0)
	{
		camera.eye.y = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_EYE_Z") == 0)
	{
		camera.eye.z = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_LOOKAT_X") == 0)
	{
		camera.lookAt.x = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_LOOKAT_Y") == 0)
	{
		camera.lookAt.y = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_LOOKAT_Z") == 0)
	{
		camera.lookAt.z = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_UP_X") == 0)
	{
		camera.up.x = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_UP_Y") == 0)
	{
		camera.up.y = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_UP_Z") == 0)
	{
		camera.up.z = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_ROLL_ANGLE") == 0)
	{
		camera.rollAngle = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}

	if(strcmp(key,"CAMERA_D") == 0)
	{
		camera.d = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	if(strcmp(key,"CAMERA_ZOOM") == 0)
	{
		camera.zoom = atof((const char *)data);
		//printf("Hres : %s\n",(const char *)data);	
	}
	
    return lcfg_status_ok;
}

int configure()
{
	/******************************
		Variable read & set
	*******************************/
    struct lcfg *c = lcfg_new("raytracer.cfg");
	
	if( lcfg_parse(c) != lcfg_status_ok )
    {
        printf("lcfg error: %s\n", lcfg_error_get(c));
        return -1;
    }
    else
    {
        lcfg_accept(c, assignValues, 0);
    }

	/******************************
		Variable set
	*******************************/
	//timeToWait
	timeToWait.tv_sec = 0; 
	timeToWait.tv_nsec = 100000; 
	//nThreadUnit = how many pixel units will handle every thread
	nThreadUnit = vp.hres / NTHREADS;
	vp.s /= camera.zoom;
	//set camera exposureTime and compute uvw
	camera.exposureTime = 1.0;
	camera.dx = camera.dy = camera.dz = 0.0;
	camera.ax = camera.ay = camera.az = 0.0;
	camera.angle = PI / 2.0;
	camera.radius = camera.eye.z;
	computeUVW();
	
	int i;
	//int n = (int)sqrt(vp.numSamples);
	samples = (Point3D_t*)malloc(vp.numSamples*sizeof(Point3D_t));
	for(i = 0; i < vp.numSamples;i++)
	{
		samples[i].x = (FLOAT_T)i / (FLOAT_T)vp.numSamples;
		samples[i].y = phi(i);
		samples[i].z = 0;
	}
	
	hSamples = (Point3D_t*)malloc(vp.numSamples*sizeof(Point3D_t));
	for(i = 0; i < vp.numSamples;i++)
	{
		hSamples[i].x = (float)rand()/(float)((unsigned)RAND_MAX);
		hSamples[i].y = (float)rand()/(float)((unsigned)RAND_MAX);
		hSamples[i].z = 0;
	}
	
	for(i = 0; i < vp.numSamples;i++)
	{
		/*
		FLOAT_T cos_phi = cos(2.0 * PI * samples[i].x);
		FLOAT_T sin_phi = sin(2.0 * PI * samples[i].x);
		FLOAT_T cos_theta = pow((1.0 - samples[i].y),1.0/(e + 1.0));
		*/
		FLOAT_T cos_phi = cos(2.0 * PI * hSamples[i].x);
		FLOAT_T sin_phi = sin(2.0 * PI * hSamples[i].x);
		FLOAT_T cos_theta = pow((1.0 - hSamples[i].y),1.0/(e + 1.0));
		FLOAT_T sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		hSamples[i].x = sin_theta * cos_phi;
		hSamples[i].y = sin_theta * sin_phi;
		hSamples[i].z = cos_theta;
			
	}
	
	totalTime = 0.0;
	nFrame = 0.0;
	//backColor
	backColor.r = 0.0;
	backColor.g = 0.0; 
	backColor.b = 0.0;

    lcfg_delete(c); 
    activado = 1;
    return 0;
}
