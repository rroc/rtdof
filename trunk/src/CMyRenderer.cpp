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

	InitForFrameBufferObject();
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
	//FrameBufferObjectExtW32();
	InitForFrameBufferObject();
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
	glDeleteTextures(2, iTextureID);
	glDeleteFramebuffersEXT(1, iFrameBufferID);

#ifdef USE_SIN_TABLE
	delete CTrigTable::iTrigTable;
#endif
	}

void CMyRenderer::InitForFrameBufferObject()
	{
	// get two texture identifiers that have not yet been used
	glGenTextures ( 2, iTextureID );

	// create & assign the first texture
	glBindTexture( GL_TEXTURE_2D, iTextureID[0] );

	// set up the Texture size, type, format,...
	// similar to glReadPixels and glDrawPixels
	// note: We're not supplying any texture image data since we're creating our own.
	// Hence, the NULL-parameter at the end of this function
	glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_RGBA8, iScreenWidth,
							  iScreenHeight, KTextureBorder, GL_RGBA, GL_FLOAT, NULL );

	// set texture parameters
	// filtering: see page 401 red book
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	// wrapping: see page 417 red book
	// texture(s,t,r,q) equal to vertex(x, y, z, w)
	// so in this case we're setting the wrap-parameters for the x direction
	// and the y direction
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );	

	// create & assign second texture
	glBindTexture( GL_TEXTURE_2D, iTextureID[1] );

	// set up the Texture size, type, format,...
	glTexImage2D( GL_TEXTURE_2D, KTextureLevel, GL_DEPTH_COMPONENT24, iScreenWidth, 
								iScreenHeight, KTextureBorder, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

	// set texture parameters
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// generate, create & assign the framebuffer object
	glGenFramebuffersEXT ( 1, iFrameBufferID );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, iFrameBufferID[0] );

	// attach the two textures to the framebuffer
	// (one is a color attachment, one is depth attachment)
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
														 GL_TEXTURE_2D, iTextureID[0], KTextureLevel );
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
														 GL_TEXTURE_2D, iTextureID[1], KTextureLevel );

	// assign the window system framebuffer again
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, KWindowSystemFrameBuffer);
	}

void CMyRenderer::FrameBufferObjectExtW32()
	{
			//
	// EXT_framebuffer_object
	//
	//char *ext = (char*)glGetString( GL_EXTENSIONS );

	//if( strstr( ext, "EXT_framebuffer_object" ) == NULL )
	//	{
	//	exit(-1);
	//	}
	//else
	//	{
		glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");

		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");

		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

		if( !glIsRenderbufferEXT || !glBindRenderbufferEXT || !glDeleteRenderbuffersEXT || 
			!glGenRenderbuffersEXT || !glRenderbufferStorageEXT || !glGetRenderbufferParameterivEXT || 
			!glIsFramebufferEXT || !glBindFramebufferEXT || !glDeleteFramebuffersEXT || 
			!glGenFramebuffersEXT || !glCheckFramebufferStatusEXT || !glFramebufferTexture1DEXT || 
			!glFramebufferTexture2DEXT || !glFramebufferTexture3DEXT || !glFramebufferRenderbufferEXT||  
			!glGetFramebufferAttachmentParameterivEXT || !glGenerateMipmapEXT )
		{
//			MessageBox(NULL,"One or more EXT_framebuffer_object functions were not found", "ERROR",MB_OK|MB_ICONEXCLAMATION);
			exit(-1);
		}
	//}
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

	CMesh* meshObject = new CMesh();
	loader->Load3DS(*meshObject, "3ds/icebear.3ds", 0.03 );
	if(meshObject->iVertices.size()>1)
		{
		iMeshList.push_back( meshObject );
		}
	//ADD OTHER OBJECTS
	iMeshList.push_back( new CBall(32, size) );
	iMeshList.push_back( new CIcosahedron(size-1) );
	iMeshList.push_back( new CTitanic(size/2) );

	//CALCULATE NORMALS
	for(int i=0, j=iMeshList.size(); i<j; i++)
		{
		iMeshList.at(i)->calculateVertexNormals();
		iMeshList.at(i)->randomColors();
		}
	iMeshList.at(0)->setSolidColor(0.8f,0.8f,1.0f);

	iSceneMesh = iMeshList.at(1);


	//--------------------------
	// BUILD THE SCENE:
	//--------------------------

	//BASE NODE:
	iScene = new CSceneNode();
	CSceneNode* currentNode = iScene; 

	//INITIAL TRANSFORMATION
	currentNode = currentNode->addChild( new CSceneTranslation( TVector3(0,0,-10-(KZNear+size)) ) );
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
	TVector3 perObjectTransition(0.1, 0.1, -5.0f);
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
		sprintf_s(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));
