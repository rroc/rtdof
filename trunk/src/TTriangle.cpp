#include "TTriangle.h"

/** \brief Default constructor
*
*	The default constructor, this yields an invalid triangle
*	where vertex index x = -1; vertex index y = -1 and vertex index z = -1
*/
TTriangle::TTriangle(void)
	: iV1(-1), iV2(-1), iV3(-1)
	{
	// invalid triangle
	}

/** \brief Set constructor
*
*		The set constructor, the trianlge corners is orded in counter-clockwise order
*		viewed from the front side.
*	\param _v1 the first vertex index
*	\param _v2 the second vertex index
*	\param _v3 the third vertex index
*
*/
TTriangle::TTriangle(int _v1, int _v2, int _v3)
	: iV1(_v1), iV2(_v2), iV3(_v3)
	{
	}

TTriangle::~TTriangle(void)
	{
	}

// **** Public

/** \brief method to get the vertex index
*
*	Method that returns back the first vertex index
* \return integer that is the first vertex index
*
*/
int TTriangle::V1() const
	{
	return this->iV1;
	}

/** \brief method to get the vertex index
*
*	Method that returns back the second vertex index
* \return integer that is the second vertex index
*
*/
int TTriangle::V2() const
	{
	return this->iV2;
	}

/** \brief method to get the vertex index
*
*	Method that returns back the third vertex index
* \return integer that is the third vertex index
*
*/
int TTriangle::V3() const
	{
	return this->iV3;
	}


