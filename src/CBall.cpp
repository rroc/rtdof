/** \brief CBall
*
* CBall is a simple ball calculated using trigonometric functions sin and cos
*
*/

//INCLUDES
#include "CBall.h"
#ifndef USE_DEGREES
#include "CTrigTable.h"
#else
	#undef USE_SIN_TABLE
#endif

//CONSTANTS
const int KVerticesPerRow( 16 );

//Constructor
CBall::CBall()
	{
	this->init( KVerticesPerRow,1 );
	}

CBall::CBall( int aRowSize )
	{
	this->init( aRowSize, 1 );
	}

CBall::CBall( int aRowSize, float aScale )
	{
	this->init( aRowSize, aScale );
	}

// **** Destructor
CBall::~CBall()
	{
	//Use the base class destructor
	}

//Create the vertices and triangles
void CBall::init( int aRowSize, float aScale )
	{
	this->clearMesh();

	iRowSize=aRowSize;

	float gridSize = pi2/aRowSize;

	//CALCULATE VERTICES
	TVector3 vertex;
	float i(0.0),j(0.0),k( pi2-(gridSize/2) ),x(0),y,z,size(0.0);

	// calculate the position for the circles (x and y coords)
	//     ___
	//    / : \
	// ..|..:..|...
	//   |  :  |
	//    \___/
	for(i=0; i < gridSize+k/2; i+=gridSize )
		{
#ifdef USE_SIN_TABLE
		x    = CTrigTable::iTrigTable->CosR( i );
		size = CTrigTable::iTrigTable->SinR( i );
#else
		//calculate the density of x coordinates
		x = cos(i);
		//calculate the size of the circle on this level (ie. ~ycoordinates)
		size = sin(i);
#endif
		//create a circle of a size needed (across y and z axis )
		//    .
		//   .^.
		// ..|:|....
		//   |:|
		//   '-'
		for(j=0.0; j < k; j+=gridSize )
			{
#ifdef USE_SIN_TABLE
			y = CTrigTable::iTrigTable->SinR( j ) * size;
			z = CTrigTable::iTrigTable->CosR( j ) * size;
#else
			y = sin(j) * size;
			z = cos(j) * size;
#endif
			vertex.set( x*aScale, y*aScale, z*aScale );
			this->iVertices.push_back(vertex);
			}
		}

	//CONSTRUCT TRIANGLES
	int v1,v2,v3,v4,vertexIndex;

	for(vertexIndex=0, j=(this->iVertices.size()-1); vertexIndex < j; vertexIndex++)
		{
		//CHOOSE THE VERTICES TO BE CONNECTED
		//   2*......*3
		//    :    ,´:
		//    :  ,´  :
		//    :,´    :
		//   1*......*4
		v1 = vertexIndex;
		v2 = vertexIndex+ aRowSize;
		v3 = vertexIndex+ aRowSize+ 1;
		v4 = vertexIndex+ 1;

		//Patch the last vertices of the row to the first ones
		//  0--.--.--.--.--.--.--.--*
		//  |,/|,/|,/|,/|,/|,/|,/|,/ ->
		//  0--'--'--'--'--'--'--'--*
		if( 0== (vertexIndex+1)%aRowSize )
			{
			v3 -= (aRowSize);
			v4 -= (aRowSize);
			}

		//The end points should have only 1 point
		//ball start
		if( vertexIndex < aRowSize)
			{
			v1=0;
			v4=0;
			}
		//ball end
		else if( j<(vertexIndex+aRowSize*2) )
			{
			v2 = static_cast<int>(j);
			v3 = static_cast<int>(j);
			}

		//CREATE THE UPPER TRIANGLE
		if( j != v2 ) // <-(if not the end of ball)
			{
			//   2*-----*3
			//    |    /:
			//    |  /  :
			//    |/    :
			//   1*.....*4
			// counter-clockwise:
			TTriangle triangle(   v1
								, v3
								, v2
								);
			this->iTriangles.push_back(triangle);
			}
		//CREATE THE LOWER TRIANGLE
		if( 0 != v1 ) // <-(if not the start of ball)
			{
			//   2*.....*3
			//    :    /|
			//    :  /  |
			//    :/    |
			//   1*-----*4
			// counter-clockwise:
			TTriangle triangle2(   v1
								, v4
								, v3
								);


			this->iTriangles.push_back( triangle2 );
			}
		}
	}

/** \brief Method that calculate random colored stripes
*/
void CBall::randomColors()
	{
	iFaceColors.clear();
	srand( clock() );
	TColorRGB a;
	a.randomColors();
	for(int triangleIndex=0, j=static_cast<int>(this->iTriangles.size()); triangleIndex< j; triangleIndex++)
		{
		if(0.0 == (triangleIndex + iRowSize) % (iRowSize*2) )
			a.randomColors();
		this->iFaceColors.push_back( a );
		}
	}

//FIX IF WE CHANGED IT!
#ifdef USE_DEGREES
	#undef USE_SIN_TABLE
#endif


