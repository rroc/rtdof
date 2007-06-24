#ifndef MYRENDERER_JOHANNES_H
#define MYRENDERER_JOHANNES_H

//INCLUDE FILES

// GLUT for Windows
//#include <GL/glut.h>

//***** MAC OS X GLUT
//#include <OpenGL/gl.h>		// Header File For The OpenGL32 Library for Mac os x
//#include <OpenGL/glu.h>		// Header File For The GLu32 Library for Mac os x
//#include <GLUT/glut.h>		// Header File For The GLUT Library for Mac os x

// ***** LINUX INCLUDE
//#include <GL/glut.h>    // Header File For The GLUT Library for linux
//#include <GL/gl.h>	// Header File For The OpenGL32 Library for linux
//#include <GL/glu.h>	// Header File For The GLu32 Library for linux

//#include <GL/glut.h>
//#include <GL/glprocs.h> //Header for the GL extensions in OGL 2.0 style


#include "Basic.h"


#include "TTriangle.h"
#include "TVector3.h"
#include "TAngles.h"
#include "TMatrix4.h"
#include "TColorRGB.h"
#include "TLight.h"

#include "CMesh.h"

#include "CSceneNode.h"
#include "CSceneRotation.h"
#include "CSceneMesh.h"
#include "CSceneTranslation.h"

#include "CTitanic.h"
#include "CIcosahedron.h"
#include "CBall.h"

//CLASS DECLARATION

