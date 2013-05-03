/*******************
	object.c (object related functions)
*******************/

void buildObjects()
{
	int i;
	//Objects
	objects = (Object_t**)malloc(nObjects*sizeof(Object_t*));
	for( i = 0; i< nObjects ;i++)
	{
		if( i == 0)
		{
			objects[i] = (Object_t*)malloc(sizeof(Object_t));
			objects[i]->type = SPHERE;
			objects[i]->center.x = -50.0f;
			objects[i]->center.y = 40.0f;
			objects[i]->center.z = -60.0f;
			objects[i]->radius 	= 30.0f;
			objects[i]->dx 	= 1.0 * axisVelocity;
			objects[i]->dy 	= 0.0;
			objects[i]->dz 	= 0.0;
			objects[i]->material = (Material_t*)malloc(sizeof(Material_t));
			objects[i]->material->type = PHONG;
			objects[i]->material->ambientBrdf.ka = 0.25; //ka = kd because ambient reflection is diffuse reflection 
			objects[i]->material->ambientBrdf.kd = 0.25;
			objects[i]->material->diffuseBrdf.kd = 0.6;
			objects[i]->material->ambientBrdf.cd.r = 0.75;
			objects[i]->material->ambientBrdf.cd.g = 0.75;
			objects[i]->material->ambientBrdf.cd.b = 0.0;
			objects[i]->material->diffuseBrdf.cd.r = 0.75;
			objects[i]->material->diffuseBrdf.cd.g = 0.75;
			objects[i]->material->diffuseBrdf.cd.b = 0.0;
			objects[i]->material->specularBrdf.cd.r = 0.75;
			objects[i]->material->specularBrdf.cd.g = 0.75;
			objects[i]->material->specularBrdf.cd.b = 0.0;
			objects[i]->material->specularBrdf.ks = 0.2;
			objects[i]->material->specularBrdf.exp = 50;
			objects[i]->material->plane = 0;
			//objects[i]->material->specularBrdf.kd = 0.65; //not used
			//objects[i]->material->specularBrdf.ka = 0.25; //not used
		}
		if( i == 1)
		{
			objects[i] = (Object_t*)malloc(sizeof(Object_t));
			objects[i]->type = SPHERE;
			objects[i]->center.x = 20.0;
			objects[i]->center.y = 50.0;
			objects[i]->center.z = -20.0;
			objects[i]->radius 	= 40.0;
			objects[i]->dx 	= 0.0;
			objects[i]->dy 	= -1.0 * axisVelocity;
			objects[i]->dz 	= 0.0;
			objects[i]->material = (Material_t*)malloc(sizeof(Material_t));
			objects[i]->material->type = PHONG;
			objects[i]->material->ambientBrdf.ka = 0.25; //ka = kd because ambient reflection is diffuse reflection 
			objects[i]->material->ambientBrdf.kd = 0.25;
			objects[i]->material->diffuseBrdf.kd = 0.6;
			objects[i]->material->ambientBrdf.cd.r = 0.75;
			objects[i]->material->ambientBrdf.cd.g = 0.0;
			objects[i]->material->ambientBrdf.cd.b = 0.75;
			objects[i]->material->diffuseBrdf.cd.r = 0.75;
			objects[i]->material->diffuseBrdf.cd.g = 0.0;
			objects[i]->material->diffuseBrdf.cd.b = 0.75;
			objects[i]->material->specularBrdf.cd.r = 0.75;
			objects[i]->material->specularBrdf.cd.g = 0.0;
			objects[i]->material->specularBrdf.cd.b = 0.75;
			objects[i]->material->specularBrdf.ks = 0.2;
			objects[i]->material->specularBrdf.exp = 50;
			objects[i]->material->plane = 0;
			//objects[i]->material->specularBrdf.kd = 0.65; //not used
			//objects[i]->material->specularBrdf.ka = 0.25; //not used 
		}
		if( i == 2)
		{
			objects[i] = (Object_t*)malloc(sizeof(Object_t));
			objects[i]->type = PLANE;
			objects[i]->a.x = 0.0;
			objects[i]->a.y = -1.0;
			objects[i]->a.z = 0.0;
			objects[i]->n.x = 0.0;
			objects[i]->n.y = 1.0;
			objects[i]->n.z = 0.0;
			objects[i]->dx 	= 0.0;
			objects[i]->dy 	= 0.0;
			objects[i]->dz 	= 0.0;
			objects[i]->material = (Material_t*)malloc(sizeof(Material_t));
			objects[i]->material->type = PHONG;
			objects[i]->material->ambientBrdf.ka = 0.25; //ka = kd because ambient reflection is diffuse reflection 
			objects[i]->material->ambientBrdf.kd = 0.25;
			objects[i]->material->diffuseBrdf.kd = 0.6;
			objects[i]->material->ambientBrdf.cd.r = 0.0;
			objects[i]->material->ambientBrdf.cd.g = 0.75;
			objects[i]->material->ambientBrdf.cd.b = 0.0;
			objects[i]->material->diffuseBrdf.cd.r = 0.0;
			objects[i]->material->diffuseBrdf.cd.g = 0.75;
			objects[i]->material->diffuseBrdf.cd.b = 0.0;
			objects[i]->material->specularBrdf.cd.r = 0.0;
			objects[i]->material->specularBrdf.cd.g = 0.75;
			objects[i]->material->specularBrdf.cd.b = 0.0;
			objects[i]->material->specularBrdf.ks = 0.2;
			objects[i]->material->specularBrdf.exp = 50;
			objects[i]->material->plane = 1;
		}
	}	
}

