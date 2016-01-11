
#ifndef SH_MY_OBJECT
#define SH_MY_OBJECT
#include "../../ObjLoader/mesh.h"
#include "../../Maths/COLOR.h"

class SH_My_Object :public SH_OBJECT
{
public:
	bool initObject(Mesh mesh, COLOR diffuseMaterial);

	virtual bool DoesRayHitObject(const RAY & ray);

	AA_BOUNDING_BOX boundingBox;

	std::vector<PLANE> planeEquations;

};


#endif