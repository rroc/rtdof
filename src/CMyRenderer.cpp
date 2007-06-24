//INCLUDES
#include "CMyRenderer.h"

//USE THIS FILE


#include "TAngles.h"
#include "TMeshLoader.h"
//#include "TImageHandler.h"
//#include "ImageIO.h"

#include "TTextFileHandler.h"


#ifdef USE_SIN_TABLE
#include "CTrigTable.h"
#endif

//CONSTANTS
const int KScreenWidth = 480; ///< Default screen width  ( = PSP's screen width)
const int KScreenHeight= 272; ///< Default screen height ( = PSP's screen height)

const int KTextureWidth = 512; ///< Texture max size
const int KTextureHeight= 512; ///< Texture max size

const int KWindowSystemFrameBuffer = 0; // Window System Framebuffer ID
const int KTextureBorder = 0; // texture border
const int KTextureLevel = 0; // texture level resolution


//VIEW FRUSTUM
const float KZNear	= 10.0f;
const float KZFar	= 300.0f;

//INIT STATIC DATA
CMyRenderer* CMyRenderer::iCurrentRenderer = 0;

//CONSTANTS
const float KXAngleModStart = 0.8f; //0.003f;
const float KYAngleModStart = -1.0f; //-0.003f;
const float KZAngleModStart = 0.0f;

const float KScaleStart = 0.0f;

const float KPointOfRotX = 0.0f;
const float KPointOfRotY = 0.0f;
const float KPointOfRotZ = 2.0f;


//CONSTRUCTORS
//
//Default constructor
CMyRenderer::CMyRenderer()
	: iScreenWidth( KScreenWidth )
	, iScreenHeight( KScreenHeight )
	, iViewVector(70.0, 3.0, -60.0)
	, iScale( KScaleStart )
	, iCurrentAngles(0,0,0)
	, iAnglesChange(KXAngleModStart, KYAngleModStart, KZAngleModStart)
	, iFrame(0)
	, iPolyCount(0)
	, iPreviousTime(0)
	, iAxisDrawn( false )
	, iNormalsDrawn( false )
	, iPointOfRotation( KPointOfRotX, KPointOfRotY, KPointOfRotZ )
	, iTransformInProcess( false )
	, iRenderType(ERenderFaceShadedCulled) //ERenderVertices)
	, iAmountOfTriangles(1)
	, iViewDistance(2)
	, iMeshIndex(0)
	, iOldMeshIndex(0)
	{
#ifdef USE_SIN_TABLE
	CTrigTable* trigtable = new CTrigTable();
	CTrigTable::iTrigTable = trigtable;
#endif

	iPixelBuffer1 = new float[iScreenWidth*iScreenHeight*4];
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];
	iDepthBuffer  = new float[iScreenWidth*iScreenHeight];

#ifdef USE_FBO
	InitForFrameBufferObject();
#endif

#ifdef USE_SHADER
	SetShaders( iShaderProgramId[0], "shader/toon.vert", "shader/toon.frag" );
#endif
	}

//Constructor wiht the screen size defined
CMyRenderer::CMyRenderer( const int aWidth, const int aHeight )
	: iScreenWidth( aWidth )
	, iScreenHeight( aHeight )
	, iViewVector(70.0, 3.0, -60.0)
	, iScale( KScaleStart )
	, iCurrentAngles(0,0,0)
	, iAnglesChange( KYAngleModStart, KYAngleModStart, KZAngleModStart)
	, iFrame(0)
	, iPolyCount(0)
	, iPreviousTime(0)
	, iAxisDrawn( false )
	, iNormalsDrawn( false )
	, iPointOfRotation( KPointOfRotX, KPointOfRotY, KPointOfRotZ )
	, iTransformInProcess( false )
	, iRenderType(ERenderFaceShadedCulled) //ERenderVertices)
	, iAmountOfTriangles(1)
	, iViewDistance(2)
	, iMeshIndex(1)
	, iOldMeshIndex(1)
	{
#ifdef USE_SIN_TABLE
	CTrigTable* trigtable = new CTrigTable();
	CTrigTable::iTrigTable = trigtable;
#endif

	iPixelBuffer1 = new float[iScreenWidth*iScreenHeight*4];
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];
	iDepthBuffer  = new float[iScreenWidth*iScreenHeight];

#ifdef USE_FBO
	InitForFrameBufferObject();
#endif

