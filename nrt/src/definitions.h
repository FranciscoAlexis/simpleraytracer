/*
*	definitions.h
*/

#define FLOAT_T double

const FLOAT_T 	PI 			= 3.1415926535897932384;
const FLOAT_T 	TWO_PI 		= 6.2831853071795864769;
const FLOAT_T 	PI_ON_180 	= 0.0174532925199432957;
const FLOAT_T 	invPI 		= 0.3183098861837906715;
const FLOAT_T 	invTWO_PI 	= 0.1591549430918953358;
const FLOAT_T 	kEpsilon 	= 0.0001; 
const FLOAT_T		kHugeValue	= 1.0E10;
const FLOAT_T		e 			= 1.0;
const FLOAT_T	axisVelocity = 	6.0;		
//objects
#define SPHERE 	1
#define PLANE	2

//lights
#define AMBIENT_BASIC	3
#define AMBIENT	1
#define POINT	2

//materials
#define MATTE	1
#define PHONG	2

//camea stop factor
#define CAMTRACTION 0.02
#define CAMACCELERATION 0.08
//cam max speed
#define CAMMAX 4.0
 
typedef struct 
{
	FLOAT_T x;
	FLOAT_T y;
	FLOAT_T z;
}Vector3D_t;

typedef struct 
{
	FLOAT_T r;
	FLOAT_T g;
	FLOAT_T b;	
}RGBColor_t;

typedef struct 
{
	FLOAT_T x;
	FLOAT_T y;
	FLOAT_T z;	
}Point3D_t;

typedef struct 
{
	FLOAT_T x;
	FLOAT_T y;
	FLOAT_T z;	
}Normal_t;

typedef struct 
{
	FLOAT_T m[4][4];	
}Matrix_t;

typedef struct
{
	Point3D_t 	o;
	Vector3D_t 	d;
}Ray_t;

typedef struct
{
	int 	hres; 			//horizontal image resolution
	int 	vres; 			//vertical image resolution
	FLOAT_T 	s; 				//pixel size
	int 	numSamples; 	//number of samples per pixel
	FLOAT_T 	gamma; 			//gamma correction factor
	FLOAT_T 	invGamma; 		// the inverse of gamma
	int 	showOutOfGamut; //display red if RGBColor out of gamut
}ViewPlane_t;

typedef struct
{
	int 	r;
	int 	c;
	Ray_t 	ray;
}ThreadData_t;

typedef struct
{
	Point3D_t 	eye;
	Point3D_t 	lookAt;
	Vector3D_t	up;
	Vector3D_t	u;
	Vector3D_t	v;
	Vector3D_t	w;
	FLOAT_T		rollAngle;
	FLOAT_T		exposureTime;
	FLOAT_T 	d; 		//view plane distance in pinhole camera
	FLOAT_T		zoom;	//zoom factor
	FLOAT_T		dx;
	FLOAT_T		dy;
	FLOAT_T		dz;
	FLOAT_T		ax;
	FLOAT_T		ay;
	FLOAT_T		az;
	FLOAT_T		angle;
	FLOAT_T		radius;
}Camera_t;

typedef struct 
{
	FLOAT_T 	kd;		//for diffuse
	FLOAT_T		ka;		//for ambient
	RGBColor_t 	cd;
}BRDFLambertian_t;

typedef struct 
{
	FLOAT_T 		ks;			//
	FLOAT_T		exp;		//exponent
	RGBColor_t 	cd;
}BRDFGlossySpecular_t;

typedef struct
{
	int 						type;
	BRDFLambertian_t			ambientBrdf; 	//MATTE - PONG
	BRDFLambertian_t			diffuseBrdf; 	//MATTE - PONG
	BRDFGlossySpecular_t		specularBrdf; 	//PONG
	int 						plane;
}Material_t;

typedef struct
{
	int 		type;
	RGBColor_t	color;
	Point3D_t 	center;
	Material_t*	material;
	FLOAT_T		radius;
	FLOAT_T		dx;		//For movement
	FLOAT_T		dy;
	FLOAT_T		dz;
	Point3D_t 	a; 		//For plane, point wich plane passes
	Normal_t 	n; 		//For plane, Normal
}Object_t;

typedef struct
{
	int					hitAnObject;		// did the ray hit an object?
	Material_t* 		material;			// pointer to the nearest object's material
	Point3D_t 			hitPoint;			// world coordinates of intersection
	Point3D_t			localHitPoint;		// world coordinates of hit point on untransformed object (used for texture transformations)
	Normal_t			normal;				// normal at hit point
	Ray_t				ray;				// required for specular highlights and area lights
	int					depth;				// recursion depth
	RGBColor_t			color;				// used in the Chapter 3 only
	FLOAT_T				t;					// ray parameter
	FLOAT_T				u;					// texture coordinate
	FLOAT_T				v;					// texture coordinate
}ShadeRec_t;

typedef struct
{
	int				type;					
	int 			shadows;				//as boolean
	FLOAT_T 			ls;
	RGBColor_t		color;
	Vector3D_t		location;
	Vector3D_t		u;						//ambient occlusion
	Vector3D_t		v;						//ambient occlusion
	Vector3D_t		w;						//ambient occlusion
	FLOAT_T			minAmount;				//ambient occlusion
	FLOAT_T			dx;
	FLOAT_T			dy;
	FLOAT_T			dz;
}Light_t;