/** \brief Rendering class.
*
* This class contains all the functions necessary for rendering.
*/
class CMyRenderer
	{
	public:

		enum TRenderType {
			ERenderVertices=0,
			ERenderWire,
			ERenderFaceShadedCulled, 

			//last value
			EEmpty
			};
	//PUBLIC FUNCTIONS
	public:

		//CONSTRUCTORS

		/**
		* Default constructor.
		* Initializes private data, and sets the window size to ( KScreenWidth, KScreenHeight ).
		*/
		CMyRenderer();

		/**
		* Default constructor.
		* Initializes private data, and sets the window size to ( aWidth, aHeight ).
		* @param aWidth width of the screen
		* @param aHeight height of the screen
		*/
		CMyRenderer( const int aWidth, const int aHeight );


		//DESTRUCTOR

		/**
		* Default constructor.
		* Frees the reserved memory (iMesh).
		*/
		~CMyRenderer();


		//MEMBER FUNCTIONS

		/**
		* FPS calculation. Frames drawn in a second.
		*/
		void FramesPerSec();

		/**
		* Initializie OpenGL Lights
		*/
		void InitLights();

		/**
		* Text output. Prints text on the screen.
		*/
		void DrawText() const;

		/**
		* Renders a scene. This function is called from OpenGL, and renders a scene.
		*/
		void RenderScene();

		/**
		* Renders a scene. This function is called from OpenGL when a window is resized,
		* iScreenWidth and iScreenHeight are updated, and the display is refreshed.
		* @param aWidth The new width of the screen
		* @param aHeight The new height of the screen
		*/
		void ResizeScene(const int aWidth, const int aHeight);

		///Creates a Scene
		void CreateScene();


		//GETTERS
		//---------------------------------------

		/**
		* Get a width of the screen.
		* @return iScreenWidth
		*/
		int Width() const 
			{ return iScreenWidth; };

		/**
		* Get a height of the screen.
		* @return iScreenHeight
		*/
		int Height() const 
			{ return iScreenHeight; };

		/**
		* Get current scale.
		* @return iScale
		* @see SetScale
		*/
		float Scale() const 
			{ return iScale; };

		/**
		* Get true if axis are drawn.
		* @return iAxisDrawn
		* @see SetDrawAxis
		*/
		bool AxisDrawn() const 
			{ return iAxisDrawn; };

		/**
		* Get true if normals are drawn.
		* @return iFaceNormalsDrawn
		*/
		bool NormalsDrawn() const 
			{ return iNormalsDrawn; };

		/**
		* Get point of rotation.
		* @return point of rotation
		*/
		TVector3 PointOfRotation() const 
			{ return iPointOfRotation; };

		/**
		* Get the change of the angles. This is the rotation modifier applied in each frame
		* @see SetAnglesChange
		* @return iAnglesChange
		*/
		TAngles AnglesChange() const 
			{ return iAnglesChange; };

		/**
		* Get the angles. This is the current angle of rotation
		* @return iAnglesChange
		*/
		TAngles CurrentAngles() const 
			{ return iCurrentAngles; };

		/**
		* Return the type of rendering.
		* @return iRenderType
		*/
		TRenderType RenderType() const 
			{ return iRenderType; };

		/**
		* Returns true if transform is in process. Usually do not interrupt!
		*/
		bool TransformInProcess() const
			{ return iTransformInProcess;}

		TAngles RotAnglesChange() const 
			{ return iRotAnglesChange; };


		//SETTERS
		//---------------------------------------

		/**
		* Set the scale. Sets the current iScale used in Calculations()
		* @param aScale the scaling factor, usually in the range 0.0 - 1.0
		* @see Scale
		* @see Calculations
		*/
		void SetScale( float aScale )
			{ iScale = aScale; }

		/**
		* Set the value to draw the axis or not.
		* @param aDrawAxis boolean value determining if axis are drawn or not
		* @see Calculations
		*/
		void SetAxisDrawn( const bool aDrawAxis )
			{ iAxisDrawn = aDrawAxis; }

		/**
		* Set the value to draw the face normals or not.
		* @param aDrawNormals boolean value determining if normals are drawn or not
		*/
		void SetNormalsDrawn( const bool aDrawNormals )
			{ iNormalsDrawn = aDrawNormals;}

		/**
		* Set the point of rotation.
		* @param aVector new point of rotation
		* @see Calculations
		*/
		void SetPointOfRotation( const TVector3& aVertex )
			{ iPointOfRotation = aVertex;}

		/**
		* Set the change of angle. This changes the rotation modifier applied in each frame
		* @param aScale the scaling factor, usually in the range 0.0 - 1.0
		* @see AnglesChange
		* @see Calculations
		*/
		void SetAnglesChange( const TAngles& aAngle )
			{ iAnglesChange = aAngle;}

		/**
		* Set the current angle. This changes the current rotation angle
		* @param aXAngle x-angle
		* @param aYAngle y-angle
		* @param aZAngle z-angle
		* @see AnglesChange
		* @see Calculations
		*/
		void SetCurrentAngles( float aXAngle, float aYAngle, float aZAngle  )
			{ iCurrentAngles = TAngles(aXAngle, aYAngle, aZAngle);}

		void SetRenderType( const TRenderType& aRenderType) 
			{iRenderType = aRenderType; };

		void SetRotAngles( const TAngles& aAngles )
			{iRotAnglesChange=TAngles(aAngles);}
		
		
		

	//PRIVATE METHODS
	private:

		void InitForFrameBufferObject();

		void RenderSceneOnQuad(void);
		void SimulateDOF();
		void ApplyFilter(int aCocDiameter, int aX, int aY );

		void SetShaders();

		//BASIC DRAWING FUNCTIONS

		void DrawSceneNode( CSceneNode* aNode );

		/**
		* Draw a triangle. Draws a triangle between vertices, this is used when rendering polygons.
		* @param aVx TVector3 table of 3 vertices
		* @param aNv TVector3 table of 3 normals
		* @param aCol TColorRGB face color
		*/
		void DrawTriangle(TVector3 aVx[], TVector3 aNv[], TColorRGB aCol) const;

		/**
		* Draws a triangle using OpenGL's 2D-lines. This is used when rendering polygons.
		* @param v1 Vertex 1
		* @param v2 Vertex 1
		* @param v3 Vertex 1
		* @param r element of RGB color
		* @param g element of RGB color
		* @param b element of RGB color
		*/
		void DrawWireTriangle( const TVector3& v1, const TVector3& v2, const TVector3& v3, float r, float g, float b ) const;

		/**
		* Draws a line using OpenGL's 3-D lines. This is used when rendering polygons.
		* @param v1 Vertex 1
		* @param v2 Vertex 1
		* @param r element of RGB color
		* @param g element of RGB color
		* @param b element of RGB color
		*/
		void DrawLine(const TVector3& v1, const TVector3& v2, float r, float g, float b) const;

		/**
		* Draw AxisMark
		* @param aX x-coord for the axis
		* @param aY y-coord for the axis
		* @param aZ z-coord for the axis
		* @param aSize size of the axis
		*/
		void DrawAxisMark( float aX, float aY, float aZ, float aSize) const;

		/**
		* Draw Face Normal
		* @param aMesh Mesh for which the normal is drawn
		* @param aTriangleIndex number of the triangle for the normal
		*/
		void DrawFaceNormal( TVector3 vx[], TVector3 nv) const;
		void DrawVertexNormal( TVector3 vx[], TVector3 nv[]) const;
		//void DrawFaceNormal( const CMesh& aMesh, const int aTriangleIndex ) const;

		void RenderPipeLine( );

		//void DrawOnScreen( int aTriangleIndex, bool aVertexNormalsUsed, bool aVisibility );
		void DrawOnScreen( TVector3 aVx[], TVector3 aNv[], TColorRGB aTriangleColor );

		void CMyRenderer::ModifyPixels();

		TColorRGB CalculateALight( TVector3 aNormalVector, TColorRGB aObjectColor );
		
		void CheckFrameBufferStatus();
		bool VerifyShaderCompilation( int aShaderId );
		bool VerifyShaderProgram( int aShaderProgramId );

	//PUBLIC STATIC DATA
	public:
		/// A static pointer to the current renderer object.
		/// This is used to be able to pass rendering method to OpenGL.
		static CMyRenderer* iCurrentRenderer;
		int iAmountOfTriangles;

		int	iMeshIndex;

	//PRIVATE DATA
	private:
		int iShaderProgramId;

		int iFBOTextureWidth;
		int iFBOTextureHeight;

		float* iPixelBuffer1;
		float* iPixelBuffer2;
		float* iDepthBuffer;
		
		GLuint iTextureID[2];
		GLuint iFrameBufferID[1];
		

		//OBJECT SPECIFICs
		CSceneNode* iScene;
		vector<CSceneRotation*> iSceneRotations; //current transformation matrix

		CSceneRotation* iRootRot;
		vector<TLight*> iLights;

		TMatrix4 iPerspectiveMatrix;
		TMatrix4 iViewportMatrix;

		CMesh* iMesh; ///< CMesh object

		vector<CMesh*> iMeshList; ///< CMesh object
		int	iOldMeshIndex;
		CMesh*  iSceneMesh; ///< CMesh object
		
		TAngles iRotAnglesChange;

		//WORLDMAP
		TAngles iCurrentAngles;	///< Current angles of mesh (rot x, rot y, rot z)
		TAngles iAnglesChange;	///< Change of angles per frame (rot x, rot y, rot z)
		float iScale;		///< Scaling to be applied

		bool  iAxisDrawn;	///< Draw Axis or Not
		bool  iNormalsDrawn; ///< Draw Normals or Not

		bool  iTransformInProcess;	///< When true, do not interrupt

		TVector3 iPointOfRotation;	//< Center Point of rotation
		TVector3 iViewVector;		//< Viewing direction vector

		float iViewDistance;

		char  iFpsCountString[60];	//< String for FPS
		int   iFrame;				//< Current frame number (used to approximate FPS)
		float iCurrentTime;			//< Current time
		float iPreviousTime;		//< The previous second
		long int   iPolyCount;

		int iScreenWidth;	//< The width of the screen
		int iScreenHeight;	//< The height of the screen

		TRenderType iRenderType;
	};


//EXTERNAL FUNCTIONS
/**
* Reference to rendering function. Used to pass the render object's rendering function to OpenGL.
*/
extern void RenderSceneWithRenderer();

/**
* Reference to resize function. Used to pass the render object's resize function to OpenGL.
*/
extern void ResizeSceneWithRenderer( int aWidth, int aHeight );

//SORTERS
bool SortByObject(CMesh* a, CMesh* b);

#endif
