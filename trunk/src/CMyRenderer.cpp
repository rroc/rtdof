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

//Aspect ratio common values: 4/3(TV), 1.66(Cartoons), 16/9(HDTV), 2.35(Epic Movies)
const float KPerspectiveAspectRatio = float(4/3);
const float KPerspectiveAngle = float(45);



//VIEW FRUSTUM
const float KZNear	= 10.0f;
const float KZFar	= 600.0f;

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
	: iAnglesChange(KXAngleModStart, KYAngleModStart, KZAngleModStart)
	, iAxisDrawn( false )
	, iCurrentAngles(0,0,0)
	, iFocus( 0.55f )
	, iFocusArea( 7.0f )
	, iFrame(0)
	, iMeshIndex(0)
	, iNormalsDrawn( false )
	, iOldMeshIndex(0)
	, iPointOfRotation( KPointOfRotX, KPointOfRotY, KPointOfRotZ )
	, iPolyCount(0)
	, iPreviousFrame(0)
	, iPreviousTime(0)
	, iScale( KScaleStart )
	, iScreenHeight( KScreenHeight )
	, iScreenWidth( KScreenWidth )
	, iTransformInProcess( false )
	, iViewDistance(2)
	, iViewVector(100,0,-10)
	{
	InitMain();
	}

//Constructor wiht the screen size defined
CMyRenderer::CMyRenderer( const int aWidth, const int aHeight )
	: iAnglesChange( KYAngleModStart, KYAngleModStart, KZAngleModStart)
	, iAxisDrawn( false )
	, iCurrentAngles(0,0,0)
	, iFocus( 0.55f )
	, iFocusArea( 7.0f )
	, iFrame(0)
	, iMeshIndex(1)
	, iNormalsDrawn( false )
	, iOldMeshIndex(1)
	, iPointOfRotation( KPointOfRotX, KPointOfRotY, KPointOfRotZ )
	, iPolyCount(0)
	, iPreviousFrame(0)
	, iPreviousTime(0)
	, iScale( KScaleStart )
	, iScreenHeight( aHeight )
	, iScreenWidth( aWidth )
	, iTransformInProcess( false )
	, iViewDistance(2)
	, iViewVector(100,0,-10)//70.0, 3.0, -60.0)
	{
	InitMain();
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

#ifdef USE_FBO
	// remove the two textures and the framebuffer object
	glDeleteTextures(KNumberOfColorMaps, iColorMapId);
	glDeleteTextures(1, &iDepthMapId);
	glDeleteFramebuffersEXT(1, &iFrameBufferId);
#endif

#ifdef USE_SIN_TABLE
	delete CTrigTable::iTrigTable;
#endif
	}


//
/// Init function for the constructors
void CMyRenderer::InitMain()
	{

#ifdef USE_SIN_TABLE
	CTrigTable* trigtable = new CTrigTable();
	CTrigTable::iTrigTable = trigtable;
#endif

#ifdef USE_PIXEL_READ_DOF
	iPixelBuffer1 = new float[iScreenWidth*iScreenHeight*4];
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];
	iDepthBuffer  = new float[iScreenWidth*iScreenHeight];
#endif


#ifdef USE_FBO
	int colorAttach(0);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &colorAttach);
	cout << "Max color attachments: " << colorAttach <<endl;
	if(3>colorAttach)
		{
		cout << "Your videocard needs more FBO color attachments supported to run this program."<<endl;
		exit(-1);
		}
	InitForFrameBufferObject();
#endif

