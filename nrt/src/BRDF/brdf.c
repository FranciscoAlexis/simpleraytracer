/*******************
	brdf.c (brdf related functions)
*******************/

RGBColor_t lambertian_f(BRDFLambertian_t lamb)
{
	RGBColor_t temp;
	temp.r = lamb.cd.r * lamb.kd * invPI;
	temp.g = lamb.cd.g * lamb.kd * invPI;
	temp.b = lamb.cd.b * lamb.kd * invPI;
	return temp;
}

RGBColor_t lambertian_rho(BRDFLambertian_t lamb)
{
	RGBColor_t temp;
	temp.r = lamb.cd.r * lamb.kd;
	temp.g = lamb.cd.g * lamb.kd;
	temp.b = lamb.cd.b * lamb.kd;
	return temp;	
}

RGBColor_t glossySpecular_f(ShadeRec_t sr, Vector3D_t wo, Vector3D_t wi)
{
	RGBColor_t temp;
	temp.r = 0.0;
	temp.g = 0.0;
	temp.b = 0.0;
	
	FLOAT_T ndotwi = sr.normal.x * wi.x + sr.normal.y * wi.y + sr.normal.z * wi.z;
	Vector3D_t r;
	r.x = -wi.x + 2.0*sr.normal.x * ndotwi;
	r.y = -wi.y + 2.0*sr.normal.y * ndotwi;
	r.z = -wi.z + 2.0*sr.normal.z * ndotwi;
	
	FLOAT_T rdotwo = r.x * wo.x + r.y * wo.y + r.z * wo.z;
	FLOAT_T poweredValue;
	if(rdotwo > 0.0)
	{
		poweredValue = pow(rdotwo,sr.material->specularBrdf.exp);
		
		temp.r = sr.material->specularBrdf.ks * sr.material->specularBrdf.cd.r * poweredValue;
		temp.g = sr.material->specularBrdf.ks * sr.material->specularBrdf.cd.g * poweredValue;
		temp.b = sr.material->specularBrdf.ks * sr.material->specularBrdf.cd.b * poweredValue;
		/*
		temp.r = sr.material->specularBrdf.ks * poweredValue;
		temp.g = sr.material->specularBrdf.ks * poweredValue;
		temp.b = sr.material->specularBrdf.ks * poweredValue;
		*/
	}
	return temp;
}