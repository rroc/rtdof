#ifndef TMESHLOADER_H
#define TMESHLOADER_H

#include "Basic.h"
#include "CMesh.h"

class TMeshLoader
	{
	public:
		TMeshLoader();
		~TMeshLoader();
		bool Load3DS( CMesh& aMesh, char* aFileName );
		//bool LoadPLY( CMesh* aMesh, char* aFileName );
	};

#endif
