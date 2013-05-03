/****************************
	light.h (light related functions)
****************************/

void buildLights();

RGBColor_t L(ShadeRec_t sr, int index);

Vector3D_t getDirectionLights(ShadeRec_t sr,int j);  

void updateLights();  