#ifndef TITANIC1_H
#define TITANIC1_H

#include "Basic.h"
#include "TTriangle.h"
#include "TVector3.h"
#include "TMatrix4.h"
#include "CMesh.h"

/** \brief Class that defines a "boat"
*
* Class that define an boat-like-shape
* data is entered into
*	- vertices
*	- triangles
*	- faceVisibility (true)
*
* These are inherited  from the CMesh object.
*
* This method is best used (if you dont have implemented you own long object) when
* calculate perspective.
*
*
*
*/
class CTitanic : public CMesh
	{
	public:
		CTitanic();
		CTitanic(float aScale);
		~CTitanic(void);

	private:
		void init(float aScale);
	};

#endif



