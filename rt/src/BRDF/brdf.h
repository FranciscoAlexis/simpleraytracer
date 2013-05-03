/*******************
	brdf.h (brdf related functions)
*******************/

RGBColor_t lambertian_f(BRDFLambertian_t lamb);
RGBColor_t lambertian_rho(BRDFLambertian_t lamb);
RGBColor_t glossySpecular_f(ShadeRec_t sr, Vector3D_t wo, Vector3D_t wi);