//INCLUDES
#include "CMyRenderer.h"

//USE THIS FILE


#include "TAngles.h"
#include "TMeshLoader.h"

#ifdef USE_SIN_TABLE
#include "CTrigTable.h"
#endif

//CONSTANTS
const int KScreenWidth = 480; ///< Default screen width  ( = PSP's screen width)
const int KScreenHeight= 272; ///< Default screen height ( = PSP's screen height)
const float KZNear	= 10.0f;
const float KZFar	= 40.0f;

//INIT STATIC DATA
CMyRenderer* CMyRenderer::iCurrentRenderer = 0;

//CONSTANTS
const float KXAngleModStart = 0.0f; //0.003f;
const float KYAngleModStart = 0.0f; //-0.003f;
const float KZAngleModStart = 0.0f;

const float KScaleStart = 0.5f;

const float KPointOfRotX = 0.0f;
const float KPointOfRotY = 0.0f;
const float KPointOfRotZ = 2.0f;


//CONSTRUCTORS
//
//Default constructor
CMyRenderer::CMyRenderer()
	: iScreenWidth( KScreenWidth )
	, iScreenHeight( KScreenHeight )
	, iViewVector(0,0,1)
	, iScale( KScaleStart )
	, iCurrentAngles(0,0,0)
	, iAnglesChange(KXAngleModStart, KYAngleModStart, KZAngleModStart)
	, iFrame(0)
	, iPreviousTime(0)
	, iAxisDrawn( false )
	, iNormalsDrawn( false )
	, iUseVertexNormals( false )
	, iPointOfRotation( KPointOfRotX, KPointOfRotY, KPointOfRotZ )
	, iTransformInProcess( false )
	, iRenderType(ERenderFaceShadedCulled) //ERenderVertices)
	, iAmountOfTriangles(1)
	, iViewDistance(2)
	{
#ifdef USE_SIN_TABLE
	CTrigTable* trigtable = new CTrigTable();
	CTrigTable::iTrigTable = trigtable;
#endif

	iPixelBuffer1 = new float[iScreenWidth*iScreenHeight*4];
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];
	iDepthBuffer  = new float[iScreenWidth*iScreenHeight];
	}

//Constructor wiht the screen size defined
CMyRenderer::CMyRenderer( const int aWidth, const int aHeight )
	: iScreenWidth( aWidth )
	, iScreenHeight( aHeight )
	, iViewVector(0,0,1)
	, iScale( KScaleStart )
	, iCurrentAngles(0,0,0)
	, iAnglesChange( KYAngleModStart, KYAngleModStart, KZAngleModStart)
	, iFrame(0)
	, iPreviousTime(0)
	, iAxisDrawn( false )
	, iNormalsDrawn( false )
	, iUseVertexNormals( true/*false*/ )
	, iPointOfRotation( KPointOfRotX, KPointOfRotY, KPointOfRotZ )
	, iTransformInProcess( false )
	, iRenderType(ERenderFaceShadedCulled) //ERenderVertices)
	, iAmountOfTriangles(1)
	, iViewDistance(2)
	{
#ifdef USE_SIN_TABLE
	CTrigTable* trigtable = new CTrigTable();
	CTrigTable::iTrigTable = trigtable;
#endif

	iPixelBuffer1 = new float[iScreenWidth*iScreenHeight*4];
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];
	iDepthBuffer  = new float[iScreenWidth*iScreenHeight];
	}

//DESTRUCTOR releases the used heap
CMyRenderer::~CMyRenderer()
	{
	if(iLights.size()>0)
		{
		for(int i=0,j=static_cast<int>(iLights.size());i<j; i++)
			{
			delete iLights.at(i);
			}
		}
	if( iMesh!=0 )
		delete iMesh;

	if( iPixelBuffer1!=0 )
		delete [] iPixelBuffer1;
	if( iPixelBuffer2!=0 )
		delete [] iPixelBuffer2;
	if( iDepthBuffer!=0 )
		delete [] iDepthBuffer;

#ifdef USE_SIN_TABLE
	delete CTrigTable::iTrigTable;
#endif
	}