void hitObjects(Point3D_t rayO, Vector3D_t rayD,ShadeRec_t* sr)
{
	int i;
	FLOAT_T 		t;
	Normal_t 	normal;
	Point3D_t 	localHitPoint;
	FLOAT_T		tmin = kHugeValue;
	
	for(i = 0; i < nObjects; i++)
	{
		switch(objects[i]->type)
		{
			case SPHERE:
				if(hitSphere(rayO,rayD,&t,sr,objects[i]->center,objects[i]->radius) && (t < tmin))
				{
					sr->hitAnObject = 1;
					tmin = t;
					sr->material = objects[i]->material;
					//sr.local_hit_point = ray.o + t * ray.d;
					sr->hitPoint.x = rayO.x + (t*rayD.x);
					sr->hitPoint.y = rayO.y + (t*rayD.y);
					sr->hitPoint.z = rayO.z + (t*rayD.z); 
					normal.x = sr->normal.x;
					normal.y = sr->normal.y;
					normal.z = sr->normal.z;
					localHitPoint.x = sr->localHitPoint.x;
					localHitPoint.y = sr->localHitPoint.y;
					localHitPoint.z = sr->localHitPoint.z;
				}
				break;
			case PLANE:
				if(hitPlane(rayO,rayD,&t,sr,objects[i]->a,objects[i]->n) && (t < tmin))
				{
					sr->hitAnObject = 1;
					tmin = t;
					sr->material = objects[i]->material;
					//sr.local_hit_point = ray.o + t * ray.d;
					sr->hitPoint.x = rayO.x + (t*rayD.x);
					sr->hitPoint.y = rayO.y + (t*rayD.y);
					sr->hitPoint.z = rayO.z + (t*rayD.z); 
					normal.x = sr->normal.x;
					normal.y = sr->normal.y;
					normal.z = sr->normal.z;
					localHitPoint.x = sr->localHitPoint.x;
					localHitPoint.y = sr->localHitPoint.y;
					localHitPoint.z = sr->localHitPoint.z;
				}
				break;
		}
	}
	if(sr->hitAnObject)
	{
		sr->t = tmin;
		sr->normal.x = normal.x;
		sr->normal.y = normal.y;
		sr->normal.z = normal.z;
		sr->localHitPoint.x = localHitPoint.x ;
		sr->localHitPoint.y = localHitPoint.y ;
		sr->localHitPoint.z = localHitPoint.z ;	
	}
		
}

int hitPlane(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t* sr, Point3D_t a, Normal_t n)
{
	Point3D_t aux1;
	
	aux1.x = a.x - rayO.x;
	aux1.y = a.y - rayO.y;
	aux1.z = a.z - rayO.z;
	
	//FLOAT_T t = (a - ray.o) * n / (ray.d * n); 
	
	FLOAT_T t = (aux1.x * n.x + aux1.y * n.y + aux1.z * n.z) / (rayD.x * n.x + rayD.y * n.y + rayD.z * n.z);													
	if (t > kEpsilon) 
	{
		*tmin = t;
		sr->normal.x = n.x;
		sr->normal.y = n.y;
		sr->normal.z = n.z;		//sr.local_hit_point = ray.o + t * ray.d;
		sr->localHitPoint.x = rayO.x + (t*rayD.x);
		sr->localHitPoint.y = rayO.y + (t*rayD.y);
		sr->localHitPoint.z = rayO.z + (t*rayD.z); 
		return 1;	
	}
	return 0;	
}

int shadowHitPlane(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t sr, Point3D_t a, Normal_t n)
{
	Point3D_t aux1;
	
	aux1.x = a.x - rayO.x;
	aux1.y = a.y - rayO.y;
	aux1.z = a.z - rayO.z;
	
	//FLOAT_T t = (a - ray.o) * n / (ray.d * n); 
	
	FLOAT_T t = (aux1.x * n.x + aux1.y * n.y + aux1.z * n.z) / (rayD.x * n.x + rayD.y * n.y + rayD.z * n.z);													
	if (t > kEpsilon) 
	{
		*tmin = t;		
		return 1;	
	}
	return 0;
}