#ifdef USE_SHADER
	SetShaders( iShaderProgramId[0], "shader/render.vert", "shader/render.frag" );
	SetShaders( iShaderProgramId[1], "shader/transform.vert", "shader/blur.frag" );
	SetShaders( iShaderProgramId[2], "shader/transform.vert", "shader/combine.frag" );
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


	//create the color textures (system needs to support at least 3, for the full functionality)
	//-------------------------
	//create colortextures (the first one is reserved for the depth)
	for(int i=0; i< KNumberOfColorMaps; i++)
		{
		//SET TEXTURE PARAMETERS
		glBindTexture( GL_TEXTURE_2D, iColorMapId[i] );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ); //Default is GL_REPEAT
	//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
		glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_RGBA8, KTextureWidth, KTextureHeight, KTextureBorder, GL_RGBA, GL_FLOAT, NULL );

		// attach texture to framebuffer
		glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, iColorMapId[i], KTextureLevel );
		}

	//// assign the window system framebuffer again
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, KWindowSystemFrameBuffer);
	glBindTexture( GL_TEXTURE_2D, 0 );

	//CHECK that everything is OK with the frame buffer
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
	iMeshList.clear();
	CMesh* meshObject = new CMesh();
	float size = 3.0f;

	//LOADER FOR 3DS
	//--------------
	TMeshLoader* loader = new TMeshLoader();


	//BEAR
	loader->Load3DS(*meshObject, "3ds/icebear.3ds", 0.06 );
	if(meshObject->iVertices.size()>1)
		{
		iMeshList.push_back( meshObject );
		}

	//ADD OTHER OBJECTS
	iMeshList.push_back( new CBall(32, size) );
	iMeshList.push_back( new CIcosahedron(size) );
	iMeshList.push_back( new CTitanic(size*0.6) );

	//LANDSCAPE
	meshObject = new CMesh();
	loader->Load3DS(*meshObject, "3ds/scene.3ds", 0.5 );
	if(meshObject->iVertices.size()>1)
		{
		iMeshList.push_back( meshObject );
		}


	//CALCULATE NORMALS FOR ALL
	//-------------------------
	for(int i=0, j=iMeshList.size(); i<j; i++)
		{
		iMeshList.at(i)->calculateVertexNormals();
		iMeshList.at(i)->randomColors();
		}
	
	//BEAR COLOR
	iMeshList.at(0)->randomColors( TColorRGB(0.9,0.4,0.2), 0.8 );
	//LANDSCAPE COLOR
	iMeshList.at(iMeshList.size()-1)->randomColors( TColorRGB(0.4,0.9,0.1), 0.2 );

	//--------------------------
	// BUILD THE SCENE:
	//--------------------------
	//BASE NODE:
	iScene = new CSceneNode();
	CSceneNode* currentNode = iScene;

	//INITIAL TRANSFORMATION
	currentNode = currentNode->addChild( new CSceneTranslation( TVector3(10,-25,-70) ) );

	//INITIAL ROTATION
	//iRootRot = new CSceneRotation( TAngles(10,20,0) );
	iRootRot = new CSceneRotation( TAngles(0.0f, 0.0f, 0.0f) );
	currentNode = currentNode->addChild( iRootRot );

	//FIRST MESH
	currentNode = currentNode->addChild( new CSceneMesh( &iMeshList.at(iMeshList.size()-1) ) );

	//INVERT ROTATION
	iRootRotNeg = new CSceneRotation( TAngles(0.0f, 0.0f, 0.0f) );
	currentNode = currentNode->addChild( iRootRotNeg );



	//OTHER NODES
	//SEPARATE OTHERS FROM SCENERY
	currentNode = currentNode->addChild( new CSceneTranslation( TVector3(-5,24,-4) ) );
	
	//Rotate around point
	iSceneRotations.push_back(new CSceneRotation( TAngles(10, 0, 0) ));
	currentNode = currentNode->addChild( iSceneRotations.at( iSceneRotations.size()-1) ); 
	currentNode = currentNode->addChild( new CSceneTranslation( TVector3(0,0,6) ) );

	//Set to the first one
	iSceneMesh = iMeshList.at(1);

	//Init holders: (these can be vectors later such as rotations)
	CSceneNode* mNodes[5];
	CSceneTranslation* tNodes[5];

	//TRANSFERS PER OBJECT
	TVector3 perObjectTransition(0.1, 0.1, (size+1)*3.0f);
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
			sprintf_s  (iFpsCountString,"FPS:%4.2f - POLYCOUNT: %d - FOCUS: %4.2f - RANGE: %4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ), iPolyCount, iFocus, iFocusArea);
#else
		sprintf(iFpsCountString,"FPS:%4.2f Polycount: %d", iFrame*1000.0/( iCurrentTime - iPreviousTime ), iPolyCount);
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
	float x( 50.0f ), y( 20.0f );

	glPushMatrix();

	//configure the transforms for 2D
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0f, iScreenWidth, 0.0f, iScreenHeight);
	//glMatrixMode( GL_MODELVIEW);
	//glLoadIdentity();

	glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);
	glRasterPos2f(x, y);
	for (int i=0, len=strlen(iFpsCountString); i < len; i++)
		{
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, iFpsCountString[i] );
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, iFpsCountString[i] );
		}
	//glFlush();
	glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

	glPopMatrix();
	}


// ********** GL DRAWING METHODs *******


void CMyRenderer::DrawTriangle(TVector3 aVx[], TVector3 aNv[], TColorRGB aCol) const
	{
	//FACE COLORING
	glColor3f( aCol.iR, aCol.iG, aCol.iB);

	glBegin(GL_TRIANGLES);
		glNormal3f(aNv[0].iX, aNv[0].iY, aNv[0].iZ);
		glVertex3f(aVx[0].iX, aVx[0].iY, aVx[0].iZ);

		glNormal3f(aNv[1].iX, aNv[1].iY, aNv[1].iZ);
		glVertex3f(aVx[1].iX, aVx[1].iY, aVx[1].iZ);

		glNormal3f(aNv[2].iX, aNv[2].iY, aNv[2].iZ);
		glVertex3f(aVx[2].iX, aVx[2].iY, aVx[2].iZ);
	glEnd();
	}