/** \brief Method that creates a mesh
*
*   Creates an instance of a desired mesh type
*/
void CMyRenderer::CreateScene()
	{
	iScene = new CSceneNode();


	//CMesh* meshObject = new CMesh();
	//TMeshLoader* loader = new TMeshLoader();
	//loader->Load3DS(*meshObject, "3ds/icebear.3ds" );

//	CMesh* meshObject  = new CIcosahedron(0.9);
	CMesh* meshObject  = new CBall(16, 1.3);
//	CMesh* meshObject  = new CTitanic();
	//meshObject->calculateFaceNormals();
	meshObject->calculateVertexNormals();
	meshObject->randomColors();

	TVector3 transInit = TVector3(-2, -2, -8 );

	//TVector3 transPerObject = TVector3(0.7, 0.7, -0.7);
	TVector3 transPerObject = TVector3(0.1, 0.1, -5.0f);
	TVector3 rotXY =  TVector3(-1, 0, 0 );
	float rotAngle = 40;

	TVector3 rotXY2 =  TVector3(0, 1, 0 );
	float rotAngle2 = -35;


	CSceneNode* mNodes[5];
	CSceneTranslation* tNodes[5];
	//CSceneRotation* rNodes[5];
	mNodes[0] = new CSceneNode( meshObject );

	iRootRot = new CSceneRotation( rotXY, rotAngle );
	CSceneRotation* rootRot2 = new CSceneRotation( rotXY2, rotAngle2 );




	CSceneNode* currentNode = iScene;
	//R*T
	currentNode = currentNode->addChild( new CSceneTranslation( transInit ) );

	currentNode = currentNode->addChild( iRootRot );
	currentNode = currentNode->addChild( rootRot2 );

	currentNode = currentNode->addChild( mNodes[0] );


	for(int i=1; i<7; i++)
		{
		mNodes[i] = new CSceneNode( meshObject );
		tNodes[i] = new CSceneTranslation( transPerObject );
		//rNodes[i] = new CSceneRotation( rotXY, rotAngle );
		iSceneRotations.push_back(new CSceneRotation( rotXY, rotAngle ));

		currentNode = currentNode->addChild( tNodes[i] );
		//currentNode = currentNode->addChild( iSceneRotations.at( iSceneRotations.size()-1) );//rNodes[i] ); //rNodes[i];
		currentNode = currentNode->addChild( mNodes[i] );
		}

	}


/** \brief Method that calculates iFrames per second
*
*	Method that uses glut to calculate iFrames per second. It update the iFpsCountString every  1000 calculation.
*	This method is to uncertain to do any scientific conclusiions but give you a chanse to evaluate the
*	speed of you code.<br>
*	The more math you implement the slower the iFrame rate should be, but not to slowe.
*
*/
void CMyRenderer::FramesPerSec()
	{
	iFrame++;
	iCurrentTime = glutGet(GLUT_ELAPSED_TIME);

	if ( iCurrentTime - iPreviousTime > 1000 )
		{
		//secure sprintf_s: (required by visualstudio 2005)
		sprintf_s(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));
		//sprintf(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));

		iPreviousTime = iCurrentTime;
		iFrame = 0;
		}

	}

/** \brief Method that draws text to the glut window
*
*	This method draws text to the glut window at the postion x = -0.85 and y = -0.95
*	If you want to print you own text, you might want to change the method to something like
*	DrawText(int x, int y, cons char *string)
*
*/
void CMyRenderer::DrawText() const
	{
	//float x = //-0.85;
	//float y = //-0.95;
	float x = 5.0f, y= iScreenHeight-20.0f;

	glPushMatrix();

	//configure the transforms for 2D
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0.0f, iScreenWidth, 0.0f, iScreenHeight);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

	glColor3f(1,0,0);
	glRasterPos2f(x, y);
	int len = (int) strlen(iFpsCountString);

	for (int i = 0; i < len; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, iFpsCountString[i]);
		}
	glFlush();
	//glutSwapBuffers();

	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	}


