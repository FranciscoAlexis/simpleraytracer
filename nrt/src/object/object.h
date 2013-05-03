/*******************
	object.h (object related functions)
*******************/

void buildObjects();

void hitObjects(Point3D_t rayO, Vector3D_t rayD,ShadeRec_t* sr);

int hitSphere(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t* sr, Point3D_t center, FLOAT_T radius);

int inShadowAmbient(Ray_t ray, ShadeRec_t sr);

int inShadow(Ray_t ray, ShadeRec_t sr, Vector3D_t lightLocation);

int shadowHitSphere(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t sr, Point3D_t center, FLOAT_T radius);

void updateObjects();

int hitPlane(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t* sr, Point3D_t a, Normal_t n);

int shadowHitPlane(Point3D_t rayO, Vector3D_t rayD,FLOAT_T* tmin, ShadeRec_t sr, Point3D_t a, Normal_t n);