#ifdef USE_SHADER
	SetShaders( iShaderProgramId[0], "shader/render.vert", "shader/render.frag" );
	SetShaders( iShaderProgramId[1], "shader/transform.vert", "shader/blur.frag" );
	SetShaders( iShaderProgramId[2], "shader/transform.vert", "shader/combine.frag" );
#endif
	}





//
//DESTRUCTOR releases the used heap
//
CMyRenderer::~CMyRenderer()
	{
	if(iMeshList.size()>0)
		{
		for(int i=0,j=static_cast<int>(iMeshList.size());i<j; i++)
			{
			delete iMeshList.at(i);
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

	// remove the two textures and the framebuffer object
	glDeleteTextures(KNumberOfColorMaps, iColorMapId);
	glDeleteTextures(1, &iDepthMapId);
	glDeleteFramebuffersEXT(1, &iFrameBufferId);

#ifdef USE_SIN_TABLE
	delete CTrigTable::iTrigTable;
#endif
	}

















void CMyRenderer::InitForFrameBufferObject()
	{
	// get two texture identifiers that have not yet been used
	glGenTextures( KNumberOfColorMaps, iColorMapId );
	glGenTextures( 1, &iDepthMapId );

	//CREATE FRAMEBUFFER OBJECT (and attach the textures)
	//---------------------------------------------------
	// generate, create & assign the framebuffer object
	glGenFramebuffersEXT ( 1, &iFrameBufferId );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, iFrameBufferId );


	//create the depth texture
	//------------------------
	glEnable( GL_TEXTURE_2D );

//	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, iDepthMapId );
	// set texture parameters
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
  // set up the Texture size, type, format,...
	glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_DEPTH_COMPONENT, KTextureWidth, KTextureHeight, KTextureBorder, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

	// attach the depth to the framebuffer
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, iDepthMapId, KTextureLevel );



	//create the color textures
	//-------------------------

	//create colortextures (the first one is reserved for the depth)
	for(int i=0; i< KNumberOfColorMaps; i++)
		{
		//ACTIVATE A NEW TEXTURE UNIT:
//		glActiveTexture( GL_TEXTURE0+i );
//		glEnable( GL_TEXTURE_2D );

		//SET TEXTURE PARAMETERS
		glBindTexture( GL_TEXTURE_2D, iColorMapId[i] );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_RGBA8, KTextureWidth, KTextureHeight, KTextureBorder, GL_RGBA, GL_FLOAT, NULL );

		// attach texture to framebuffer
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, iColorMapId[i], KTextureLevel );

		////Enable multi textures
		////---------------------
		//glActiveTexture( GL_TEXTURE0+i );
		//glEnable( GL_TEXTURE_2D );
		//glBindTexture( GL_TEXTURE_2D, iColorMapId[i] );
		}

//	glActiveTexture( GL_TEXTURE1 );

//	glEnable( GL_TEXTURE_2D );
//	glBindTexture( GL_TEXTURE_2D, iColorMapId[1] );


	//// assign the window system framebuffer again
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, KWindowSystemFrameBuffer);
	glBindTexture( GL_TEXTURE_2D, 0 );
	CheckFrameBufferStatus();
	}

