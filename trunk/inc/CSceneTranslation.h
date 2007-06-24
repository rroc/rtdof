#ifndef CSCENE_TRANSLATION_H
#define CSCENE_TRANSLATION_H


#include "Basic.h"
#include "TTriangle.h"
#include "TVector3.h"
#include "TMatrix4.h"
#include "CMesh.h"


/** \brief Class Scene Node
*
*/
class CSceneTranslation : public CSceneNode
	{
	public:
		CSceneTranslation()
			: CSceneNode()
			{ iNodeType=ETranslationNode;iLocalTransform.loadIdentity(); };
		CSceneTranslation( TVector3 aVector )
			: CSceneNode()
			, iX(aVector.iX)
			, iY(aVector.iY)
			, iZ(aVector.iZ)
			{iNodeType=ETranslationNode;/*update();*/};
		void setX(float aX){ iX=aX; /*update();*/ };
		void setY(float aY){ iY=aY; /*update();*/ };
		void setZ(float aZ){ iZ=aZ; /*update();*/ };
	private:
//		void update(){ iLocalTransform.makeTranslate( iX, iY, iZ); };

	public:
		float iX, iY, iZ;
	};

#endif