// ********** GL DRAWING METHODs *******


/** \brief Draw a triangle using openGL
*
*	This method draw a triangle using openGL, given three vertices, v1,v2,v3 and
*	the colors that the face shall have. The r, g, b value has to be between
*	0 and 1, where 0 = no intensity, and 1 = full intensity
*
*	The triangle is drawn counter clockwise.
*
*	You dont have to change or understand the openGL or this method
*
*/
void CMyRenderer::DrawTriangle(TVector3 aVx[], TVector3 aNv[], TColorRGB aCol[]) const
	{
	//FACE COLORING
	//glColor3f( aCol[0].getR(), aCol[0].getG(), aCol[0].getB());
	GLfloat mat_diffuse[] = { aCol[0].getR(), aCol[0].getG(), aCol[0].getB() };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glBegin(GL_TRIANGLES);
		glNormal3f(aNv[0].iX, aNv[0].iY, aNv[0].iZ);
		glVertex3f(aVx[0].iX, aVx[0].iY, aVx[0].iZ);

		glNormal3f(aNv[1].iX, aNv[1].iY, aNv[1].iZ);
 		glVertex3f(aVx[1].iX, aVx[1].iY, aVx[1].iZ);

		glNormal3f(aNv[2].iX, aNv[2].iY, aNv[2].iZ);
 		glVertex3f(aVx[2].iX, aVx[2].iY, aVx[2].iZ);
	glEnd();
	}

void CMyRenderer::DrawTriangle(const TVector3& v1, const TVector3& v2, const TVector3& v3, float r, float g, float b) const
	{
	glColor3f(r,g,b);
	glBegin(GL_TRIANGLES);
		glVertex3f(v1.iX, v1.iY, v1.iZ);
 		glVertex3f(v2.iX, v2.iY, v2.iZ);
 		glVertex3f(v3.iX, v3.iY, v3.iZ);
	glEnd();
	}


void CMyRenderer::DrawTriangle(const TVector3& v1, const TVector3& v2, const TVector3& v3, const TColorRGB& c) const
	{
	DrawTriangle( v1, v2, v3, c.getR(), c.getG(), c.getB() );
	}

void CMyRenderer::DrawTriangle( const TVector3& v1, const TVector3& v2, const TVector3& v3, const TColorRGB& c1, const TColorRGB& c2, const TColorRGB& c3 ) const
	{
	glBegin(GL_TRIANGLES);
		glColor3f(c1.getR(), c1.getG(), c1.getB());
		glVertex3f(v1.iX, v1.iY, v1.iZ);
		glColor3f(c2.getR(), c2.getG(), c2.getB());
 		glVertex3f(v2.iX, v2.iY, v2.iZ);
		glColor3f(c3.getR(), c3.getG(), c3.getB());
 		glVertex3f(v3.iX, v3.iY, v3.iZ);
	glEnd();
	}

/** \brief Draw a line using openGL
*
*	This method draw a line using openGL, given three vertices, v1,v2,v3 and
*	the colors that the line shall be drawn in. The r, g, b value has to be between
*	0 and 1, where 0 = no intensity, and 1 = full intensity
*
*	The method dont use any z information, and the triangle is drawn in
*	 v1 -> v2 <br>
*	 v2 -> v3 <br>
*	 v3 -> v1 order
*
*	You dont have to change or understand the openGL or this method
*
*/
void CMyRenderer::DrawWireTriangle(const TVector3& v1, const TVector3& v2, const TVector3& v3, float r, float g, float b) const
	{
	glColor3f(r,g,b);
	glLineWidth(1.2);
	glBegin(GL_LINE_LOOP);
		glVertex3f( v1.iX, v1.iY, v1.iZ );
		glVertex3f( v2.iX, v2.iY, v2.iZ );
		glVertex3f( v3.iX, v3.iY, v3.iZ );
	glEnd();
	}