/** \brief Method that creates a mesh
*
*   Creates an instance of a desired mesh type
*/
void CMyRenderer::CreateScene()
	{
	//--------------------------
	// INITIALIZE MESHES
	//--------------------------
	float size = 2.0f;

	//LOAD FROM 3DS
	TMeshLoader* loader = new TMeshLoader();

	//BEAR
	CMesh* meshObject = new CMesh();
	loader->Load3DS(*meshObject, "3ds/icebear.3ds", 0.04 );
	if(meshObject->iVertices.size()>1)
		{
		iMeshList.push_back( meshObject );
		}
	//ADD OTHER OBJECTS
	iMeshList.push_back( new CBall(32, size) );
	iMeshList.push_back( new CIcosahedron(size-1) );
	iMeshList.push_back( new CTitanic(size/2) );

	//LANDSCAPE
	meshObject = new CMesh();
	loader->Load3DS(*meshObject, "3ds/scene.3ds", 0.5 );
	if(meshObject->iVertices.size()>1)
		{
		iMeshList.push_back( meshObject );
		}

	//CALCULATE NORMALS
	for(int i=0, j=iMeshList.size(); i<j; i++)
		{
		iMeshList.at(i)->calculateVertexNormals();
		iMeshList.at(i)->randomColors();
		}
	iMeshList.at(0)->setSolidColor(0.9f,0.9f,1.0f);
	iMeshList.at(iMeshList.size()-1)->setSolidColor(0.5f,0.8f,1.0f);
	iSceneMesh = iMeshList.at(1);


	//--------------------------
	// BUILD THE SCENE:
	//--------------------------

	//BASE NODE:
	iScene = new CSceneNode();
	CSceneNode* currentNode = iScene;

	//INITIAL TRANSFORMATION
	currentNode = currentNode->addChild( new CSceneTranslation( TVector3(0,-5,-25-(KZNear+size)) ) );
	//INITIAL ROTATION
	iRootRot = new CSceneRotation( TAngles(10,20,0) );
	currentNode = currentNode->addChild( iRootRot );
	//FIRST MESH
	currentNode = currentNode->addChild( new CSceneMesh( &iSceneMesh ) );


	//OTHER NODES

	//Init holders: (these can be vectors later such as rotations)
	CSceneNode* mNodes[5];
	CSceneTranslation* tNodes[5];

	//TRANSFERS PER OBJECT
	TVector3 perObjectTransition(0.1, 0.1, 5.0f);
	TAngles  perObjectRotation(5, 2, 4);

	//SET ALL:
	for(int i=0; i<5; i++)
		{
		mNodes[i] = new CSceneMesh( &iSceneMesh );
		tNodes[i] = new CSceneTranslation( perObjectTransition );
		iSceneRotations.push_back(new CSceneRotation( perObjectRotation ));

		currentNode = currentNode->addChild( tNodes[i] );
		currentNode = currentNode->addChild( iSceneRotations.at( iSceneRotations.size()-1) );//rNodes[i] ); //rNodes[i];
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
#ifdef _WIN32
		//sprintf_s(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));
		sprintf_s  (iFpsCountString,"FPS:%4.2f Mipmap: %4.2f Polycount: %d", iFrame*1000.0/( iCurrentTime - iPreviousTime ), iScale, iPolyCount);
#else
		sprintf(iFpsCountString,"FPS:%4.2f Mipmap: %4.2f Polycount: %d", iFrame*1000.0/( iCurrentTime - iPreviousTime ), iScale, iPolyCount);
#endif
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
	float x = 50.0f, y= 20.0f;

	glPushMatrix();

	//configure the transforms for 2D
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( 0.0f, iScreenWidth, 0.0f, iScreenHeight);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

	glColor4f(1.0,0.0,0.0,1.0);
	glRasterPos2f(x, y);
	for (int i=0, len=strlen(iFpsCountString); i < len; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, iFpsCountString[i] );
		//glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, iFpsCountString[i] );
		}
	//glFlush();

	glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);


	glPopMatrix();
	}


// ********** GL DRAWING METHODs *******

