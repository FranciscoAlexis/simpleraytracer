/**********************
	shade.c (for return the color form a material and a ShadeRec)
**********************/
RGBColor_t shade(ShadeRec_t sr)
{
	int j;

	Vector3D_t 	wo;
	wo.x = -sr.ray.d.x;
	wo.y = -sr.ray.d.y;
	wo.z = -sr.ray.d.z;
	
	RGBColor_t 	aux1;
	RGBColor_t 	aux2;
	RGBColor_t 	aux3;
	RGBColor_t l;
	
	/*
	if(sr.material->type == MATTE)
	{
		aux1 = lambertian_rho(sr.material->ambientBrdf);
		aux2 = L(sr,0); //0 is the index of the ambient light
		
		
		l.r = aux1.r * aux2.r;
		l.g = aux1.g * aux2.g;
		l.b = aux1.b * aux2.b;
		
		for (j = 0; j < nLights; j++) 
		{
			Vector3D_t wi = getDirectionLights(sr,j);    
			FLOAT_T ndotwi = sr.normal.x * wi.x + sr.normal.y * wi.y + sr.normal.z * wi.z;
	
			if (ndotwi > 0.0)
			{ 
				//L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
				aux1 = lambertian_f(sr.material->diffuseBrdf);
				aux2 = L(sr,j);
				l.r += aux1.r * aux2.r * ndotwi;
				l.g += aux1.g * aux2.g * ndotwi;
				l.b += aux1.b * aux2.b * ndotwi;
			}
		}
	}*/
	
	if(sr.material->type == PHONG)
	{
		aux1 = lambertian_rho(sr.material->ambientBrdf);
		aux2 = L(sr,0); //0 is the index of the ambient light
		
		l.r = aux1.r * aux2.r;
		l.g = aux1.g * aux2.g;
		l.b = aux1.b * aux2.b;
		
		for (j = 1; j < nLights; j++)
		{
			Vector3D_t wi = getDirectionLights(sr,j);    
			FLOAT_T ndotwi = sr.normal.x * wi.x + sr.normal.y * wi.y + sr.normal.z * wi.z;
	
			if (ndotwi > 0.0)
			{
				int in_Shadow = 0;
				if(lights[j]->shadows)
				{
					Ray_t shadowRay;
					shadowRay.o.x = sr.hitPoint.x;
					shadowRay.o.y = sr.hitPoint.y;
					shadowRay.o.z = sr.hitPoint.z;
					shadowRay.d.x = wi.x;
					shadowRay.d.y = wi.y;
					shadowRay.d.z = wi.z;
					if(lights[j]->type == POINT)
						in_Shadow = inShadow(shadowRay,sr,lights[j]->location);
					if(lights[j]->type == AMBIENT)
						in_Shadow = inShadowAmbient(shadowRay,sr);	
				}
				
				if(!in_Shadow)
				{
					//L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
					aux1 = lambertian_f(sr.material->diffuseBrdf);	
					aux3 = glossySpecular_f(sr, wo, wi);
					aux1.r += aux3.r;
					aux1.g += aux3.g;
					aux1.b += aux3.b;
					if(sr.material->plane)
					{
						FLOAT_T dist = sqrt(sr.localHitPoint.x * sr.localHitPoint.x);
						if(dist < 1.0)
						{
							aux1.r = 1.0;
							aux1.g = 0.0;
							aux1.b = 0.0;	
						}
						else
						{
							dist = sqrt(sr.localHitPoint.z * sr.localHitPoint.z);
							if(dist < 1.0)
							{
								aux1.r = 1.0;
								aux1.g = 1.0;
								aux1.b = 0.0;	
							}	
						}
						
					}
					aux2 = L(sr,j);
					l.r += aux1.r * aux2.r * ndotwi;
					l.g += aux1.g * aux2.g * ndotwi;
					l.b += aux1.b * aux2.b * ndotwi;	
				}
				else
				{
					
				}
			}
		}
	}
	return l;		
}