void CMyRenderer::DrawWireTriangle(const TVector3& v1, const TVector3& v2, const TVector3& v3, const TColorRGB& c) const
	{
	DrawWireTriangle( v1, v2, v3, c.getR(), c.getG(), c.getB() );
	}


/** \brief Draw a line using openGL
*
*	This method draw a line using openGL, given two vectors and the color
*
*/
void CMyRenderer::DrawLine(const TVector3& v1, const TVector3& v2, float r, float g, float b) const
	{
	glColor3f(r,g,b);
	glBegin(GL_LINES);
		glVertex3f( v1.iX, v1.iY, v1.iZ );
		glVertex3f( v2.iX, v2.iY, v2.iZ );
	glEnd();
	}

void CMyRenderer::DrawLine(const TVector3& v1, const TVector3& v2, const TColorRGB& c) const
	{
	DrawLine( v1, v2, c.getR(), c.getG(), c.getB() );
	}


/** \brief Draw axis at defined location
*
*/
void CMyRenderer::DrawAxisMark( float aX, float aY, float aZ, float aSize) const
	{
	//draw origo
	TVector3 origoX1( aX-aSize,	aY,			aZ		 );
	TVector3 origoX2( aX+aSize,	aY,			aZ		 );

	TVector3 origoY1( aX,		aY-aSize,	aZ		 );
	TVector3 origoY2( aX,		aY+aSize,	aZ		 );

	TVector3 origoZ1( aX,		aY,			aZ-aSize );
	TVector3 origoZ2( aX,		aY,			aZ+aSize );

	DrawLine( origoX1, origoX2, 1,0,0);
	DrawLine( origoY1, origoY2, 0,1,0);
	DrawLine( origoZ1, origoZ2, 0,0,1);
	}

/** \brief Draw a normal for a face in a mesh
*
*/
void CMyRenderer::DrawFaceNormal( TVector3 vx[], TVector3 nv) const
	{
	//Center of the triangle
	TVector3 vv1 = (
					(
					vx[0] +
					vx[1] +
					vx[2]
					)/3
					);
	//Normal vectors vertex coordinates
	glDisable(GL_DEPTH_TEST);
	DrawLine( vv1, (-nv)*0.2+vv1, 1, 0.7, 0);
	glEnable(GL_DEPTH_TEST);
	}

/** \brief Draw a normal for a face in a mesh
*
*/
void CMyRenderer::DrawVertexNormal( TVector3 vx[], TVector3 nv[]) const
	{
	//Normal vectors vertex coordinates
	glDisable(GL_DEPTH_TEST);
	DrawLine( vx[0], (-nv[0])*0.2+vx[0], 0.7, 1, 0);
	DrawLine( vx[1], (-nv[1])*0.2+vx[1], 0.7, 1, 0);
	DrawLine( vx[2], (-nv[2])*0.2+vx[2], 0.7, 1, 0);
	glEnable(GL_DEPTH_TEST);
	}

// *************	RENDERING METHODS *********** /