void CMyRenderer::DrawTriangle(TVector3 aVx[], TVector3 aNv[], TColorRGB aCol) const
	{
	glEnable(GL_DEPTH_TEST);
	//FACE COLORING
	//glColor3f( aCol.iR, aCol.iG, aCol.iB);

	GLfloat mat_diffuse[] = { aCol.iR, aCol.iG, aCol.iB };
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
	iPolyCount=0;
	int	loc(0);

	//To store the alpha values
	//glEnable (GL_BLEND);

	//SET PERSPECTIVE
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, iScreenWidth/iScreenHeight, KZNear, KZFar);
	glEnable(GL_DEPTH_TEST);

	//Rest is for the models...
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(70.0, 3.0, -60.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(iViewVector.iX,iViewVector.iY,iViewVector.iZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//CHANGE OBJECT IF NEEDED
	iMeshIndex = (iMeshIndex<0)?iMeshList.size()-2:iMeshIndex;
	iMeshIndex = ( (iMeshList.size()-2) < static_cast<unsigned int>(iMeshIndex) )?0:iMeshIndex;
	if(iOldMeshIndex != iMeshIndex)
		{
		iSceneMesh = iMeshList.at( iMeshIndex );
		iOldMeshIndex=iMeshIndex;
		}

	//APPLY ROTATIONS
	iRootRot->Rotate( iAnglesChange );
	//to all others as well
	for(int i=0,j=iSceneRotations.size();i<j;i++)
		{
		iSceneRotations.at(i)->Rotate( iAnglesChange );
		}


/*
	//NICE TO KNOW: :)
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLdouble x,y,z;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
	gluUnProject( 0.0, 0.0, 0.0, mvmatrix, projmatrix, viewport, &x, &y, &z);
	gluUnProject( viewport[2], viewport[3], 0.0, mvmatrix, projmatrix, viewport, &x, &y, &z);
*/

#ifdef USE_FBO
	//PREPARE THE FRAMEBUFFER OBJECT
	glViewport( 0,0, KTextureWidth, KTextureHeight );

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFrameBufferId);
	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
#endif

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#ifdef USE_SHADER	//ENABLE SHADER PROGRAM 0
	glUseProgram(iShaderProgramId[0]);
	loc;
	loc = glGetUniformLocation( iShaderProgramId[0], "distanceScale" );
	glUniform1f( loc, 0.0140f );
	loc = glGetUniformLocation( iShaderProgramId[0], "Ks" );
	glUniform1f( loc, 0.94 );
	loc = glGetUniformLocation( iShaderProgramId[0], "Kd" );
	glUniform1f( loc, 0.96 );
#endif


//------------------------------------------------
//	REAL SCENE
//------------------------------------------------
	//DRAW LANDSCAPE
	iMesh = iMeshList.at(iMeshList.size()-1);
	if(NULL!=iMesh)
		{
		glLoadIdentity();
		glTranslatef( 1.0, -20.0, -250-(KZNear) );
		RenderObject( );
		glLoadIdentity();
		}
	//DRAW Nodes
	DrawSceneNode( iScene );
	glFlush();
//------------------------------------------------



// APPLYING THE FIRST BLUR...
#ifdef USE_FBO
	//PREPARE THE FRAMEBUFFER OBJECT
	glDrawBuffer( GL_COLOR_ATTACHMENT1_EXT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#ifdef USE_SHADER	//ENABLE SHADER PROGRAM 1
	glUseProgram(iShaderProgramId[1]);
	loc = glGetUniformLocation( iShaderProgramId[1], "sampleDist" );
	glUniform1f( loc, 0.0105f );
	loc = glGetUniformLocation( iShaderProgramId[1], "RT" );
	glUniform1i( loc, 0 );
#endif

	//Render quad on a texture
	RenderSceneOnQuad( 0, -1, KTextureWidth, KTextureHeight  );
#endif

// APPLYING THE SECOND BLUR...
#ifdef USE_FBO
	//PREPARE THE FRAMEBUFFER OBJECT
	glDrawBuffer( GL_COLOR_ATTACHMENT2_EXT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#ifdef USE_SHADER	//ENABLE SHADER PROGRAM 1
	glUseProgram(iShaderProgramId[1]);
	loc = glGetUniformLocation( iShaderProgramId[1], "sampleDist" );
	glUniform1f( loc, 0.02280f );
	loc = glGetUniformLocation( iShaderProgramId[1], "RT" );
	glUniform1i( loc, 0 );
#endif

	//Render quad on a texture
	RenderSceneOnQuad( 1, -1, KTextureWidth, KTextureHeight  );
#endif


// COMBINING THE SHARP IMAGE AND THE SECOND BLUR...
//#ifdef USE_FBO
#ifdef USE_SHADER	//ENABLE SHADER PROGRAM 2
	glUseProgram(iShaderProgramId[2]);
	loc = glGetUniformLocation( iShaderProgramId[2], "range" );
	glUniform1f( loc, 10.9f );
	loc = glGetUniformLocation( iShaderProgramId[2], "focus" );
	glUniform1f( loc, 0.45f );
	loc = glGetUniformLocation( iShaderProgramId[2], "RT" );
	glUniform1i( loc, 0 );
	loc = glGetUniformLocation( iShaderProgramId[2], "Blur" );
	glUniform1i( loc, 1 );
#endif

	//NOW DRAW TO THE SCREEN
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, KWindowSystemFrameBuffer );

	glViewport( 0,0, iScreenWidth, iScreenHeight );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//gluOrtho2D( 0.0f, iScreenWidth, 0.0f, iScreenHeight );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//PREPARE THE SCREENBUFFER
	glDrawBuffer( GL_BACK );

	//CLEAR SCREEN AMD DEPTH BUFFER
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[0] );
	//glBindTexture( GL_TEXTURE_2D, iDepthMapId );

	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[2] );

//	glutWireSphere(1,20,16);

	glBegin(GL_QUADS);
		glMultiTexCoord2f( GL_TEXTURE0, 0, 0 );
		glMultiTexCoord2f( GL_TEXTURE1, 0, 0 );
		glVertex2i( -1, -1 );
		glMultiTexCoord2f( GL_TEXTURE0, 1, 0 );
		glMultiTexCoord2f( GL_TEXTURE1, 1, 0 );
		glVertex2i( 1, -1 );
		glMultiTexCoord2f( GL_TEXTURE0, 1, 1 );
		glMultiTexCoord2f( GL_TEXTURE1, 1, 1 );
		glVertex2i( 1, 1 );
		glMultiTexCoord2f( GL_TEXTURE0, 0, 1 );
		glMultiTexCoord2f( GL_TEXTURE1, 0, 1 );
		glVertex2i( -1, 1 );
	glEnd();

	glFlush();
	glDisable( GL_TEXTURE_2D );
