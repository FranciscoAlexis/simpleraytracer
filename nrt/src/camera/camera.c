/********************
	camera.c
********************/


void computeUVW(void)
{
	//w = eye - lookAt; 
	camera.w.x = camera.eye.x - camera.lookAt.x; 
	camera.w.y = camera.eye.y - camera.lookAt.y;
	camera.w.z = camera.eye.z - camera.lookAt.z;
	//normalize w
	FLOAT_T length = sqrtf( camera.w.x*camera.w.x + camera.w.y*camera.w.y + camera.w.z*camera.w.z );
	camera.w.x /= length;
	camera.w.y /= length;
	camera.w.z /= length;

	// u = up cross w
	camera.u.x =  camera.up.y*camera.w.z - camera.up.z*camera.w.y;
	camera.u.y =  camera.up.z*camera.w.x - camera.up.x*camera.w.z; 
	camera.u.z =  camera.up.x*camera.w.y - camera.up.y*camera.w.x;	
	//normalize u
	length = sqrtf( camera.u.x*camera.u.x + camera.u.y*camera.u.y + camera.u.z*camera.u.z );
	camera.u.x /= length;
	camera.u.y /= length;
	camera.u.z /= length;
	
	// v = w cross u
	camera.v.x =  camera.w.y*camera.u.z - camera.w.z*camera.u.y;
	camera.v.y =  camera.w.z*camera.u.x - camera.w.x*camera.u.z; 
	camera.v.z =  camera.w.x*camera.u.y - camera.w.y*camera.u.x;
	
	length = sqrtf( camera.v.x*camera.v.x + camera.v.y*camera.v.y + camera.v.z*camera.v.z );
	camera.v.x /= length;
	camera.v.y /= length;
	camera.v.z /= length;
	
	// take care of the singularity by hardwiring in specific camera orientations
	
	if (camera.eye.x == camera.lookAt.x && camera.eye.z == camera.lookAt.z && camera.eye.y > camera.lookAt.y) 
	{ 
		// camera looking vertically down
		camera.u.x = 0.0;
		camera.u.y = 0.0;		camera.u.z = 1.0;
		
		camera.v.x = 1.0;
		camera.v.y = 0.0;		camera.v.z = 0.0;		camera.w.x = 0.0;
		camera.w.y = 1.0;		camera.w.z = 0.0;	
	}
	
	if (camera.eye.x == camera.lookAt.x && camera.eye.z == camera.lookAt.z && camera.eye.y < camera.lookAt.y) 
	{ 
		// camera looking vertically up
		camera.u.x = 1.0;
		camera.u.y = 0.0;		camera.u.z = 0.0;
		
		camera.v.x = 0.0;
		camera.v.y = 0.0;		camera.v.z = 1.0;		camera.w.x = 0.0;
		camera.w.y = -1.0;		camera.w.z = 0.0;
	}
	
}

Vector3D_t rayDirection(FLOAT_T px, FLOAT_T py)
{
	Vector3D_t dir;
	dir.x = px * camera.u.x + py * camera.v.x - camera.d * camera.w.x; 
	dir.y = px * camera.u.y + py * camera.v.y - camera.d * camera.w.y;
	dir.z = px * camera.u.z + py * camera.v.z - camera.d * camera.w.z;
	
	//normalize dir
	FLOAT_T length = sqrtf( dir.x*dir.x + dir.y*dir.y + dir.z*dir.z );
	dir.x /= length;
	dir.y /= length;
	dir.z /= length;
	
	return dir;
}

void updateCamera()
{
	int changed = 0;
	
	if(camera.dx != 0.0)
	{
		changed = 1;
		
		if(camera.dx > 0.0)
			camera.ax += CAMACCELERATION;
		else
			camera.ax -= CAMACCELERATION;
		
		if(camera.ax > CAMMAX)
			camera.ax = CAMMAX;
		if(camera.ax < -CAMMAX)
			camera.ax = -CAMMAX;
			
		camera.angle += camera.ax;
		
		if(camera.angle > 2.0 * PI)
			camera.angle = camera.angle - 2.0 * PI;
		if(camera.angle < 0.0 )
			camera.angle = 2.0 * PI + camera.angle ;
			
		camera.eye.x = camera.radius * cos(camera.angle);
		camera.eye.z = camera.radius * sin(camera.angle);	
	}
	else if(camera.ax != 0.0)
	{
		changed = 1;
		if(camera.ax > 0.0)
			camera.ax -= CAMTRACTION;
		else
			camera.ax += CAMTRACTION; 
			
		if(camera.ax > -0.02 && camera.ax < 0.02)
			camera.ax = 0.0;
			
		camera.angle += camera.ax;
		
		if(camera.angle > 2.0 * PI)
			camera.angle = camera.angle - 2.0 * PI;
		if(camera.angle < 0.0 )
			camera.angle = 2.0 * PI + camera.angle ;
		
		camera.eye.x = camera.radius * cos(camera.angle);
		camera.eye.z = camera.radius * sin(camera.angle);	
	}
	camera.dx = 0.0;
	
	if(camera.dz != 0.0)
	{
		changed = 1;
		camera.radius += camera.dz;
		
		if(camera.radius < 100.0)
		{
			camera.radius = 100.0;	
		}
		if(camera.radius > 600)
		{
			camera.radius = 600;	
		}
		
		camera.eye.x = camera.radius * cos(camera.angle);
		camera.eye.z = camera.radius * sin(camera.angle);
	}
	camera.dz = 0.0;
	
	if(changed)
		computeUVW();
}