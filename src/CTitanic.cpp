#include "CTitanic.h"


CTitanic::CTitanic() : CMesh()
	{
	this->init();
	}

// **** Deconstructor
CTitanic::~CTitanic(void)
	{
	//Use the base class destructor
	}

//Define vertices and triangles
void CTitanic::init()
	{
	this->clearMesh();

	TVector3 v0(-4,2,2);
	this->iVertices.push_back(v0);

	TVector3 v1(-3,1,0);
	this->iVertices.push_back(v1);

	TVector3 v2(-3,-1,0);
	this->iVertices.push_back(v2);

	TVector3 v3(-4,-2,2);
	this->iVertices.push_back(v3);

	TVector3 v4(3,2,2);
	this->iVertices.push_back(v4);

	TVector3 v5(3,1,0);
	this->iVertices.push_back(v5);

	TVector3 v6(3,-1,0);
	this->iVertices.push_back(v6);

	TVector3 v7(3,-2,2);
	this->iVertices.push_back(v7);

	TVector3 v8(5,0,2);
	this->iVertices.push_back(v8);

/*
	TTriangle t0(0,2,1);
	this->iTriangles.push_back(t0);


	TTriangle t1(0,3,2);
	this->iTriangles.push_back(t1);


	TTriangle t2(0,1,4);
	this->iTriangles.push_back(t2);


	TTriangle t3(1,5,4);
	this->iTriangles.push_back(t3);


	TTriangle t4(1,6,5);
	this->iTriangles.push_back(t4);


	TTriangle t5(1,2,6);
	this->iTriangles.push_back(t5);


	TTriangle t6(2,3,6);
	this->iTriangles.push_back(t6);


	TTriangle t7(3,7,6);
	this->iTriangles.push_back(t7);


	TTriangle t8(4,5,8);
	this->iTriangles.push_back(t8);


	TTriangle t9(5,6,8);
	this->iTriangles.push_back(t9);


	TTriangle t10(6,7,8);
	this->iTriangles.push_back(t10);


	TTriangle t11(0,4,3);
	this->iTriangles.push_back(t11);


	TTriangle t12(3,4,7);
	this->iTriangles.push_back(t12);


	TTriangle t13(4,8,7);
	this->iTriangles.push_back(t13);

*/

	TTriangle t0(0,1,2);
	this->iTriangles.push_back(t0);

	TTriangle t1(0,2,3);
	this->iTriangles.push_back(t1);

	TTriangle t2(0,4,1);
	this->iTriangles.push_back(t2);

	TTriangle t3(1,4,5);
	this->iTriangles.push_back(t3);

	TTriangle t4(1,5,6);
	this->iTriangles.push_back(t4);

	TTriangle t5(1,6,2);
	this->iTriangles.push_back(t5);

	TTriangle t6(2,6,3);
	this->iTriangles.push_back(t6);

	TTriangle t7(3,6,7);
	this->iTriangles.push_back(t7);

	TTriangle t8(4,8,5);
	this->iTriangles.push_back(t8);

	TTriangle t9(5,8,6);
	this->iTriangles.push_back(t9);

	TTriangle t10(6,8,7);
	this->iTriangles.push_back(t10);

	TTriangle t11(0,3,4);
	this->iTriangles.push_back(t11);

	TTriangle t12(3,7,4);
	this->iTriangles.push_back(t12);

	TTriangle t13(4,7,8);
	this->iTriangles.push_back(t13);
	}