//#endif

//DISABLE SHADER PROGRAM WHEN WRITING FPS ON THE SCREEN!
#ifdef USE_SHADER
	glUseProgram(0);
#endif


#ifdef USE_FPS_LIMIT
	//framerate limiter
	while(glutGet(GLUT_ELAPSED_TIME)-iPreviousTime < 30);
	iPreviousTime = glutGet(GLUT_ELAPSED_TIME);
#else
	FramesPerSec();
	DrawText();
	glFlush(); // Force the execution of OpenGL commands so far
#endif

	//Swap to screen
	glutSwapBuffers();

	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	}



void CMyRenderer::RenderSceneOnQuad(int aColorMapId1, int aColorMapId2, int aQuadWidth, int aQuadHeight )
	{
//	int aQuadWidth = 512;
//	int aQuadHeight = 512;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0f, aQuadWidth, 0.0f, aQuadHeight );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[aColorMapId1] );

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
		glMultiTexCoord2f( GL_TEXTURE0, 0,0 );
		glVertex2i( -1.0, -1.0 );

		glMultiTexCoord2f( GL_TEXTURE0, 1, 0 );
		glVertex2i( 1, -1.0 );

		glMultiTexCoord2f( GL_TEXTURE0, 1, 1 );
		glVertex2i( 1, 1 );

		glMultiTexCoord2f( GL_TEXTURE0, 0, 1 );
		glVertex2i( -1.0, 1 );
	glEnd();

	glFlush();
	glDisable( GL_TEXTURE_2D );

/*
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0f, aQuadWidth, 0.0f, aQuadHeight );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );

	glEnable( GL_TEXTURE_2D );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[aColorMapId1] );

	if ( 0 < aColorMapId2)
		{
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, iColorMapId[aColorMapId2] );
		}

	//RENDER MULTITEXTURE ON QUAD
	glBegin(GL_QUADS);
		glMultiTexCoord2f( GL_TEXTURE0, 0,0 );
		if ( 0 < aColorMapId2)
			{
			glMultiTexCoord2f( GL_TEXTURE1, 0,0 );
			}
		glVertex2i( -1, -1 );

		glMultiTexCoord2f( GL_TEXTURE0, 1, 0 );
		if ( 0 < aColorMapId2)
			{
			glMultiTexCoord2f( GL_TEXTURE1, 1, 0 );
			}
		glVertex2i( 1, -1 );

		glMultiTexCoord2f( GL_TEXTURE0, 1, 1 );
		if ( 0 < aColorMapId2)
			{
			glMultiTexCoord2f( GL_TEXTURE1, 1, 1 );
			}
		glVertex2i( 1, 1 );

		glMultiTexCoord2f( GL_TEXTURE0, 0, 1 );
		if ( 0 < aColorMapId2)
			{
			glMultiTexCoord2f( GL_TEXTURE1, 0, 1 );
			}
		glVertex2i( -1, 1 );
	glEnd();

	glFlush();
	glDisable( GL_TEXTURE_2D );

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
*/
	}



/** \brief Function thant applies the DOF simulation using z-buffer information
* and OpenGL glReadPixels();
*
*/
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

	//Clear this
	//glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer2 );
	delete iPixelBuffer2;
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];

	//SET THE READING BUFFER
	glReadBuffer( GL_BACK );
	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer1 );
	//glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer2 );
	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_DEPTH_COMPONENT, GL_FLOAT, iDepthBuffer);

	//MODIFY PIXELS ACCORDING TO DEPTH
	ModifyPixels();

	//Write back to the buffer
	glDrawBuffer( GL_BACK );
	glRasterPos2i( 0, 0 );
	//glDrawPixels( iScreenWidth, iScreenHeight, GL_LUMINANCE, GL_FLOAT, iDepthBuffer);
	//glDrawPixels( iScreenWidth, iScreenHeight, GL_BLUE, GL_FLOAT, iPixelBuffer1);
	glDrawPixels( iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer2);
	glFlush();
	}