#else
		sprintf(iFpsCountString,"FPS:%4.2f", iFrame*1000.0/( iCurrentTime - iPreviousTime ));
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
void CMyRenderer::DrawTriangle(TVector3 aVx[], TVector3 aNv[], TColorRGB aCol) const
	{
	//FACE COLORING
	//glColor3f( aCol.getR(), aCol.getG(), aCol.getB());
	GLfloat mat_diffuse[] = { aCol.getR(), aCol.getG(), aCol.getB() };
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
	//CLEAR SCREEN AMD DEPTH BUFFER
	glClear( GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT );

	//check that perspective is on
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 45.0f, (GLfloat)iScreenWidth / (GLfloat)iScreenHeight, KZNear, KZFar);

	//Rest is for the models
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//CHANGE OBJECT IF NEEDED
	iMeshIndex = (iMeshIndex<0)?iMeshList.size()-1:iMeshIndex;
	iMeshIndex = (iMeshIndex>iMeshList.size()-1)?0:iMeshIndex;
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



	//PREPARE THE FRAMEBUFFER OBJECT
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, iFrameBufferID[0]);	
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//DRAW THE WHOLE SCENE (ALL OBJECTS)
	DrawSceneNode( iScene );
	//if(iAxisDrawn) DrawAxisMark( 0, 0, 0, 0.1 ); //Draw the center

	//FRAMEBUFFER OBJECT TO SCREEN
	//Assign the window-system framebuffer back
//	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, KWindowSystemFrameBuffer);
	//Render a quad with the pre-rendered scene as a texture
//	RenderSceneOnQuad();

	glFlush(); // Force the execution of OpenGL commands so far

	//DOF with pixel buffers
	//SimulateDOF();

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




void CMyRenderer::RenderSceneOnQuad()
	{
	int iQuadWidth= 400;
	int iQuadHeight = 200;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0f, iScreenWidth, 0.0f, iScreenHeight );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );

	glBindTexture( GL_TEXTURE_2D, iTextureID[0] );	
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );


	glBegin(GL_QUADS);
		glTexCoord2i( 0, 0 );	
		glVertex2i( iScreenWidth-iQuadWidth, iScreenHeight-iQuadHeight );
		
		glTexCoord2i( 0, 1 );
		glVertex2i( iScreenWidth, iScreenHeight-iQuadHeight );
		
		glTexCoord2i( 1, 1 );
		glVertex2i( iScreenWidth, iScreenHeight );
		
		glTexCoord2i( 1, 0 );
		glVertex2i( iScreenWidth-iQuadWidth, iScreenHeight );
	glEnd();

	glFlush();	
	glDisable( GL_TEXTURE_2D );
	
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
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
				cocDiameter = 2.0f+abs(  aperture * ( relativeLensPosition*( 1.0f/focalLength - 1.0f/pixelDepth)-1) );
				//cocDiameter = (1-pdOrig)*5+abs(  aperture * ( relativeLensPosition*( 1.0f/focalLength - 1.0f/pixelDepth)-1) );
				//cocDiameter = abs(  aperture * ( pow(relativeLensPosition*( (1.0f/focalLength)-(1.0f/(pixelDepth))-1),2) ));

				//Apply the filter accordingly:
				ApplyFilter(cocDiameter, x, y );
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
		*(iPixelBuffer2+(aY*iScreenWidth*4)+4*aX+3)  = pixel.getA();

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

					*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX  ) += factor*intensity.getR();
					*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+1) += factor*intensity.getG();
					*(iPixelBuffer2+(pixelY*iScreenWidth*4)+4*pixelX+2) += factor*intensity.getB();
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


void CMyRenderer::RenderPipeLine( )
	{
	//===============================
	// RENDERING PIPELINE
	//===============================

	//vertice count
	int vertCount(	 static_cast<int>( iMesh->iVertices.size()    ) );
	int normalCount( static_cast<int>( iMesh->iFaceNormals.size() ) );
	//bool iUseVertexNormals = (normalCount==vertCount)?true:false;

	TVector3  vx[3];//v1, v2, v3;
	TVector3  nv[3];//nv1, nv2, nv3;
	TColorRGB triangleColor;//nv1, nv2, nv3;

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
			DrawOnScreen( &vx[0], &nv[0], triangleColor );
			}
		}
	}


void CMyRenderer::DrawOnScreen( TVector3 aVx[], TVector3 aNv[], TColorRGB aTriangleColor )
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
			if(iUseVertexNormals)
				{
				DrawTriangle(aVx, aNv, aTriangleColor );
				}
			break;
		default:
			//on default don't render any geometry
			break;
		}
	if(iNormalsDrawn )
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



