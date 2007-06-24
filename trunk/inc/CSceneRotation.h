#ifndef CSCENE_ROTATION_H
#define CSCENE_ROTATION_H


#include "Basic.h"
#include "TTriangle.h"
#include "TVector3.h"
#include "TMatrix4.h"
#include "CMesh.h"


/** \brief Class Scene Node
*
*/
class CSceneRotation : public CSceneNode
	{
	public:
		CSceneRotation()
			: CSceneNode()
			, iAxis(TVector3(0,0,0))
			, iAngle(0.0f)
			{ iNodeType=ERotationNode;iLocalTransform.loadIdentity();};
		CSceneRotation( const TVector3& aAxis, float aAngle )
			: CSceneNode()
			, iAxis(aAxis)
			, iAngle(aAngle)
			{iNodeType=ERotationNode;/*update();*/};
		void setAngle(float aAngle){ iAngle=aAngle; /*update();*/ };
		void setAxis(const TVector3& aAxis){ iAxis=aAxis; iAxis.normalize(); /*update();*/ };
		void setRotation(const TVector3& aAxis, float aAngle){iAxis=aAxis;iAxis.normalize();iAngle=aAngle;/*update();*/}
		void Rotate(const TVector3& aAxis, float aAngle){iAxis+=aAxis;iAxis.normalize();iAngle+=aAngle;}
//	private:
//		void update(){ iLocalTransform.makeRotateAxisAngle( iAxis, iAngle ); };
	public:
		TVector3 iAxis;
		float    iAngle;
	};

#endif