// *************	RENDERING METHODS *********** /

/** \brief Method that specifies how the screen is rendered
*/

void CMyRenderer::RenderScene()
	{
	//static float i(0);
	iPolyCount=0;
	int	loc(0);

	//SET PERSPECTIVE
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( KPerspectiveAngle, KPerspectiveAspectRatio, KZNear, KZFar);

	//Depth is needed for the first render pass
	glEnable(GL_DEPTH_TEST);

	//Rest is for the models...
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();


	//CHANGE ACTIVE OBJECT TYPE IF REQUIRED
	iMeshIndex = (iMeshIndex<0)?iMeshList.size()-2:iMeshIndex;
	iMeshIndex = ( (iMeshList.size()-2) < static_cast<unsigned int>(iMeshIndex) )?0:iMeshIndex;
	if(iOldMeshIndex != iMeshIndex)
		{
		iSceneMesh = iMeshList.at( iMeshIndex );
		iOldMeshIndex=iMeshIndex;
		}

	//APPLY ROTATIONS
	//Scene rotation seperate from others:
	iRootRot->Rotate( TAngles( 0.0, -iAnglesChange.iRotY, 0.0) );
	iRootRotNeg->Rotate( TAngles( 0.0, iAnglesChange.iRotY, 0.0) ); //(...just to make it independent)

	//Animate the rotation around a point
	iSceneRotations.at(0)->Rotate( TAngles( 0.0, -iAnglesChange.iRotY*(1.2), 0.0) );
	
	//Animate other rotations
	for(int i=1,j=iSceneRotations.size();i<j;i++)
		{
		iSceneRotations.at(i)->Rotate( iAnglesChange );
		}


/*
	//NICE TO KNOW: :)
	//(getting screen space<->objectspace)
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

	//Bind the first framebuffer object and its first color attachment
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFrameBufferId);
	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
#endif

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

#ifdef USE_SHADER	
	//ENABLE SHADER PROGRAM 0
	// This program:
	// 1. sends the color to the fragments 
	// 2. does Phong shading
	// 3. Saves distance vector in the alpha value
	glUseProgram(iShaderProgramId[0]);
	loc = glGetUniformLocation( iShaderProgramId[0], "distanceScale" ); glUniform1f( loc, 0.0140f );
	loc = glGetUniformLocation( iShaderProgramId[0], "Ks" ); glUniform1f( loc, 1.94 );
	loc = glGetUniformLocation( iShaderProgramId[0], "Kd" ); glUniform1f( loc, 0.96 );
#endif


//------------------------------------------------
//	REAL SCENE
//------------------------------------------------
	//DRAW Nodes
	DrawSceneNode( iScene );
	glFlush();
//------------------------------------------------


// APPLYING THE FIRST BLUR...
#if defined(USE_FBO) && defined ( USE_SHADER )
	//PREPARE THE FRAMEBUFFER OBJECT
	glDrawBuffer( GL_COLOR_ATTACHMENT1_EXT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//ENABLE SHADER PROGRAM 1
	//This program applies the plur to the input image
	glUseProgram(iShaderProgramId[1]);
	loc = glGetUniformLocation( iShaderProgramId[1], "sampleDist" );glUniform1f( loc, 0.0055f );
	loc = glGetUniformLocation( iShaderProgramId[1], "RT" );		glUniform1i( loc, 0 );

	//Render quad on a texture
	RenderSceneOnQuad( 0 );
#endif



// APPLYING THE SECOND BLUR...
#if defined(USE_FBO) && defined ( USE_SHADER )
	//PREPARE THE FRAMEBUFFER OBJECT
	glDrawBuffer( GL_COLOR_ATTACHMENT2_EXT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//ENABLE SHADER PROGRAM 1
	//The second pass of the blur (the amount of texture operations in GLSL is limited)
	glUseProgram(iShaderProgramId[1]);
	loc = glGetUniformLocation( iShaderProgramId[1], "sampleDist" );glUniform1f( loc, 0.01280f );
	loc = glGetUniformLocation( iShaderProgramId[1], "RT" );		glUniform1i( loc, 0 );

	//Render quad on a texture
	RenderSceneOnQuad( 1 );
#endif


// COMBINING THE SHARP IMAGE AND THE SECOND BLUR...
#if defined(USE_FBO) && defined ( USE_SHADER )
	//ENABLE SHADER PROGRAM 2
	// This program combines the texture 0 and texture 2, 
	// by using the alpha values from the first image amd smoothing the images to one.
	glUseProgram(iShaderProgramId[2]);
	loc = glGetUniformLocation( iShaderProgramId[2], "range" );	glUniform1f( loc, iFocusArea );
	loc = glGetUniformLocation( iShaderProgramId[2], "focus" );	glUniform1f( loc, iFocus );
	loc = glGetUniformLocation( iShaderProgramId[2], "RT" );	glUniform1i( loc, 0 );
	loc = glGetUniformLocation( iShaderProgramId[2], "Blur" );	glUniform1i( loc, 1 );
#endif


	//NOW DRAW TO THE SCREEN
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, KWindowSystemFrameBuffer );
	glDrawBuffer( GL_BACK );

	//Ensure the screen size stays
	glViewport( 0,0, iScreenWidth, iScreenHeight );
	
#ifdef USE_FBO
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//CLEAR SCREEN AND DEPTH BUFFER
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Enable texturing to be able to use textured object (quad)
	glEnable( GL_TEXTURE_2D );

	//Activate and bind the two input textures
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[0] );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[2] );

	//Draw the screen filling quad:
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
#endif

//DISABLE SHADER PROGRAM WHEN WRITING FPS ON THE SCREEN!
#ifdef USE_SHADER
	glUseProgram(0);
#endif


#ifdef USE_FPS_LIMIT
	//framerate limiter
	while(glutGet(GLUT_ELAPSED_TIME)-iPreviousFrame < 30);
	iPreviousFrame = glutGet(GLUT_ELAPSED_TIME);
#endif
	FramesPerSec();
	DrawText();
	glFlush(); // Force the execution of OpenGL commands so far

	//Swap to screen
	glutSwapBuffers();

	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	}



void CMyRenderer::RenderSceneOnQuad(int aColorMapId )
	{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( -1.0f, 1.0f, -1.0f, 1.0f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, iColorMapId[aColorMapId] );

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
	delete iPixelBuffer2;
	iPixelBuffer2 = new float[iScreenWidth*iScreenHeight*4];

	//SET THE READING BUFFER
	glReadBuffer( GL_BACK );
	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_RGBA, GL_FLOAT, iPixelBuffer1 );
	glReadPixels( 0,0, iScreenWidth, iScreenHeight, GL_DEPTH_COMPONENT, GL_FLOAT, iDepthBuffer);

	//MODIFY PIXELS ACCORDING TO DEPTH
	ModifyPixels();

	//Write back to the buffer
	glDrawBuffer( GL_BACK );
	glRasterPos2i( 0, 0 );
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


/// Browses through the entire node and draws every mesh on its way
//  also the matrices are applied
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




// Render a single object
// there are two implementations, one which draws every triangle separately 
// and one that uses vertex-(,color-, and normal-) arrays
void CMyRenderer::RenderObject( )
	{
	TTriangle t;

	//vertice count
	int vertCount(	 static_cast<int>( iMesh->iVertices.size()    ) );
	int normalCount( static_cast<int>( iMesh->iFaceNormals.size() ) );

#ifdef USE_VERTEX_ARRAYS
	
	//COLOR ARRAY (can be used with vertex colors otherwise its no good)
	//glColorPointer( 4, GL_FLOAT, 0, reinterpret_cast<void*>( &iMesh->iFaceColors.at(0) ) );

	//VECTOR ARRAY
	glVertexPointer(3, GL_FLOAT, 0, reinterpret_cast<void*>( &iMesh->iVertices.at(0) ) );
	
	//NORMAL ARRAY
	glNormalPointer( GL_FLOAT, 0, reinterpret_cast<void*>( &iMesh->iVertexNormals.at(0) ) );

	TColorRGB color;
	glBegin(GL_TRIANGLES);
	//Go through the Mesh Polygon by polygon
	for (int triangleIndex=0, triangleCount=static_cast<int>(iMesh->iTriangles.size()); triangleIndex<triangleCount; triangleIndex++)
		{
		t = iMesh->iTriangles.at(triangleIndex);

		color = iMesh->iFaceColors.at(triangleIndex);
		glColor3f( color.iR, color.iG, color.iB );
		glArrayElement( t.iV1 );
		glArrayElement( t.iV2 );
		glArrayElement( t.iV3 );
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
			}
		}
#endif

	}


//-----------------------------
//resize the window
//-----------------------------
void CMyRenderer::ResizeScene(const int aWidth, const int aHeight)
	{
    iScreenWidth =  aWidth;
    iScreenHeight = aHeight;

	glViewport( 0, 0, iScreenWidth, iScreenHeight );

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
			_ASSERT(0);
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