/** \brief Method that specify with method should be used to render the scene
*
*	First the functions Calculations() is called to perform all you
*	operations on your objects, dont do any operations on you object in
*	this method, keep them all in Calculations().
*
*	You can choose between 5 diffrent drawing methods
*
*	RenderWire()<br>
*	This metod draw the wire iFrame of you object, it will "connect" the x and y variables in your CMesh::vertices
*
*	RenderWireCulled()<br>
*	This method is the same as the RenderWire() before you implement the culling
*
*	RenderFacedCulledSimpleShaded()<br>
*	This method draw triangles from you vertex data from the CMesh::vertices, only the x and y coordinats is used.
*	This method also uses the CMesh::iFaceNormals to calculate shading. If you want to use this one you have to
*	implement the culling inte method, and you also have to implemented the CMesh::calculateFaceNormals
*
*	renderFacedCulledColors()<br>
*	This method draw triangles, you have to implement the culling functions. It uses the CMesh::iFaceColors to
*	color the triangle, if not it wont draw anything and halt in an error.
*
*	RenderFaceShadedCulled()<br>
*	This method also draw triangles, and you have to implement the culling functions. It uses the CMesh::faceLighting to
*	color the triangle.
*
*/
void CMyRenderer::RenderScene()
	{
	//Clear screen amd depth buffer
	glClear( GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );
	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer2 );

	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();

	gluPerspective( 45.0f, (GLfloat)iScreenWidth / (GLfloat)iScreenHeight, KZNear, KZFar);
	//gluPerspective( zoom, (float)width/(float)height, distance/10.0f, distance*2.0f);

	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	iRootRot->Rotate( TVector3(1,0,0), iAnglesChange.iRotX );
	iRootRot->Rotate( TVector3(0,1,0), iAnglesChange.iRotY );
	iRootRot->Rotate( TVector3(0,0,1), iAnglesChange.iRotZ );

	//for(int i=0,j=iSceneRotations.size();i<j;i++)
	//	{
	//	iSceneRotations.at(i)->Rotate( TVector3(1,0,0), iAnglesChange.iRotX );
	//	iSceneRotations.at(i)->Rotate( TVector3(0,1,0), iAnglesChange.iRotY );
	//	iSceneRotations.at(i)->Rotate( TVector3(0,0,1), iAnglesChange.iRotZ );
	//	}

	//sort( iMeshes.begin(), iMeshes.end(), SortByObject );

//	glLoadIdentity(); // Initialize the model matrix as identity
//	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW); // Modeling transformation
    glLoadIdentity(); // Initialize the model matrix as identity
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glTranslatef(0.0,0.0,-300); // We move the object forward (the model matrix is multiplied by the translation matrix)

	//Recursively draws the whole scene
	DrawSceneNode( iScene );
//	glutSolidSphere (1.0, 20, 16);
//	glutWireCube (1.0);

	if(iAxisDrawn) DrawAxisMark( 0, 0, 0, 0.1 );



	glFlush(); // This force the execution of OpenGL commands

	SimulateDOF();

#ifdef USE_FPS_LIMIT
	//framerate limiter
	while(glutGet(GLUT_ELAPSED_TIME)-iPreviousTime < 30);
	iPreviousTime = glutGet(GLUT_ELAPSED_TIME);
#else
	FramesPerSec();
	DrawText();
#endif

	//Swap to screen
	glutSwapBuffers();

	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	}



void CMyRenderer::SimulateDOF()
	{
	//Read pixels from the buffer
	
	//configure the transforms for 2D
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0.0f, iScreenWidth, 0.0f, iScreenHeight);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

	//Set the reading buffer
	glReadBuffer( GL_BACK );

//	const int width=200;
//	const int height=150;
	//float pixelBuffer[height*width*4];
	//float depthBuffer[height*width];

	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer1 );
	//glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer2 );
	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_DEPTH_COMPONENT, GL_FLOAT, iDepthBuffer);
	
	//Modify Pixels according to depth
	float z;
	//float cocDiameter = 20.0f;

	float cocDiameter(5.0f);
	float aperture     = 0.100f; //2.8f;
	float focalLength  = 0.750f; //1.0f;
	float planeOfFocus = 4; //0.971; //KZFar;//10.0f;

	for(int y=(0+cocDiameter); y<iScreenHeight-cocDiameter; y++)
		{
		for(int x=0+cocDiameter; x<iScreenWidth-cocDiameter; x++)
			{
			z = *(iDepthBuffer+(y*iScreenWidth)+x) ;
			z = (KZNear*KZFar)/(KZFar- (z*(KZFar-KZNear)) );
			if(KZFar!=z)
				{
				//cocDiameter = 40*z;

				cocDiameter = abs(  aperture - 3.3*(planeOfFocus*(1.0f/focalLength - 1.0f/z)-1) );
	//			//cocDiameter = abs(  aperture - 3.3*(planeOfFocus*(1 - (1.0f/z))-1) );
				ApplyFilter(cocDiameter, x, y );
				}
			}
		}
	//Write back to the buffer
	glDrawBuffer( GL_BACK );
	glRasterPos2i( 0, 0 );
	//glDrawPixels( iScreenWidth, iScreenHeight, GL_BLUE, GL_FLOAT, iPixelBuffer1);
	glDrawPixels( iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer2);
	glFlush();
	}