/** \brief Browses through the depth buffer and sets the lens parameters
*
*/
void CMyRenderer::ModifyPixels()
	{
	float pixelDepth; //d  (from z-buffer)
	float cocDiameter(0.0f); //init to something

	//- LENS COEFFICIENTS
	float relativeLensPosition = 6.0f; //0.1f; //s
	float focalLength  = 5.4f; // f;
	float aperture = 150.0f; //2.8f;

	for(int y=0; y<iScreenHeight; y++)
		{
		for(int x=0; x<iScreenWidth; x++)
			{
			//DEPTH SPACE
			pixelDepth = *(iDepthBuffer+(y*iScreenWidth)+x); //d

			//NASTY CODE
			if( (1.0f == pixelDepth) )
				{
				//return;
				}
			else
				{
				float pdOrig =pixelDepth;
				//pixelDepth = pixelDepth*(KZFar-KZNear)+relativeLensPosition;	//(relativeLensPosition*KZFar)/(KZFar-(pixelDepth*(KZFar-relativeLensPosition)) );
				pixelDepth = (KZFar*KZNear) / (KZFar - ( pixelDepth * (KZFar-KZNear)) );

				//Calculate the circle of confusion:
				//cocDiameter = 30*pixelDepth;//pixelDepth; //abs(  aperture * ( relativeLensPosition*( 1.0f/focalLength - 1.0f/pixelDepth)-1) );
				cocDiameter = 2.0f+fabsf(  aperture * ( relativeLensPosition*( 1.0f/focalLength - 1.0f/pixelDepth)-1) );
				//cocDiameter = (1-pdOrig)*5+abs(  aperture * ( relativeLensPosition*( 1.0f/focalLength - 1.0f/pixelDepth)-1) );
				//cocDiameter = abs(  aperture * ( pow(relativeLensPosition*( (1.0f/focalLength)-(1.0f/(pixelDepth))-1),2) ));

				//Apply the filter accordingly:
				ApplyFilter(static_cast<int>(cocDiameter), x, y );
				}
			}
		}

	}


/** \brief Apply the average filter with a desired diameter
*
*/
void CMyRenderer::ApplyFilter(int aCocDiameter, int aX, int aY )
	{
	//aCocDiameter = (aCocDiameter<2)?2:aCocDiameter;
	float radius = aCocDiameter/2;
	float area   = pi*(radius*radius); //area of a circle

	//Get original pixel
	TColorRGB pixel(
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX  ),
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX+1),
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX+2),
					*(iPixelBuffer1+(aY*iScreenWidth*4)+4*aX+3)
					);

	//if(radius<2.0f)
	//	{
	//	*(iPixelBuffer2+(aY*iScreenWidth*4)+4*aX  ) += pixel.getR()-(0.2*radius);
	//	*(iPixelBuffer2+(aY*iScreenWidth*4)+4*aX+1) += pixel.getG()-(0.2*radius);
	//	*(iPixelBuffer2+(aY*iScreenWidth*4)+4*aX+2) += pixel.getB()-(0.2*radius);
	//	*(iPixelBuffer2+(aY*iScreenWidth*4)+4*aX+3) += pixel.getA()-(0.2*radius);
	//	//radius=(radius>1)?2.0f:radius;
	//	}
	//else
	//	{
		//Calculate intensity
		TColorRGB intensity = pixel/area;

		//Keep the Alpha
		*(iPixelBuffer2+(aY*iScreenWidth*4)+4*aX+3)  = pixel.iA;

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

					//float factorX =  (radiusInt-abs(x))/radiusInt;
					//float factorY =  (radiusInt-abs(y))/radiusInt;
					//float factor = 0.5+(factorX+factorY)/2;
					float factor = 1;

					*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX  ) += factor*intensity.iR;
					*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+1) += factor*intensity.iG;
					*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+2) += factor*intensity.iB;
					}
				}
			}
//		}

	}