int hitSphere(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t* sr, Point3D_t center, FLOAT_T radius)
{
	FLOAT_T t;
	Vector3D_t temp;
	temp.x	= rayO.x - center.x;
	temp.y	= rayO.y - center.y;
	temp.z	= rayO.z - center.z;
	
	FLOAT_T a = rayD.x*rayD.x + rayD.y*rayD.y + rayD.z * rayD.z;
	FLOAT_T b = 2.0 * (temp.x*rayD.x + temp.y*rayD.y + temp.z * rayD.z);
	FLOAT_T c = (temp.x*temp.x + temp.y*temp.y + temp.z * temp.z) - radius * radius;
	FLOAT_T disc = b * b - 4.0 * a * c;

	if(disc < 0.0)
		return 0;
	else
	{
		FLOAT_T e = sqrt(disc);
		FLOAT_T denom = 2.0 * a;
		t = (-b - e) /denom; //smaller root

		if ( t > kEpsilon)
		{
			*tmin = t;
			//sr.normal 	 = (temp + t * ray.d) / radius;
			sr->normal.x = (temp.x + (t* rayD.x))/ radius;
			sr->normal.y = (temp.y + (t* rayD.y))/ radius;
			sr->normal.z = (temp.z + (t* rayD.z))/ radius;
			FLOAT_T len = sqrt(sr->normal.x*sr->normal.x + sr->normal.y*sr->normal.y + sr->normal.z*sr->normal.z);
			sr->normal.x /= len;
			sr->normal.y /= len;
			sr->normal.z /= len;
			
			//sr.local_hit_point = ray.o + t * ray.d;
			sr->localHitPoint.x = rayO.x + (t*rayD.x);
			sr->localHitPoint.y = rayO.y + (t*rayD.y);
			sr->localHitPoint.z = rayO.z + (t*rayD.z);
			return  1;
		}
		
		t = (-b + e ) /denom; //larger root
		
		if (t > kEpsilon)
		{
			*tmin = t;
			//sr.normal 	 = (temp + t * ray.d) / radius;
			sr->normal.x = (temp.x + (t* rayD.x))/ radius;
			sr->normal.y = (temp.y + (t* rayD.y))/ radius;
			sr->normal.z = (temp.z + (t* rayD.z))/ radius;
			FLOAT_T len = sqrt(sr->normal.x*sr->normal.x + sr->normal.y*sr->normal.y + sr->normal.z*sr->normal.z);
			sr->normal.x /= len;
			sr->normal.y /= len;
			sr->normal.z /= len;
			//sr.local_hit_point = ray.o + t * ray.d;
			sr->localHitPoint.x = rayO.x + (t*rayD.x);
			sr->localHitPoint.y = rayO.y + (t*rayD.y);
			sr->localHitPoint.z = rayO.z + (t*rayD.z);
			return 1;
		}
	}	
	return 0;
	
}

int shadowHitSphere(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t sr, Point3D_t center, FLOAT_T radius)
{
	FLOAT_T t;
	Vector3D_t temp;
	temp.x	= rayO.x - center.x;
	temp.y	= rayO.y - center.y;
	temp.z	= rayO.z - center.z;
	
	FLOAT_T a = rayD.x*rayD.x + rayD.y*rayD.y + rayD.z * rayD.z;
	FLOAT_T b = 2.0 * (temp.x*rayD.x + temp.y*rayD.y + temp.z * rayD.z);
	FLOAT_T c = (temp.x*temp.x + temp.y*temp.y + temp.z * temp.z) - radius * radius;
	FLOAT_T disc = b * b - 4.0 * a * c;

	if(disc < 0.0)
		return 0;
	else
	{
		FLOAT_T e = sqrt(disc);
		FLOAT_T denom = 2.0 * a;
		t = (-b - e) /denom; //smaller root

		if ( t > kEpsilon)
		{
			*tmin = t;

			return  1;
		}
		
		t = (-b + e ) /denom; //larger root
		
		if (t > kEpsilon)
		{
			*tmin = t;
			return 1;
		}
	}	
	return 0;
	
}

int inShadowAmbient(Ray_t ray, ShadeRec_t sr)
{	
	int i;
	FLOAT_T t;
	int returnValue = 0;
	for(i = 0; i < nObjects; i++)
	{
		switch(objects[i]->type)
		{
			case SPHERE:
				if(shadowHitSphere(ray.o,ray.d,&t,sr,objects[i]->center,objects[i]->radius) )
					returnValue = 1;
				break;
			case PLANE:
				if(shadowHitPlane(ray.o,ray.d,&t,sr,objects[i]->a,objects[i]->n) )
					returnValue = 1;
				break;
		}
	}
	//if(returnValue)
	//	printf("La sombra toco un objeto en ambient\n");
	return returnValue;
	
}

