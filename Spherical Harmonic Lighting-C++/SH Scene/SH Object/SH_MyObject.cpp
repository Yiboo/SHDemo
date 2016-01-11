#include <vector>
#include "../../GL files/glee.h"
#include "../../Maths/Maths.h"
#include "../../Log/LOG.h"
#include "../../Bounding Volumes/Bounding Volumes.h"
#include "../../Ray/RAY.h"
#include "../SH_SCENE.h"
#include "SH_MyObject.h"



bool SH_My_Object::initObject(Mesh mesh, COLOR diffuseMaterial)
{
	int i;
	for(i=0; i<mesh.positions.size(); ++i){
		SH_VERTEX vertex;
		vertex.position.Set(mesh.positions[i].x, mesh.positions[i].y, mesh.positions[i].z);
		vertex.normal.Set(mesh.normals[i].x, mesh.normals[i].y, mesh.normals[i].z);
		vertex.diffuseMaterial = diffuseMaterial;
		vertices.push_back(vertex);
	}

	for(i=0; i<mesh.indices.size(); ++i){
		indices.push_back(mesh.indices[i]);
	}

	name = mesh.name;
	
	VECTOR3D minVec = vertices[0].position;
	VECTOR3D maxVec = vertices[0].position;

	for(std::size_t i=1; i<vertices.size(); ++i){
		if(vertices[i].position.x < minVec.x)
			minVec.x = vertices[i].position.x;
		if(vertices[i].position.y < minVec.y)
			minVec.y = vertices[i].position.y;
		if(vertices[i].position.z < minVec.z)
			minVec.z = vertices[i].position.z;

		if(vertices[i].position.x > maxVec.x)
			maxVec.x = vertices[i].position.x;
		if(vertices[i].position.y > maxVec.y)
			maxVec.y = vertices[i].position.y;
		if(vertices[i].position.z > maxVec.z)
			maxVec.z = vertices[i].position.z;
	}

	boundingBox.SetFromMinsMaxes(minVec, maxVec);

	//Calculate the plane equation for each triangle
	for(std::size_t i=0; i<indices.size()/3; ++i)
	{
		//Get the 3 vertex positions for this triangle
		VECTOR3D v[3];
		v[0]=vertices[indices[i*3  ]].position;
		v[1]=vertices[indices[i*3+1]].position;
		v[2]=vertices[indices[i*3+2]].position;

		PLANE planeEquation;
		planeEquation.SetFromPoints(v[0], v[1], v[2]);

		planeEquations.push_back(planeEquation);
	}

	return true;
}


bool SH_My_Object::DoesRayHitObject(const RAY & ray)
{
	if(!ray.IntersectsAABB(boundingBox, NULL))
		return false;

	for(int i=0; i<(int)(indices.size()/3); ++i)
	{
		VECTOR3D v[3];
		v[0] = vertices[indices[i*3]].position;
		v[1] = vertices[indices[i*3+1]].position;
		v[2] = vertices[indices[i*3+2]].position;

		if(ray.IntersectsTriangle(planeEquations[i], v, NULL))
			return true;
	}

	return false;
}