void CMyRenderer::ApplyFilter(int aCocDiameter, int aX, int aY )
	{
	//const int width=200;
	//const int height=150;

	float radius = aCocDiameter/2;
	float area   = pi*(radius*radius); //area of a circle

	//Get original pixel
	TColorRGB pixel( 
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX  ), 
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX+1), 
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX+2), 
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX+3) 
					);
	//Calculate intensity
	TColorRGB intensity = pixel/area;

	

	////Applying filter
	int radiusInt = static_cast<int>(radius);

	int pixelX, pixelY;
	int screenX = ( aX-radiusInt <0 )? radiusInt: aX;
	int screenY = ( aY-radiusInt <0 )? radiusInt: aY;
	screenX = ( aX+radiusInt >iScreenWidth )?  iScreenWidth-radiusInt:  screenX;
	screenY = ( aY+radiusInt >iScreenHeight )? iScreenHeight-radiusInt: screenY;

	for(int y= (-radiusInt); y<(radiusInt); y++)
		{
		for(int x=(-radiusInt); x<(radiusInt); x++)
			{
			//making circular filter
			if( sqrt( static_cast<float>(x*x+y*y)) < radius )
				{
				pixelX=(screenX-x);
				pixelY=(screenY-y);
				*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX  ) += intensity.getR();
				*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+1) += intensity.getG();
				*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+2) += intensity.getB();
				*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+3) = pixel.getA();
				}
			}
		}
	}


void CMyRenderer::DrawSceneNode( CSceneNode* aNode )
	{

	//---------------------------------------------------------------
	// PUSH NEW TRANSFORM ON TOP OF CTM (SAVING PARENT NODE'S MATRIX)
	//---------------------------------------------------------------
	// MULTIPLY NEW TRANSFORM(with the CTM)...
	glPushMatrix();

//	TMatrix4 m( iCTM.at(iCTM.size()-1) );
//	m*=aNode->getLocalTransform();
	//...PUSH IT TO THE CTM
//	iCTM.push_back(m);


	//----------------------
	// DIFFERENT NODE TYPES
	//----------------------
	switch(aNode->getNodeType())
		{
		// BASE NODE
		case CSceneNode::EBaseNode:
			{
			}
			break;

		//===============================
		// MESH NODE ->
		//	* NEEDS TO BE RENDERED *
		//===============================
		case CSceneNode::EMeshNode:
			{
			//Create a temporary object
			iMesh = new CMesh( *aNode->getMesh() );
			if(NULL!=iMesh)
				{
				//glScalef( iScale );
				//glutSolidSphere (1.0, 20, 16);
				RenderPipeLine( );

				//Object center
				if(iAxisDrawn)
					{
					TVector3 objCenter = TVector3(0,0,0); //iPerspectiveMatrix.multAndNormalizeW( m.mult( TVector3(0,0,0) ) );
					DrawAxisMark( objCenter.iX, objCenter.iY, objCenter.iZ, 0.05 );
					}

				//Delete the temporary object
				delete iMesh;
				}
			}
			break;

		// ROTATION NODE
		case CSceneNode::ERotationNode:
			{
			CSceneRotation* rNode=reinterpret_cast<CSceneRotation*>(aNode);
			glRotatef( rNode->iAngle, rNode->iAxis.iX, rNode->iAxis.iY, rNode->iAxis.iZ);
			}
			break;

		// TRANSLATION NODE
		case CSceneNode::ETranslationNode:
			{
			CSceneTranslation* tNode=reinterpret_cast<CSceneTranslation*>(aNode);
			glTranslatef(tNode->iX, tNode->iY, tNode->iZ);
			}
			break;

		// UNKNOWN NODE
		default:
			{
			}
			break;
		}
	//-------------------------------
	// BROWSE THE CHILDREN...
	//-------------------------------
	for(int i=0; i<aNode->getChildAmount();i++)
		{
		DrawSceneNode( aNode->getChild(i) );
		}

	//-------------------------------
	// POP THE PARENT NODE'S MATRIX
	//-------------------------------
	//iCTM.pop_back();
	glPopMatrix();
	}



