#ifndef CSCENE_NODE_H
#define CSCENE_NODE_H


#include "Basic.h"
#include "TTriangle.h"
#include "TVector3.h"
#include "TMatrix4.h"
#include "CMesh.h"


/** \brief Class Scene Node
*
*/
class CSceneNode
	{
	public:

	enum TNodeType
		{
		EBaseNode=0,
		EMeshNode,
		ERotationNode,
		ETranslationNode,
		//last value
		EUndefined
		};
	public:
		CSceneNode()
			: //iObjectMesh(NULL)
//			, iChildren(NULL)
			 iNodeType(EBaseNode)
			{};
		CSceneNode( CMesh* aMesh )
			: iObjectMesh( aMesh )
//			, iChildren(NULL)
			, iNodeType(EMeshNode)
			{};
		~CSceneNode()
			{
			for(int i=0,j=static_cast<int>(iChildren.size());i<j;i++)
				delete iChildren.at(i);
			iChildren.clear();
			};

		TMatrix4	getLocalTransform(){ return iLocalTransform; };
		CMesh*		getMesh()const { return iObjectMesh;};
		CSceneNode*	getChild(int aIndex)const {return iChildren.at(aIndex); };
		int			getChildAmount()const {return static_cast<int>(iChildren.size()); };
		TNodeType	getNodeType()const{return iNodeType;}

		CSceneNode*	addChild(CSceneNode* aNode)
						{
						iChildren.push_back( aNode );
						return aNode;
						};
	protected:
		TMatrix4	 iLocalTransform;
		CMesh*		 iObjectMesh;
		vector<CSceneNode*> iChildren;

		TNodeType	iNodeType;
		//Later maybe
		//enable/disable children
	};

#endif

