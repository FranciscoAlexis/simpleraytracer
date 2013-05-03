/****************************
	light.c (light related functions)
****************************/

void buildLights()
{
	int i;
	
	lights = (Light_t**)malloc(nLights*sizeof(Light_t*));
	
	for( i = 0; i< nLights ;i++)
	{
		if(i == 0)
		{
			lights[i] = (Light_t*)malloc(sizeof(Light_t));
			lights[i]->type = AMBIENT;
			lights[i]->shadows = 1;
			lights[i]->ls = 1.0;
			lights[i]->minAmount = 0.0;
			lights[i]->color.r = 1.0;
			lights[i]->color.g = 1.0;
			lights[i]->color.b = 1.0;
		}
		else
		{
			lights[i] = (Light_t*)malloc(sizeof(Light_t));
			lights[i]->type = POINT;
			lights[i]->shadows = 1;
			lights[i]->ls = 3.0;
			lights[i]->location.x = 20.0f;
			lights[i]->location.y = 150.0f;
			lights[i]->location.z = -40.0f;
			//lights[i]->location.z = 40.0f;
			/*lights[i]->location.x = 10.0;
			lights[i]->location.y = 0.0;
			lights[i]->location.z = 0.0;*/
			lights[i]->dx = 4.0;
			lights[i]->dy = 4.0;
			lights[i]->dz = 4.0;
			lights[i]->color.r = 1.0;
			lights[i]->color.g = 1.0;
			lights[i]->color.b = 1.0;
		}
	}
	
}

RGBColor_t L(ShadeRec_t sr, int index)
{
	RGBColor_t returnColor;
	//instead ocuping w vector from ambien oclucion we'll use w
	Vector3D_t u,v,w;
	//Return the color from the ambient light
	if(lights[index]->type == POINT)
	{
		returnColor.r = lights[index]->ls * lights[index]->color.r;
		returnColor.g = lights[index]->ls * lights[index]->color.g;
		returnColor.b = lights[index]->ls * lights[index]->color.b; 
		return returnColor;	
	}
	if(lights[index]->type == AMBIENT)
	{
		//As ambient normal
		/*
		returnColor.r = lights[index]->ls * lights[index]->color.r;
		returnColor.g = lights[index]->ls * lights[index]->color.g;
		returnColor.b = lights[index]->ls * lights[index]->color.b; 
		return returnColor;			
		*/
		//As ambient occlusion
		w.x = sr.normal.x;
		w.y = sr.normal.y;
		w.z = sr.normal.z;	
		//v = w cross 0.0072,1.0,0.0034 in case normal is vertical
		v.x = w.y * 0.0034 - w.z * 1.0;
		v.y = w.z * 0.0072 - w.x * 0.0034;
		v.z = w.x * 1.0 - w.y * 0.0072 ;
		//normalize v
		FLOAT_T mod = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); 
		v.x /= mod;
		v.y /= mod;
		v.z /= mod;
		// u = v cross w
		u.x = v.y * w.z - v.z * w.y;
		u.y = v.z * w.x - v.x * w.z;
		u.z = v.x * w.y - v.y * w.x;
		
		//normalize u ?
		/*mod = sqrt(u.x * u.x + u.y * u.y + u.z * u.z); 
		u.x /= mod;
		u.y /= mod;
		u.z /= mod;
		*/
		Ray_t shadowRay;
		
		shadowRay.o.x = sr.hitPoint.x;
		shadowRay.o.y = sr.hitPoint.y;
		shadowRay.o.z = sr.hitPoint.z;

		//instead calling get direction wich uses u,v,w we'll calculate this values right here
		//si means Sample Index
		
		int si = getRandomSample();
		/*
		shadowRay.d.x = samples[si].x * u.x + samples[si].y * v.x + samples[si].z * w.x;
		shadowRay.d.y = samples[si].x * u.y + samples[si].y * v.y + samples[si].z * w.y;
		shadowRay.d.z = samples[si].x * u.z + samples[si].y * v.z + samples[si].z * w.z;
		*/
		
		shadowRay.d.x = hSamples[si].x * u.x + hSamples[si].y * v.x + hSamples[si].z * w.x;
		shadowRay.d.y = hSamples[si].x * u.y + hSamples[si].y * v.y + hSamples[si].z * w.y;
		shadowRay.d.z = hSamples[si].x * u.z + hSamples[si].y * v.z + hSamples[si].z * w.z;
		
		//normalize d?
		/*
		mod = sqrt( shadowRay.d.x * shadowRay.d.x + shadowRay.d.y * shadowRay.d.y + shadowRay.d.z * shadowRay.d.z);
		shadowRay.d.x /= mod;
		shadowRay.d.y /= mod;
		shadowRay.d.z /= mod; 
		*/
		
		//Regular sampling (NOT RECOMENDED)
		/*
		shadowRay.d.x = 0.5 * u.x + 0.5 * v.x + 0.5 * w.x;
		shadowRay.d.y = 0.5 * u.y + 0.5 * v.y + 0.5 * w.y;
		shadowRay.d.z = 0.5 * u.z + 0.5 * v.z + 0.5 * w.z;
		*/
		if(inShadowAmbient(shadowRay,sr))
		{
			returnColor.r = lights[index]->minAmount * lights[index]->ls * lights[index]->color.r;
			returnColor.g = lights[index]->minAmount * lights[index]->ls * lights[index]->color.g;
			returnColor.b = lights[index]->minAmount * lights[index]->ls * lights[index]->color.b;
		}
		else
		{
			returnColor.r = lights[index]->ls * lights[index]->color.r;
			returnColor.g = lights[index]->ls * lights[index]->color.g;
			returnColor.b = lights[index]->ls * lights[index]->color.b;	
		}
		
		return returnColor;
	}
	return returnColor;
}