void CMyRenderer::RenderPipeLine( )
	{
	//===============================
	// RENDERING PIPELINE
	//===============================

	//vertice count
	int vertCount(	 static_cast<int>( iMesh->iVertices.size()    ) );
	int normalCount( static_cast<int>( iMesh->iFaceNormals.size() ) );
	//bool iUseVertexNormals = (normalCount==vertCount)?true:false;
	bool visibility(true);

	TVector3  vx[3];//v1, v2, v3;
	TVector3  nv[3];//nv1, nv2, nv3;
	TColorRGB triangleColor;//nv1, nv2, nv3;
	TColorRGB lightColors[3];

	TTriangle t;

	//Go through the Mesh Polygon by polygon
	for (int triangleIndex=0, triangleCount=static_cast<int>(iMesh->iTriangles.size()); triangleIndex<triangleCount; triangleIndex++)
		{
		//Current triangle
		t = iMesh->iTriangles.at(triangleIndex);
		triangleColor = iMesh->iFaceColors.at(triangleIndex);

		//check that there is enough vertices in the vertices list
		if( (t.V1()<=vertCount ) && (t.V2()<=vertCount) && (t.V3()<=vertCount) )
			{
			//VERTICES
			vx[0] = iMesh->iVertices.at(t.V1());
			vx[1] = iMesh->iVertices.at(t.V2());
			vx[2] = iMesh->iVertices.at(t.V3());

			//NORMALS
			if( iUseVertexNormals )
				{
				nv[0] = iMesh->iVertexNormals.at(t.V1());
				nv[1] = iMesh->iVertexNormals.at(t.V2());
				nv[2] = iMesh->iVertexNormals.at(t.V3());
				}
			else //face normals
				{
				nv[0] = iMesh->iFaceNormals.at(triangleIndex);
				nv[1] = nv[2] = nv[0];
				}

			DrawOnScreen( &vx[0], &nv[0], &lightColors[0], triangleColor,  true );
			}
		}
	}


