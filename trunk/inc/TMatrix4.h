#ifndef MATRIX4F1_H
#define MATRIX4F1_H

#include "Basic.h"
#include "TVector3.h"

#ifdef USE_SIN_TABLE
#include "CTrigTable.h"
#endif

/** \brief Class that defines an 4x4 matrix
*
*	Class that define an 4x4 matrix, different mathematical
*	operations that is associated with matrix and
*	some 3D computer graphics methods that uses matrix
*
*
*	The matrix is build up as following
*
*		 a11     a12     a13     a14   <br>
*		 a21     a22     a23     a24   <br>
*		 a31     a32     a33     a34   <br>
*		 a41     a42     a43     a44   <br>

*/
class TMatrix4 {
	public:
		// Constructors
		TMatrix4();
		TMatrix4(const TMatrix4&);
		TMatrix4(float, float, float, float,
				float, float, float, float,
				float, float, float, float,
				float, float, float, float);
		//Destructor
		~TMatrix4(void);

		//PUBLIC FUNCTIONS
		void clear(void);

		TMatrix4 loadIdentity( );
		TMatrix4 getRotateAroundPointX( TVector3 aPoint, float aAngle) const;
		TMatrix4 getRotateXInstance( float ) const;
		TMatrix4 getRotateYInstance( float ) const;
		TMatrix4 getRotateZInstance( float ) const;
		TMatrix4 getRotateAxisAngleInstance( const TVector3& aAxis, float aAngle) const;
		TMatrix4 getTranslateInstance( float, float, float ) const;
		TMatrix4 getScaleInstance( float ) const;
		TMatrix4 getScaleInstance( float, float, float ) const;

		TMatrix4 getSimplePerpectiveZInstance( float aDistance ) const;

		TMatrix4 getCamera( TVector3 aEyePoint, TVector3 aTargetPoint, TVector3 aUpVector ) const;
		TMatrix4 getViewPortTransformation( int x0, int x1, int y0, int y1 ) const;

		///Method that creates a transformation matrix for Perspective Frustum
		//Aspect ratio is the  x/y ratio of the final image
		// -common values: 4/3(TV), 1.66(Cartoons), 16/9(HDTV), 2.35(Epic Movies)
		TMatrix4 getPerpectiveFrustumInstance( float aFOVAngle, float aNear, float aFar, float aAspectRatio ) const;

		TMatrix4 getViewportInstance( int aX0,int aX1, int aY0,int aY1 ) const;

		//OPERATING WITH THE CURRENT MATRIX
		void makeRotateAroundPointX( TVector3 aPoint, float aAngle);
		void makeRotateX( float );
		void makeRotateY( float );
		void makeRotateZ( float );
		void makeRotateAxisAngle( const TVector3& aAxis, float aAngle);
		void makeTranslate( float, float, float );
		void makeScale( float );
		void makeScale( float, float, float );
		void makeSimplePerpectiveZ( float aDistance );

		void set(const TMatrix4&);

		void RightConcat(const TMatrix4&);
		void LeftConcat(const TMatrix4&);

		TMatrix4 mult(const TMatrix4&) const;
		TVector3 mult(const TVector3&) const;
		TVector3 multVector(const TVector3& _v3) const;
		TVector3 multAndNormalizeW(const TVector3&) const;

		TMatrix4 mult(const TMatrix4&, const TMatrix4&) const;

		TMatrix4 operator  *(const TMatrix4& aMatrix);
		TMatrix4 operator  =(const TMatrix4& aMatrix);
		TMatrix4 operator *=(const TMatrix4& aMatrix);

	private:
		/** \brief private variables, check the details what corresponds to what
		*	     The matrix is build up as following
		*
		*		 a11     a12     a13     a14   <br>
		*		 a21     a22     a23     a24   <br>
		*		 a31     a32     a33     a34   <br>
		*		 a41     a42     a43     a44   <br>
		*/
		float a11, a12, a13, a14,a21, a22, a23, a24, a31, a32, a33, a34, a41, a42, a43, a44;
	};

//INLINE FUNCTIONS




#endif