void CMyRenderer::DrawSceneNode( CSceneNode* aNode )
	{

	// Push old transform (saving parent node's matrix)
	glPushMatrix();

	//----------------------
	// DIFFERENT NODE TYPES
	//----------------------
	switch(aNode->getNodeType())
		{
		//===============================
		// BASE NODE
		//===============================
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
			CSceneMesh* mNode=reinterpret_cast<CSceneMesh*>(aNode);

			//Create a temporary object
			iMesh = new CMesh( *(mNode->GetMesh()) );
			if(NULL!=iMesh)
				{
				RenderObject( );

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

		//===============================
		// ROTATION NODE
		//===============================
		case CSceneNode::ERotationNode:
			{
			CSceneRotation* rNode=reinterpret_cast<CSceneRotation*>(aNode);
			glRotatef( rNode->iAngles.iRotX, 1,0,0 );
			glRotatef( rNode->iAngles.iRotY, 0,1,0 );
			glRotatef( rNode->iAngles.iRotZ, 0,0,1 );
			}
			break;

		//===============================
		// TRANSLATION NODE
		//===============================
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
	for(int i=0, j=aNode->getChildAmount(); i<j; i++)
		{
		DrawSceneNode( aNode->getChild(i) );
		}

	//-------------------------------
	// POP THE PARENT NODE'S MATRIX
	//-------------------------------
	glPopMatrix();
	}



void CMyRenderer::RenderObject( )
	{
	TTriangle t;

	//vertice count
	int vertCount(	 static_cast<int>( iMesh->iVertices.size()    ) );
	int normalCount( static_cast<int>( iMesh->iFaceNormals.size() ) );

#ifdef USE_VERTEX_ARRAYS
	//COLOR ARRAY
	glColorPointer( 4, GL_FLOAT, 0, reinterpret_cast<void*>( &iMesh->iFaceColors.at(0) ) );

	//VECTOR ARRAY
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<void*>( &iMesh->iVertices.at(0) ) );
	
	//NORMAL ARRAY
	glNormalPointer( GL_FLOAT, 0, reinterpret_cast<void*>( &iMesh->iVertexNormals.at(0) ) );

	glBegin(GL_TRIANGLES);
	//Go through the Mesh Polygon by polygon
	for (int triangleIndex=0, triangleCount=static_cast<int>(iMesh->iTriangles.size()); triangleIndex<triangleCount; triangleIndex++)
		{
		t = iMesh->iTriangles.at(triangleIndex);

		glArrayElement( t.V1() );
		glArrayElement( t.V2() );
		glArrayElement( t.V3() );
		}
	glEnd();

	iPolyCount += iMesh->iTriangles.size()*3;

#else
	TVector3  vx[3];//v1, v2, v3;
	TVector3  nv[3];//nv1, nv2, nv3;
	TColorRGB triangleColor;//nv1, nv2, nv3;

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
			nv[0] = iMesh->iVertexNormals.at(t.V1());
			nv[1] = iMesh->iVertexNormals.at(t.V2());
			nv[2] = iMesh->iVertexNormals.at(t.V3());

			iPolyCount++;
			DrawTriangle( &vx[0], &nv[0], triangleColor );
			//DrawOnScreen( &vx[0], &nv[0], triangleColor );
			}
		}
#endif

	}


void CMyRenderer::DrawOnScreen( TVector3 aVx[], TVector3 aNv[], TColorRGB aTriangleColor )
	{
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
			iPolyCount++;
			DrawTriangle(aVx, aNv, aTriangleColor );
			break;

		default:
			//on default don't render any geometry
			break;
		}
	if(iNormalsDrawn )
		{
		DrawVertexNormal( aVx, aNv );
		}
	}








//-----------------------------
//resize the window
//-----------------------------
void CMyRenderer::ResizeScene(const int aWidth, const int aHeight)
	{
    iScreenWidth =  aWidth;
    iScreenHeight = aHeight;
/*
	int min = (iScreenHeight<iScreenWidth)?iScreenHeight:iScreenWidth;
	int max = (iScreenHeight<iScreenWidth)?iScreenWidth:iScreenHeight;
	max = (max<1)? 1:max;
	min = static_cast<int>(512*((static_cast<float>(min))/max));
	min = (min>512)? 512:min;
	max = (max>512)? 512:max;


	iFBOTextureHeight = (iScreenHeight<iScreenWidth)?min:max;
	iFBOTextureWidth  = (iScreenHeight<iScreenWidth)?max:min;

*/
	// We clear both the color and the depth buffer so to draw the next frame
//    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Viewport transformation
    //glViewport( 0, 0, iScreenWidth, iScreenHeight );

	//// Projection transformation
 //   glMatrixMode( GL_PROJECTION );
	//// We initialize the projection matrix as identity
 //   glLoadIdentity();
 //   gluPerspective( 45.0f, (GLfloat)iScreenWidth / (GLfloat)iScreenHeight,-1.0f,10.0f);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	// This command redraw the scene (it calls the same routine of glutDisplayFunc)
    glutPostRedisplay ();
	}



