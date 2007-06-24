#ifndef TRIANGLE1_H
#define TRIANGLE1_H

#include "Basic.h"

/** \brief Class that defines a triangle
*
*	This class define a triangle polygon and is used
*	in the triangles vector in the CMesh object.
*
*	The corners is orded in counter-clockwise from the front side
*
*/
class TTriangle
	{
	public:
		// Constructors
		TTriangle();
		TTriangle( int, int, int);

		// functions
		int V1() const;
		int V2() const;
		int V3() const;

		void setV1(int);
		void setV2(int);
		void setV3(int);

		void reverse();

		// Destructor
		~TTriangle(void);

	private:
		/** \brief First vertex indexes
		*/
		int iV1, iV2, iV3;
	};

//INLINE FUNCTIONS
/// method that reverses the vertex order of the triangle
inline
void TTriangle::reverse(void)
	{
	int _v = this->iV1;
	this->iV1 = iV2;
	this->iV2 = _v;
	}

#endif