Vector3D_t getDirectionLights(ShadeRec_t sr,int j)
{
	Vector3D_t returnVector;
	if(lights[j]->type == POINT);
	{
		
		returnVector.x = lights[j]->location.x - sr.hitPoint.x;
		returnVector.y = lights[j]->location.y - sr.hitPoint.y;
		returnVector.z = lights[j]->location.z - sr.hitPoint.z;
		// converts the vector to a unit vector and returns the vector
		FLOAT_T length = sqrt(returnVector.x * returnVector.x + returnVector.y * returnVector.y + returnVector.z * returnVector.z);
		returnVector.x /= length; 
		returnVector.y /= length; 
		returnVector.z /= length;
		return returnVector;
	}
	if(lights[j]->type == AMBIENT)
	{
		//As ambient normal
		/*returnVector.x = 0.0;
		returnVector.y = 0.0;
		returnVector.z = 0.0;
		return returnVector;*/
		
		//As ambient occlusion
		Vector3D_t u,v,w;
		w.x = sr.normal.x;
		w.y = sr.normal.y;
		w.z = sr.normal.z;	
		//v = w cross 0.0072,1.0,00034 in case normal is vertical
		v.x = w.y * 0.00034 - w.z * 1.0;
		v.y = w.z * 0.0072 - w.x * 0.00034;
		v.z = w.x * 1.0 - w.y * 0.0072 ;
		//normalize v
		FLOAT_T mod = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); 
		v.x /= mod;
		v.y /= mod;
		v.z /= mod;
		// u = v cross w
		u.x = v.y * w.z - v.z * w.y;
		u.y = v.z * w.x - v.x * w.z;
		u.z = v.x * w.y - v.y * w.x;
		//normalize u?
		mod = sqrt(u.x * u.x + u.y * u.y + u.z * u.z); 
		u.x /= mod;
		u.y /= mod;
		u.z /= mod;
		
		
		//REGULAR SAMPLING (NOT RECOMENDED)
		//0.5 can change for any value according to the sampling technique (in this case, regular)
		/*returnVector.x = 0.5 * lights[j]->u.x + 0.5 * lights[j]->v.x + 0.5 * lights[j]->w.x;
		returnVector.y = 0.5 * lights[j]->u.y + 0.5 * lights[j]->v.y + 0.5 * lights[j]->w.y;
		returnVector.z = 0.5 * lights[j]->u.z + 0.5 * lights[j]->v.z + 0.5 * lights[j]->w.z;*/
		
		//Hammersey sampling (RECOMENDED)
		Vector3D_t a;
		int sampleIndex = getRandomSample();
		a.x = samples[sampleIndex].x;
		a.y = samples[sampleIndex].y;
		a.z = samples[sampleIndex].z;
	
		returnVector.x = a.x * lights[j]->u.x + a.y * lights[j]->v.x + a.z * lights[j]->w.x;
		returnVector.y = a.x * lights[j]->u.y + a.y * lights[j]->v.y + a.z * lights[j]->w.y;
		returnVector.z = a.x * lights[j]->u.z + a.y * lights[j]->v.z + a.z * lights[j]->w.z;
			
		FLOAT_T length = sqrt(returnVector.x * returnVector.x + returnVector.y * returnVector.y + returnVector.z * returnVector.z);
		returnVector.x /= length; 
		returnVector.y /= length; 
		returnVector.z /= length;
			
		return returnVector;	
	}
}

void updateLights()
{
	int i;
	
	for(i = 0; i < nLights ; i++)
	{
		if(lights[i]->type != AMBIENT)
		{
			lights[i]->location.x += lights[i]->dx;
			//lights[i]->location.y += dy;
			//lights[i]->location.z += dz;
			if(	lights[i]->location.x >= 100.0 || lights[i]->location.x < -50.0)
			{
				lights[i]->dx*=-1;	
			}	
		}
	}
	
	
}