void CMyRenderer::SetShaders( int& aShaderProgramId, char* aVertexShader, char* aFragmentShader )
	{
	cout << "Setting shaders: " << aVertexShader << " and " << aFragmentShader <<endl;
	//INIT NAMES AND STORAGE
	GLuint vertexShaderId, fragmentShaderId;
	char   *vertexShaderStore = NULL, *fragmentShaderStore = NULL;

	vertexShaderId		= glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderId	= glCreateShader(GL_FRAGMENT_SHADER);

	//LOAD SHADERS
	vertexShaderStore    = TTextFileHandler::TextFileRead( aVertexShader );
	fragmentShaderStore = TTextFileHandler::TextFileRead( aFragmentShader );

	//CREATE SOURCE HOLDERS
	const char* vertexShaderStoreC		= vertexShaderStore;
	const char* fragmentShaderStoreC	= fragmentShaderStore;
	glShaderSource(vertexShaderId,   1, &vertexShaderStoreC, NULL);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderStoreC, NULL);

	free(vertexShaderStore);
	free(fragmentShaderStore);

	//COMPILE
	glCompileShader( vertexShaderId );
	glCompileShader( fragmentShaderId );

	//LINK PROGRAM
	aShaderProgramId = glCreateProgram();
	glAttachShader( aShaderProgramId, fragmentShaderId );
	glAttachShader( aShaderProgramId, vertexShaderId );
	glLinkProgram( aShaderProgramId );

	//Check the statuses:
	bool status(true);
	status &= VerifyShaderCompilation( vertexShaderId );
	status &= VerifyShaderCompilation( fragmentShaderId );
	status &= VerifyShaderProgram( aShaderProgramId );
	if ( !status ){ exit(-1); }
	}

void CMyRenderer::CheckFrameBufferStatus()
{
  GLenum status;
  status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  fprintf(stderr, "%x\n", status);
  switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			fprintf(stderr,"framebuffer complete!\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			fprintf(stderr,"framebuffer GL_FRAMEBUFFER_UNSUPPORTED_EXT\n");
			/* you gotta choose different formats */
			_ASSERT(0);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			fprintf(stderr,"framebuffer INCOMPLETE_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			fprintf(stderr,"framebuffer FRAMEBUFFER_MISSING_ATTACHMENT\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			fprintf(stderr,"framebuffer FRAMEBUFFER_DIMENSIONS\n");
			break;
//		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
//			fprintf(stderr,"framebuffer INCOMPLETE_DUPLICATE_ATTACHMENT\n");
//			break;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			fprintf(stderr,"framebuffer INCOMPLETE_FORMATS\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			fprintf(stderr,"framebuffer INCOMPLETE_DRAW_BUFFER\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			fprintf(stderr,"framebuffer INCOMPLETE_READ_BUFFER\n");
			break;
		case GL_FRAMEBUFFER_BINDING_EXT:
			fprintf(stderr,"framebuffer BINDING_EXT\n");
			break;
			/*
		case GL_FRAMEBUFFER_STATUS_ERROR_EXT:
			fprintf(stderr,"framebuffer STATUS_ERROR\n");
			break;
			*/ \
			default:
				/* programming error; will fail on all hardware */
				assert(0);
		}
}

bool CMyRenderer::VerifyShaderCompilation( int aShaderId )
	{
	GLint status;
	GLint type;
	glGetShaderiv( aShaderId, GL_COMPILE_STATUS, &status );
	glGetShaderiv( aShaderId, GL_SHADER_TYPE, &type );

	string typeString = (GL_VERTEX_SHADER==type)?"Vertex":"Fragment";

	if( GL_FALSE == status )
		{
		const int KLengthMax=500;
		char infoLog[KLengthMax];

		glGetShaderInfoLog( aShaderId, KLengthMax, NULL, infoLog);
		cout << " - "<< typeString <<" Shader compilation FAILED:\n" << infoLog << endl;

	//	exit(-1);
		}
	else
		{
		cout << " - "<< typeString <<" Shader compilation OK.\n";
		}
	return (0==status)? false:true;
	}


bool CMyRenderer::VerifyShaderProgram( int aShaderProgramId )
	{
	GLint status;
	glGetProgramiv( aShaderProgramId, GL_LINK_STATUS, &status);

	if( GL_FALSE == status )
		{
		const int KLengthMax=500;
		char infoLog[KLengthMax];
		glGetProgramInfoLog( aShaderProgramId, KLengthMax, NULL, infoLog);
		cout << " - Program Linking FAILED:\n" << infoLog << endl;
		}
	else
		{
		cout << " - Program Linking OK.\n";
		}
	return (0==status)? false:true;
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




