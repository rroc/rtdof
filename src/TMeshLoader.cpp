
#include "TMeshLoader.h"
using namespace std;

/*using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::ios;
*/
#include <fstream>
//using std::istream;
//using std::fstream;

//CONSTANTS
const int KMaxObjNameLength = 20;

//CLASS DEFINITION
TMeshLoader::TMeshLoader()
	{
	}

TMeshLoader::~TMeshLoader()
	{
	}

bool TMeshLoader::Load3DS( CMesh& aMesh, char* aFileName, float aScale )
	{
	fstream dataFile( aFileName, ios::in | ios::binary );
	// exit program if fstream cannot open file
   if ( !dataFile )
		{
		//cerr << "File: \"" << aFileName << "\" could not be opened." << endl;
		std::cout << "File: \"" << aFileName << "\" could not be opened." << endl;
		return( false );
		}

	unsigned short dataId(0);
	unsigned int dataLength(0);

	int dataIdSize = sizeof(dataId);
	int dataLengthSize = sizeof(dataLength);
	int i(0);

	aMesh.clearMesh();

	std::cout << "Reading: " << aFileName << "..." << endl;
	while( !dataFile.eof() )
		{
		//DATA ID
		dataFile.read( reinterpret_cast<char *>(&dataId), dataIdSize );
		//DATA SIZE
		dataFile.read( reinterpret_cast<char *>(&dataLength), dataLengthSize );
		//std::cout << "ID: " << dataId << " LENGTH: " << dataLength << endl;

		//PARSE FILE
		switch (dataId)
			{
			//----------- MAIN3DS -----------
			// Description: Main block, contains all the other blocks
			// ID: 4d4d
			// Length: 0 + subblocks
			//-------------------------------
			case 0x4d4d:
				{
				}
			break;

			//----------- EDIT3DS -----------
			// Description: 3D Editor block, objects layout info
			// ID: 3d3d (hex)
			// Length: 0 + sub blocks
			//-------------------------------
			case 0x3d3d:
				{
				}
			break;

			//----------- EDIT_OBJECT -----------
			// Description: Object block, info for each object
			// ID: 4000 (hex)
			// Length: len(object name) + sub blocks
			//-----------------------------------
			case 0x4000:
				{
				char objectname[KMaxObjNameLength];
				//READ OBJECT NAME
				i=0; char a('a');

				while(i<KMaxObjNameLength && a!='\0')
					{
					//DATA ID
					dataFile.read( reinterpret_cast<char *>(&a), sizeof(char) );
					objectname[i] = a;
					i++;
					}
				cout << "NAME: " << objectname << endl;
				}
			break;

			//----------- OBJ_TRIMESH -----------
			// Description: Triangular mesh, contains blocks for 3d mesh info
			// ID: 4100 (hex)
			// Length: 0 + sub blocks
			//-----------------------------------
			case 0x4100:
				{
				}
			break;

			//----------- TRI_VERTEXL -----------
			// Description: Vertices list
			// ID: 4110 (hex)
			// Length: 1 x unsigned short (number of vertices)
			//             + 3 x float (vertex coordinates) x (number of vertices)
			//             + sub blocks
			//-----------------------------------
			case 0x4110:
				{
				//READ THE AMOUNT OF VERTICES
				unsigned short numberOfVertices(0);
				dataFile.read( reinterpret_cast<char *>(&numberOfVertices), sizeof (unsigned short) );
				cout << "VERTICES: " << numberOfVertices << endl;

				//READ VERTICES INTO MESH
				TVector3 vertex;
				float x,y,z;

                for (i=0; i<numberOfVertices; i++)
					{
					dataFile.read( reinterpret_cast<char *>(&x), sizeof (float) );
					dataFile.read( reinterpret_cast<char *>(&y), sizeof (float) );
					dataFile.read( reinterpret_cast<char *>(&z), sizeof (float) );
					vertex.set( x*aScale, y*aScale, z*aScale );
					aMesh.iVertices.push_back(vertex);
					}
				}
				break;

			//----------- TRI_FACEL1 -----------
			// Description: Face list
			// ID: 4120 (hex)
			// Length: 1 x unsigned short (number of polygons)
			//             + 3 x unsigned short (polygon points) x (number of polygons)
			//             + sub blocks
			//----------------------------------
			case 0x4120:
				{
				//READ THE AMOUNT OF FACEs
				unsigned short numberOfTriangles(0);
				dataFile.read( reinterpret_cast<char *>(&numberOfTriangles), sizeof (unsigned short) );
				cout << "TRIANGLES: " << numberOfTriangles << endl;

				//READ TRIANGLES INTO MESH
				unsigned short v1, v2, v3, flags;

                for (i=0; i<numberOfTriangles; i++)
					{
					dataFile.read( reinterpret_cast<char *>(&v1), sizeof (unsigned short) );
					dataFile.read( reinterpret_cast<char *>(&v2), sizeof (unsigned short) );
					dataFile.read( reinterpret_cast<char *>(&v3), sizeof (unsigned short) );

					dataFile.read( reinterpret_cast<char *>(&flags), sizeof (unsigned short) );
					TTriangle triangle( v1, v2, v3 );
					aMesh.iTriangles.push_back(triangle);
					}
				}
                break;

			//----------- TRI_MAPPINGCOORS -----------
			// Description: Vertices list
			// ID: 4140 (hex)
			// Length: 1 x unsigned short (number of mapping points)
			//             + 2 x float (mapping coordinates) x (number of mapping points)
			//             + sub blocks
			case 0x4140:
				{
				//TEXTURE MAPPING COORDINATES
				unsigned short numberOfTextureCoordinates(0);
				dataFile.read( reinterpret_cast<char *>(&numberOfTextureCoordinates), sizeof (unsigned short) );
				cout << "TEXTURECOORDs: " << numberOfTextureCoordinates << endl;

				//READ TEXTUREMAP
				float u,v;
				for (i=0; i<numberOfTextureCoordinates; i++)
					{
					dataFile.read( reinterpret_cast<char *>(&u), sizeof(float) );
					dataFile.read( reinterpret_cast<char *>(&v), sizeof(float) );
					
					//aMesh.iTextureCoords.push_back(TVector3(u*aScale,v*aScale,0));
					}
				}
                break;
/*
			//LOCAL AXIS
			case 0x4160:
				{
				}
				break;
			//LIGHT
			case 0x4600:
				{
				}
				break;
			//CAMERA
			case 0x4700:
				{
				}
				break;

*/
			//----------- UNKNOWN BLOCKS -----------
			// Skip the rest...
			default:
				dataFile.seekg( dataLength - (dataIdSize+dataLengthSize), ios::cur );
			}
		}
	dataFile.close();
	std::cout << "Read OK." << endl << endl;
	return true;
	}

//bool TMeshLoader::LoadPLY( CMesh* aMesh, char* aFileName )
//	{
//	return false;
//	}

