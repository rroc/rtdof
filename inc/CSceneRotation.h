#ifndef CSCENE_ROTATION_H
#define CSCENE_ROTATION_H


#include "Basic.h"
#include "TVector3.h"
#include "TAngles.h"
#include "CMesh.h"


/** \brief Class Scene Node
*
*/
class CSceneRotation : public CSceneNode
	{
	public:
		CSceneRotation()
			: CSceneNode()
			,iAngles()
			{ 
			iNodeType=ERotationNode;
			};

		CSceneRotation( const TAngles& aAngles )
			: CSceneNode()
			,iAngles(aAngles)
			{
			iNodeType=ERotationNode;
			};

		CSceneRotation( const TVector3& aAxis, float aAngle )
			: CSceneNode()
			,iAngles(aAxis.iX*aAngle, aAxis.iY*aAngle, aAxis.iZ*aAngle)
			{
			iNodeType=ERotationNode;
			};

		void setAngleX(float aAngle){ iAngles.iRotX=aAngle; };
		void setAngleY(float aAngle){ iAngles.iRotY=aAngle; };
		void setAngleZ(float aAngle){ iAngles.iRotZ=aAngle; };

		void SetRotation(const TAngles& aAngles)
			{
			iAngles = aAngles;
			}
		void Rotate(const TAngles& aAngles)
			{
			iAngles += aAngles;
			}

	public:
		TAngles iAngles;
	};
#endif

