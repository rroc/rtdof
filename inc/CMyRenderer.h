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

#include "CMesh.h"

#include "CSceneNode.h"
#include "CSceneRotation.h"
#include "CSceneMesh.h"
#include "CSceneTranslation.h"

#include "CTitanic.h"
#include "CIcosahedron.h"
#include "CBall.h"


//CONSTANTS:
const int KNumberOfColorMaps = 3;
const int KNumberOfShaderPrograms = 3;

//CLASS DECLARATION

/** \brief Rendering class.
*
* This class contains all the functions necessary for rendering.
*/
class CMyRenderer
	{
	//PUBLIC FUNCTIONS
	//------------------
	public:

		//CONSTRUCTORS
		//------------------

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
		//------------------
		/**
		* Default constructor.
		* Frees the reserved memory (iMesh).
		*/
		~CMyRenderer();


		//MEMBER FUNCTIONS
		///Renders a scene. This function is called from OpenGL, and renders a scene.
		void RenderScene();

		///Resize the rendering window
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

		TAngles RotAnglesChange() const
			{ return iRotAnglesChange; };


		//SETTERS
		//---------------------------------------
		
		/** Changes the focal plane
		* @param aFocusChange amount of focus change
		*/
		void ChangeFocus( float aFocusChange ) 
			{ iFocus += aFocusChange; }
		
		/** Changes the focus area
		* @param aFocusArea amount of focus area
		*/
		void ChangeFocusArea( float aFocusArea ) 
			{ iFocusArea += aFocusArea; }

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

		void SetRotAngles( const TAngles& aAngles )
			{iRotAnglesChange=TAngles(aAngles);}


	//PRIVATE FUNCTIONS
	//------------------
	private:
		//Constructors will call this one
		void InitMain();

		//Render current object
		void RenderObject( );

		// FPS calculation. Frames drawn in a second.
		void FramesPerSec();

		/// Initializie OpenGL Lights
		void InitLights();

		/// Text output. Prints text on the screen.
		void DrawText() const;



		//DRAWING FUNCTIONS
		//------------------
		/// Draws a triangle using the face color
		void DrawTriangle(TVector3 aVx[], TVector3 aNv[], TColorRGB aCol) const;
		void DrawSceneNode( CSceneNode* aNode );

		//FBO 
		//--------
		void InitForFrameBufferObject();
		void CheckFrameBufferStatus();
		void RenderSceneOnQuad( int aColorMapId );

		//SHADERS
		//--------
		void SetShaders( int& aShaderProgramId, char* aVertexShader, char* aFragmentShader );
		bool VerifyShaderCompilation( int aShaderId );
		bool VerifyShaderProgram( int aShaderProgramId );


		//Simulating DOF
		//---------------
		void SimulateDOF();
		void ModifyPixels();
		void ApplyFilter(int aCocDiameter, int aX, int aY );



	//PUBLIC STATIC DATA
	//------------------
	public:
		/// A static pointer to the current renderer object.
		/// This is used to be able to pass rendering method to OpenGL.
		static CMyRenderer* iCurrentRenderer;
		int	iMeshIndex;



	//PRIVATE DATA
	//------------------
	private:
		CMesh* iSceneMesh;	///< Mesh object List
		CMesh* iMesh;		///< Current Mesh object

		CSceneNode* iScene;
		CSceneRotation* iRootRot;
		CSceneRotation* iRootRotNeg;

		GLuint iColorMapId[ KNumberOfColorMaps ];
		GLuint iDepthMapId;
		GLuint iFrameBufferId;

		TAngles iAnglesChange;	///< Change of angles per frame (rot x, rot y, rot z)
		TAngles iCurrentAngles;	///< Current angles of mesh (rot x, rot y, rot z)
		TAngles iRotAnglesChange;

		TVector3 iPointOfRotation;	//< Center Point of rotation
		TVector3 iViewVector;		//< Viewing direction vector

		bool  iAxisDrawn;	///< Draw Axis or Not
		bool  iNormalsDrawn; ///< Draw Normals or Not
		bool  iTransformInProcess;	///< When true, do not interrupt

		char  iFpsCountString[ 60 ];	//< String for FPS
		
		float iCurrentTime;		//< Current time
		float iFocus;
		float iFocusArea;
		float iPreviousFrame;	///< The previous second
		float iPreviousTime;	///< The previous second
		float iScale;			///< Scaling to be applied
		float iViewDistance;

		float* iDepthBuffer;
		float* iPixelBuffer1;
		float* iPixelBuffer2;

		int	iOldMeshIndex;
		int iFrame;				//< Current frame number (used to approximate FPS)
		int iScreenHeight;	//< The height of the screen
		int iScreenWidth;	//< The width of the screen
		int iShaderProgramId[ KNumberOfShaderPrograms ];
		long int iPolyCount;

		vector<CMesh*> iMeshList; ///< CMesh object
		vector<CSceneRotation*> iSceneRotations; //current transformation matrix
	};


//EXTERNAL FUNCTIONS
/// Reference to rendering function. Used to pass the render object's rendering function to OpenGL.
extern void RenderSceneWithRenderer();
///Reference to resize function. Used to pass the render object's resize function to OpenGL.
extern void ResizeSceneWithRenderer( int aWidth, int aHeight );

#endif
