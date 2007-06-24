#ifndef CSCENE_MESH_H
#define CSCENE_MESH_H


#include "Basic.h"
#include "CMesh.h"


/** \brief Class Scene Node
*
*/
class CSceneMesh : public CSceneNode
	{
	public:
		//CONSTUCTORs
		//CSceneMesh()
		//	: iObjectMesh
		//	{
		//	iNodeType=EMeshNode;
		//	};
		CSceneMesh( CMesh** aMesh )
			: CSceneNode()
			, iObjectMesh( aMesh )
			{
			iNodeType=EMeshNode;
			};
		//PUBLIC METHODS
		CMesh*	GetMesh()const { return *iObjectMesh;};
		void	SetMesh( CMesh** aMesh )
					{ 
					_ASSERT( *aMesh != NULL );
					iObjectMesh = aMesh;
					};
	private:
		CMesh**		 iObjectMesh;
	};

#endif