void CMyRenderer::DrawOnScreen( TVector3 aVx[], TVector3 aNv[], TColorRGB aLightColors[], TColorRGB aTriangleColor, bool aVisibility )
	{
	//Check if normal vectors are by vertex, or by polygon
	//bool vertexNormalsUsed = ( TVector3() == aNv[1] )?false:true;

	//-------------------------------
	// CHOOSE THE RENDERING TYPE
	//-------------------------------
	switch(iRenderType)
		{
		case ERenderVertices:
			glColor3f(1,1,1);
			glBegin( GL_POINTS );
				glVertex3f( aVx[0].iX, aVx[0].iY, aVx[0].iZ );
				glVertex3f( aVx[1].iX, aVx[1].iY, aVx[1].iZ );
				glVertex3f( aVx[2].iX, aVx[2].iY, aVx[2].iZ );
			glEnd();
			break;

		case ERenderWire:
			DrawWireTriangle( aVx[0], aVx[1], aVx[2], 1, 1, 1);
			break;
		case ERenderFaceShadedCulled:
			if(aVisibility)
				{
				if(iUseVertexNormals)
					{
					//USE LIGHTS
					//DrawTriangle(aVx[0], aVx[1], aVx[2], aLightColors[0], aLightColors[1], aLightColors[2] );
					//DrawTriangle( aVx[0], aVx[1], aVx[2], aTriangleColor.getR(), aTriangleColor.getG(), aTriangleColor.getB() );
					aLightColors[0]=aTriangleColor;
					DrawTriangle(aVx, aNv, aLightColors );
					}
				else
					{
					//USE LIGHTS
					DrawTriangle(aVx[0], aVx[1], aVx[2], aLightColors[0] );
					}
				}
			break;
		default:
			//on default don't render any geometry
			break;
		}
	if( (iNormalsDrawn ) && aVisibility)
		{
		if( iUseVertexNormals )
			{
			DrawVertexNormal( aVx, aNv );
			}
		else
			{
			DrawFaceNormal( aVx, aNv[0] );
			}
		}
	}


//-----------------------------
//resize the window
//-----------------------------
void CMyRenderer::ResizeScene(const int aWidth, const int aHeight)
	{
    iScreenWidth =  aWidth;
    iScreenHeight = aHeight;

	// We clear both the color and the depth buffer so to draw the next frame
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Viewport transformation
    glViewport( 0, 0, iScreenWidth, iScreenHeight );

	// Projection transformation
    glMatrixMode( GL_PROJECTION );
	// We initialize the projection matrix as identity
    glLoadIdentity();
    gluPerspective( 45.0f, (GLfloat)iScreenWidth / (GLfloat)iScreenHeight,-1.0f,10.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// This command redraw the scene (it calls the same routine of glutDisplayFunc)
    glutPostRedisplay ();
	}


//SETTERS
//
//----------------------------------------------

void CMyRenderer::SetScale( float aScale )
	{
	iScale = aScale;
	}

void CMyRenderer::SetPointOfRotation( const TVector3& aVertex )
	{
	iPointOfRotation = aVertex;
	}

void CMyRenderer::SetPointOfRotation( float x, float y, float z )
	{
	iPointOfRotation = TVector3(
								    x /(iScreenWidth /2)  -1		//X
								, -(y /(iScreenHeight/2) -1 )	//Y
								,   z
								);
	}

void CMyRenderer::SetAxisDrawn( const bool aDrawAxis )
	{
	iAxisDrawn = aDrawAxis;
	}

void CMyRenderer::SetNormalsDrawn( const bool aDrawNormals )
	{
	iNormalsDrawn = aDrawNormals;
	}

void CMyRenderer::SetUseVertexNormals( const bool aVNormals )
	{
	iUseVertexNormals = aVNormals;
	}

void CMyRenderer::SetAnglesChange( const TAngles& aAngle )
	{
	iAnglesChange = aAngle;
	}

void CMyRenderer::SetCurrentAngles( float aXAngle, float aYAngle, float aZAngle  )
	{
	iCurrentAngles = TAngles(aXAngle, aYAngle, aZAngle);
	}

// SORTERS
//-----------------------------------------------------
bool SortByObject(CMesh* a, CMesh* b)
    {
    return true;//a->iCurrentObjectPosition.iZ < b->iCurrentObjectPosition.iZ;
    }

//EXTERNAL FUNCTIONS TO USE GLUT CALLBACKS
//-----------------------------------------------------
void RenderSceneWithRenderer()
	{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->RenderScene();
	}

void ResizeSceneWithRenderer( int aWidth, int aHeight )
	{
	if (CMyRenderer::iCurrentRenderer != NULL)
		CMyRenderer::iCurrentRenderer->ResizeScene(aWidth, aHeight);
	}