//For Point Light
int inShadow(Ray_t ray, ShadeRec_t sr, Vector3D_t lightLocation)
{
	FLOAT_T t;
	int i;
	int returnValue = 0;
	FLOAT_T d = 	sqrt(	(lightLocation.x - ray.o.x) * (lightLocation.x - ray.o.x) 
					+ 	(lightLocation.y - ray.o.y) * (lightLocation.y - ray.o.y)
					+	(lightLocation.z - ray.o.z) * (lightLocation.z - ray.o.z) );

	for(i = 0; i < nObjects; i++)
	{
		switch(objects[i]->type)
		{
			case SPHERE:
				if(shadowHitSphere(ray.o,ray.d,&t,sr,objects[i]->center,objects[i]->radius) && (t < d))
					returnValue = 1;
				break;
			case PLANE:
				if(shadowHitPlane(ray.o,ray.d,&t,sr,objects[i]->a,objects[i]->n) && (t < d))
					returnValue = 1;
				break;
		}
	}
	//if(returnValue)
		//printf("La sombra toco un objeto en pointlight\n");
	return returnValue;
}

void updateObjects()
{
	int i;
	for(i = 0; i < nObjects ; i++)
	{
		if(objects[i]->type == SPHERE)
		{
			objects[i]->center.x += objects[i]->dx;
			objects[i]->center.y += objects[i]->dy;
			objects[i]->center.z += objects[i]->dz;
			
			if(	objects[i]->center.x > 60.0 || objects[i]->center.x < -60.0)
			{
				if( objects[i]->center.x > 0.0)
					objects[i]->center.x = 60.0;
				else
					objects[i]->center.x = -60;
					
				objects[i]->dx*=-1;	
			}
			
			if(	objects[i]->center.y > 60.0 || objects[i]->center.y < (objects[i]->radius - 0.0))
			{
				if( objects[i]->center.y > 60.0)
					objects[i]->center.y = 60.0;
				else
					objects[i]->center.y = objects[i]->radius - 0.0 ;

				objects[i]->dy*=-1;	
			}
			
			if(	objects[i]->center.z > 60.0 || objects[i]->center.z < -60.0)
			{
				if( objects[i]->center.z > 60.0)
					objects[i]->center.z = 60.0;
				else
					objects[i]->center.z = -60.0;
					
				objects[i]->dz*=-1;	
			}
		}
	}
	
	for(i = 0; i < nObjects-1 ; i++)
	{
		if(objects[i]->type == SPHERE)
		{
			FLOAT_T distance = 	
				(objects[i]->center.x - objects[i+1]->center.x) * (objects[i]->center.x - objects[i+1]->center.x) +
				(objects[i]->center.y - objects[i+1]->center.y) * (objects[i]->center.y - objects[i+1]->center.y) +
				(objects[i]->center.z - objects[i+1]->center.z) * (objects[i]->center.z - objects[i+1]->center.z) ;
				
			FLOAT_T minDistance = objects[i]->radius + objects[i+1]->radius;
			if(activado)
			{
				if(	distance <= (minDistance * minDistance) + kEpsilon)
				{
					
					//activado = 0;
					/*
					objects[i]->dy*=-1;
					objects[i]->dx*=-1;
					objects[i]->dz*=-1;
					objects[i+1]->dy*=-1;
					objects[i+1]->dx*=-1;
					objects[i+1]->dz*=-1;
					*/
					Vector3D_t normal;
					normal.x = objects[i]->center.x - objects[i+1]->center.x;
					normal.y = objects[i]->center.y - objects[i+1]->center.y;
					normal.z = objects[i]->center.z - objects[i+1]->center.z;
					
					FLOAT_T mod = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
					normal.x /= mod;
					normal.y /= mod;
					normal.z /= mod;
					
					objects[i]->dx		=	normal.x*axisVelocity;
					objects[i]->dy		=	normal.y*axisVelocity;
					objects[i]->dz		=	normal.z*axisVelocity;
					objects[i+1]->dx	=	-normal.x*axisVelocity;
					objects[i+1]->dy	=	-normal.y*axisVelocity;
					objects[i+1]->dz	=	-normal.z*axisVelocity;
					
					/*
					objects[i]->dx		=	-normal.x;
					objects[i]->dy		=	-normal.y;
					objects[i]->dz		=	-normal.z;
					objects[i+1]->dx	=	normal.x;
					objects[i+1]->dy	=	normal.y;
					objects[i+1]->dz	=	normal.z;
					*/
				}
			}
		}
	}	
}
