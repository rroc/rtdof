//INCLUDES
#include "TMatrix4.h"

//CLASS DEFINITION

/** \brief Default constructor
*
*
*	     The default constructor create a identity matrix
*
*		 1     0     0     0   <br>
*		 0     1     0     0   <br>
*		 0     0     1     0   <br>
*		 0     0     0     1   <br>
*
*
*/
TMatrix4::TMatrix4()
	//ROW 1
	:a11(1)
	,a12(0)
	,a13(0)
	,a14(0)
	//ROW 2
	,a21(0)
	,a22(1)
	,a23(0)
	,a24(0)
	//ROW 3
	,a31(0)
	,a32(0)
	,a33(1)
	,a34(0)
	//ROW 4
	,a41(0)
	,a42(0)
	,a43(0)
	,a44(1)
	{
	}

/** \brief Copy constructor
*
*	Copy the TMatrix4 object that is given as argument
*
* \param _v the object that shall be copied
*/
TMatrix4::TMatrix4(const TMatrix4& _v)
	//ROW 1
	:a11(_v.a11)
	,a12(_v.a12)
	,a13(_v.a13)
	,a14(_v.a14)
	//ROW2
	,a21(_v.a21)
	,a22(_v.a22)
	,a23(_v.a23)
	,a24(_v.a24)
	//ROW3
	,a31(_v.a31)
	,a32(_v.a32)
	,a33(_v.a33)
	,a34(_v.a34)
	//ROW4
	,a41(_v.a41)
	,a42(_v.a42)
	,a43(_v.a43)
	,a44(_v.a44)
	{
	}

/** \brief Set constructor
*
*
*	     The matrix is build up as following
*
*		 a11     a12     a13     a14   <br>
*		 a21     a22     a23     a24   <br>
*		 a31     a32     a33     a34   <br>
*		 a41     a42     a43     a44   <br>
*
*
*/
TMatrix4::TMatrix4(float _a11, float _a12, float _a13, float _a14,
		 float _a21, float _a22, float _a23, float _a24,
		 float _a31, float _a32, float _a33, float _a34,
		 float _a41, float _a42, float _a43, float _a44)
	//ROW 1
	:a11(_a11)
	,a12(_a12)
	,a13(_a13)
	,a14(_a14)
	//ROW2
	,a21(_a21)
	,a22(_a22)
	,a23(_a23)
	,a24(_a24)
	//ROW3
	,a31(_a31)
	,a32(_a32)
	,a33(_a33)
	,a34(_a34)
	//ROW4
	,a41(_a41)
	,a42(_a42)
	,a43(_a43)
	,a44(_a44)
	{
	}

// **** Public Functions

/** \brief Clear the matrix
*
*
* Build up an indentity matrix
*
*		 1     0     0     0   <br>
*		 0     1     0     0   <br>
*		 0     0     1     0   <br>
*		 0     0     0     1   <br>
*
*
*/
void TMatrix4::clear()
	{
	this->a11 = this->a22 = this->a33 = this->a44 = 1;
	this->a12 =
		this->a13 =
		this->a14 =

		this->a21 =
		this->a23 =
		this->a24 =

		this->a31 =
		this->a32 =
		this->a34 =

		this->a41 =
		this->a42 =
		this->a43 = 0;
	}

/** \brief Method that copy the value from another matrix
*
*
*
*/
void TMatrix4::set(const TMatrix4& _m4)
	{
	this->a11 = _m4.a11;
	this->a12 = _m4.a12;
	this->a13 = _m4.a13;
	this->a14 = _m4.a14;

	this->a21 = _m4.a21;
	this->a22 = _m4.a22;
	this->a23 = _m4.a23;
	this->a24 = _m4.a24;

	this->a31 = _m4.a31;
	this->a32 = _m4.a32;
	this->a33 = _m4.a33;
	this->a34 = _m4.a34;

	this->a41 = _m4.a41;
	this->a42 = _m4.a42;
	this->a43 = _m4.a43;
	this->a44 = _m4.a44;
	}


TMatrix4 TMatrix4::loadIdentity( )
	{
	this->a12 =
	this->a13 =
	this->a14 =
	this->a21 =
	this->a23 =
	this->a24 =
	this->a31 =
	this->a32 =
	this->a34 =
	this->a41 =
	this->a42 =
	this->a43 = 0;

	this->a11 =	this->a22 =	this->a33 =	this->a44 = 1;
	return *this;
	}

/// Method that create a transformation matrix for rotation around X
TMatrix4 TMatrix4::getRotateXInstance( float _rx) const
	{
	TMatrix4 m;
#ifdef USE_SIN_TABLE
	m.a22 =  CTrigTable::iTrigTable->CosR( _rx );
	m.a32 =  CTrigTable::iTrigTable->SinR( _rx );;
#else
	m.a22 =  cos( _rx );
	m.a32 =  sin( _rx );
#endif
	m.a23 =  -m.a32;
	m.a33 =  m.a22;
	return m;
	}

/// Method that create a transformation matrix for rotation around X
TMatrix4 TMatrix4::getRotateAroundPointX( TVector3 aPoint, float aAngle) const
	{
	TMatrix4 m;
	float x(aPoint.iX);
	float y(aPoint.iY);
	float z(aPoint.iZ);
#ifdef USE_SIN_TABLE
	m.a22 =  CTrigTable::iTrigTable->CosR( aAngle );
	m.a23 = -CTrigTable::iTrigTable->SinR( aAngle );
	m.a24 = -CTrigTable::iTrigTable->CosR( aAngle )*y
				  +CTrigTable::iTrigTable->SinR( aAngle )*z+y;
	m.a32 = CTrigTable::iTrigTable->SinR( aAngle );
	m.a33 = -CTrigTable::iTrigTable->CosR( aAngle );
	m.a34 = -CTrigTable::iTrigTable->SinR( aAngle )*y
				  -CTrigTable::iTrigTable->CosR( aAngle )*z+z;
#else
	m.a22 =  cos( aAngle );
	m.a23 = -sin( aAngle );
	m.a24 = -cos( aAngle )*y
				 +sin( aAngle )*z+y;
	m.a32 = sin( aAngle );
	m.a33 = -cos( aAngle );
	m.a34 = -sin( aAngle )*y
				 -cos( aAngle )*z+z;
#endif
	return m;
	}

/// Method that create a transformation matrix for rotation around Y
TMatrix4 TMatrix4::getRotateYInstance(float _ry) const
	{
	TMatrix4 m;
#ifdef USE_SIN_TABLE
	m.a11 =  CTrigTable::iTrigTable->CosR( _ry );
	m.a13 =  CTrigTable::iTrigTable->SinR( _ry );;
#else
	m.a11 =  cos( _ry );
	m.a13 =  sin( _ry );
#endif
	m.a31 = -m.a13;
	m.a33 =  m.a11;
	return m;
	}

/// Method that creates a transformation matrix for rotation around Z
TMatrix4 TMatrix4::getRotateZInstance(float _rz) const
	{
	TMatrix4 m;

#ifdef USE_SIN_TABLE
	m.a11 =  CTrigTable::iTrigTable->CosR( _rz );
	m.a21 =  CTrigTable::iTrigTable->SinR( _rz );;
#else
	m.a11 =  cos( _rz );
	m.a21 =  sin( _rz );
#endif
	m.a12 = -m.a21;
	m.a22 =  m.a11;

	return m;
	}

///All rotations combined
TMatrix4 TMatrix4::getRotateAxisAngleInstance( const TVector3& aAxis, float aAngle) const
	{
	TVector3 axis(aAxis);
	axis.normalize();
	float x( axis.iX );
	float y( axis.iY );
	float z( axis.iZ );

#ifdef USE_SIN_TABLE
	float one_cos_a = (1-CTrigTable::iTrigTable->CosR(aAngle));
	float sin_a = CTrigTable::iTrigTable->SinR(aAngle);
#else
	float one_cos_a = (1-cos(aAngle));
	float sin_a = sin(aAngle);
#endif

	return TMatrix4
		(
		 1+one_cos_a*(x*x-1),
		-z*sin_a + one_cos_a*x*y,
		 y*sin_a + one_cos_a*x*z,
		 0,
		 z*sin_a + one_cos_a*y*x,
		 1+one_cos_a*(y*y-1),
		-x*sin_a + one_cos_a*y*z,
		 0,
		-y*sin_a + one_cos_a*z*x,
		 x*sin_a + one_cos_a*z*y,
		 1+one_cos_a*(z*z-1),
		 0,
		 0,0,0,1
		);
	}

TMatrix4 TMatrix4::getViewportInstance( int aX0,int aX1, int aY0,int aY1 ) const
	{
	TMatrix4 m;
	m.a11 = static_cast<float>(aX1-aX0)/2;
	m.a14 = aX0+static_cast<float>(aX1-aX0)/2.0f;
	m.a22 = static_cast<float>(aY1-aY0)/2;
	m.a22 = aY0+static_cast<float>(aY1-aY0)/2.0f;
	m.a33 =  1; 
	m.a44 =  1;
	return m;
	}

/// Method that create a transformation matrix for Perspective Projection to Z
TMatrix4 TMatrix4::getSimplePerpectiveZInstance( float aDistance ) const
	{
	TMatrix4 m;
	m.a43 =  1/aDistance;
	m.a44 =  0;
	return m;
	}

/// Method that create a transformation matrix for Perspective Frustum
TMatrix4 TMatrix4::getPerpectiveFrustumInstance( float aFOVAngle, float aNear, float aFar, float aAspectRatio ) const
	{
	float tan_FOW_div2 = tan( aFOVAngle/2 );
	TMatrix4 m;
	m.a11 = 1/(aAspectRatio*tan_FOW_div2);
	m.a22 = 1/tan_FOW_div2;
	m.a33 = (aNear+aFar)/(aNear-aFar);
	m.a34 = (2*aNear*aFar)/(aNear-aFar);
	m.a43 =  -1;
	m.a44 =  0;
	return m;
	}

/// Method that create a transformation matrix for Perspective Projection Volume
/*TMatrix4 TMatrix4::getPerpectiveVolumeInstance( float aD, float aF, float aH ) const
	{
	TMatrix4 m;
	m.a11 = aD/aH;
	m.a22 = aD/aH;
	m.a33 = aF/(aF-aD);
	m.a34 = -(aD*aF)/(aF-aD);
	m.a43 =  1;
	m.a44 =  0;
	return m;
	}
*/





/// Method that create a transformation matrix for translation
TMatrix4 TMatrix4::getTranslateInstance(float _tx, float _ty, float _tz) const
	{
	TMatrix4 m;
	m.a14 = _tx;
	m.a24 = _ty;
	m.a34 = _tz;
	return m;
	}


/// Method that create a matrix for uniform scaling
TMatrix4 TMatrix4::getScaleInstance(float _s) const
	{
	TMatrix4 m;
	m.a11 = _s;
	m.a22 = _s;
	m.a33 = _s;
	return m;
	}

/// Method that create a matrix for non-uniform scaling
TMatrix4 TMatrix4::getScaleInstance(float _sx, float _sy, float _sz) const
	{
	TMatrix4 m;
	m.a11 = _sx;
	m.a22 = _sy;
	m.a33 = _sz;
	return m;
	}

/// Method that create a matrix for non-uniform scaling
TMatrix4 TMatrix4::getCamera( TVector3 aEyePoint, TVector3 aTargetPoint, TVector3 aUpVector ) const
	{
	TVector3 c = (aEyePoint-aTargetPoint)/(aEyePoint.length()-aTargetPoint.length());
	TVector3 a = (aUpVector.cross(c))/(aUpVector.cross(c).length());
	TVector3 b=c.cross(a);

	return TMatrix4(
	a.iX,
	b.iX,
	c.iX,
	aEyePoint.iX,

	a.iY,
	b.iY,
	c.iY,
	aEyePoint.iY,

	a.iZ,
	b.iZ,
	c.iZ,
	aEyePoint.iZ,

	0,0,0,1 );
	}

/// Scale and translate to the viewport
TMatrix4 TMatrix4::getViewPortTransformation( int x0, int x1, int y0, int y1 ) const
	{
	return TMatrix4(
		(x1-x0)/2,
		0,
		0,
		(x0+x1)/2,

		0,
		(y1-y0)/2,
		0,
		(y0+y1)/2,

		0,
		0,
		0.5,
		0.5,

		0,0,0,1 );
	}



/** \brief Method to concatenate (rith) with another matrix
*
* Method to concatenate (right) with another matrix
*
* \param _m4 the matrix to concatenate from the right with this matrix
*
*/
void TMatrix4::RightConcat(const TMatrix4& _m4)
	{
	this->set(this->mult(_m4));
	}

/** \brief Method to concatenate (left) with another matrix
*
* Method to concatenate (left) with another matrix
*
* \param _m4 the matrix to concatenate from the left with this matrix
*
*/
void TMatrix4::LeftConcat(const TMatrix4& _m4)
	{
	this->set(_m4.mult(*this));
	}

/** \brief Method to multiply this with another matrix
*
* Method to multiply this with another matrix
*
* \param _m4 the second (right) matrix for the multiplication
* \return The resulting matrix product
*
*/
TMatrix4 TMatrix4::mult(const TMatrix4& _m4) const
	{
	return TMatrix4(
	this->a11*_m4.a11 + this->a12*_m4.a21 + this->a13*_m4.a31 + this->a14*_m4.a41,
	this->a11*_m4.a12 + this->a12*_m4.a22 + this->a13*_m4.a32 + this->a14*_m4.a42,
	this->a11*_m4.a13 + this->a12*_m4.a23 + this->a13*_m4.a33 + this->a14*_m4.a43,
	this->a11*_m4.a14 + this->a12*_m4.a24 + this->a13*_m4.a34 + this->a14*_m4.a44,

	this->a21*_m4.a11 + this->a22*_m4.a21 + this->a23*_m4.a31 + this->a24*_m4.a41,
	this->a21*_m4.a12 + this->a22*_m4.a22 + this->a23*_m4.a32 + this->a24*_m4.a42,
	this->a21*_m4.a13 + this->a22*_m4.a23 + this->a23*_m4.a33 + this->a24*_m4.a43,
	this->a21*_m4.a14 + this->a22*_m4.a24 + this->a23*_m4.a34 + this->a24*_m4.a44,

	this->a31*_m4.a11 + this->a32*_m4.a21 + this->a33*_m4.a31 + this->a34*_m4.a41,
	this->a31*_m4.a12 + this->a32*_m4.a22 + this->a33*_m4.a32 + this->a34*_m4.a42,
	this->a31*_m4.a13 + this->a32*_m4.a23 + this->a33*_m4.a33 + this->a34*_m4.a43,
	this->a31*_m4.a14 + this->a32*_m4.a24 + this->a33*_m4.a34 + this->a34*_m4.a44,

	this->a41*_m4.a11 + this->a42*_m4.a21 + this->a43*_m4.a31 + this->a44*_m4.a41,
	this->a41*_m4.a12 + this->a42*_m4.a22 + this->a43*_m4.a32 + this->a44*_m4.a42,
	this->a41*_m4.a13 + this->a42*_m4.a23 + this->a43*_m4.a33 + this->a44*_m4.a43,
	this->a41*_m4.a14 + this->a42*_m4.a24 + this->a43*_m4.a34 + this->a44*_m4.a44 );
	}


TVector3 TMatrix4::mult(const TVector3& _v3) const
	{
	return TVector3(                                              // + transform
		this->a11*_v3.iX + this->a12*_v3.iY + this->a13*_v3.iZ +this->a14,
		this->a21*_v3.iX + this->a22*_v3.iY + this->a23*_v3.iZ +this->a24,
		this->a31*_v3.iX + this->a32*_v3.iY + this->a33*_v3.iZ +this->a34
		);
	}

/** \brief Multiply matrix with a 3-element vector to transform it
 *
 * This method transform a TVector3 with the matrix object. Only
 * the upper left 3x3 portion of the matrix should be used.
 *
 *	(Translation and homogeneous coordinate changes are ignored)
 *
 * \param _v3 The vector to transform
 * \return TVector3 containing the transformed vertex
 *
 */
TVector3 TMatrix4::multVector(const TVector3& _v3) const
	{
	return TVector3(                                              // + transform
		this->a11*_v3.iX + this->a12*_v3.iY + this->a13*_v3.iZ,
		this->a21*_v3.iX + this->a22*_v3.iY + this->a23*_v3.iZ,
		this->a31*_v3.iX + this->a32*_v3.iY + this->a33*_v3.iZ
		);
	}


/** \brief Multiply matrix with a 3-element vector to transform it and normalize by W
 *
 * This method transform a TVector3 with the matrix object. Only
 * the upper left 3x3 portion of the matrix should be used.
 *
 *	(Translation and homogeneous coordinate changes are ignored)
 *
 * \param _v3 The vector to transform
 * \return TVector3 containing the transformed vertex
 *
 */
TVector3 TMatrix4::multAndNormalizeW(const TVector3& _v3) const
	{
	float w(this->a41*_v3.iX + this->a42*_v3.iY + this->a43*_v3.iZ + this->a44);
	w = ( 0==w )? 1:w;

	return TVector3(
				(this->a11*_v3.iX + this->a12*_v3.iY + this->a13*_v3.iZ + this->a14)/w ,
				(this->a21*_v3.iX + this->a22*_v3.iY + this->a23*_v3.iZ + this->a24)/w ,
				(this->a31*_v3.iX + this->a32*_v3.iY + this->a33*_v3.iZ + this->a34)/w
				);
	}


/** \brief Multiply two matrices
 *
 *	Method that multiply two matrices and return the result
 *
 *
 *\param  _m1 the first (left) matrix for the multiplication
 *\param  _m2 the second (right) matrix for the multiplication
 *\return TMatrix4 the resulting matrix product
 *
 */
TMatrix4 TMatrix4::mult(const TMatrix4& _m1, const TMatrix4& _m2) const
	{
	return TMatrix4(
		_m1.a11*_m2.a11 + _m1.a12*_m2.a21 + _m1.a13*_m2.a31 + _m1.a14*_m2.a41,
		_m1.a11*_m2.a12 + _m1.a12*_m2.a22 + _m1.a13*_m2.a32 + _m1.a14*_m2.a42,
		_m1.a11*_m2.a13 + _m1.a12*_m2.a23 + _m1.a13*_m2.a33 + _m1.a14*_m2.a43,
		_m1.a11*_m2.a14 + _m1.a12*_m2.a24 + _m1.a13*_m2.a34 + _m1.a14*_m2.a44,

		_m1.a21*_m2.a11 + _m1.a22*_m2.a21 + _m1.a23*_m2.a31 + _m1.a24*_m2.a41,
		_m1.a21*_m2.a12 + _m1.a22*_m2.a22 + _m1.a23*_m2.a32 + _m1.a24*_m2.a42,
		_m1.a21*_m2.a13 + _m1.a22*_m2.a23 + _m1.a23*_m2.a33 + _m1.a24*_m2.a43,
		_m1.a21*_m2.a14 + _m1.a22*_m2.a24 + _m1.a23*_m2.a34 + _m1.a24*_m2.a44,

		_m1.a31*_m2.a11 + _m1.a32*_m2.a21 + _m1.a33*_m2.a31 + _m1.a34*_m2.a41,
		_m1.a31*_m2.a12 + _m1.a32*_m2.a22 + _m1.a33*_m2.a32 + _m1.a34*_m2.a42,
		_m1.a31*_m2.a13 + _m1.a32*_m2.a23 + _m1.a33*_m2.a33 + _m1.a34*_m2.a43,
		_m1.a31*_m2.a14 + _m1.a32*_m2.a24 + _m1.a33*_m2.a34 + _m1.a34*_m2.a44,

		_m1.a41*_m2.a11 + _m1.a42*_m2.a21 + _m1.a43*_m2.a31 + _m1.a44*_m2.a41,
		_m1.a41*_m2.a12 + _m1.a42*_m2.a22 + _m1.a43*_m2.a32 + _m1.a44*_m2.a42,
		_m1.a41*_m2.a13 + _m1.a42*_m2.a23 + _m1.a43*_m2.a33 + _m1.a44*_m2.a43,
		_m1.a41*_m2.a14 + _m1.a42*_m2.a24 + _m1.a43*_m2.a34 + _m1.a44*_m2.a44);
	}

TMatrix4 TMatrix4::operator*(const TMatrix4& _m4)
	{
	return TMatrix4(
	this->a11*_m4.a11 + this->a12*_m4.a21 + this->a13*_m4.a31 + this->a14*_m4.a41,
	this->a11*_m4.a12 + this->a12*_m4.a22 + this->a13*_m4.a32 + this->a14*_m4.a42,
	this->a11*_m4.a13 + this->a12*_m4.a23 + this->a13*_m4.a33 + this->a14*_m4.a43,
	this->a11*_m4.a14 + this->a12*_m4.a24 + this->a13*_m4.a34 + this->a14*_m4.a44,

	this->a21*_m4.a11 + this->a22*_m4.a21 + this->a23*_m4.a31 + this->a24*_m4.a41,
	this->a21*_m4.a12 + this->a22*_m4.a22 + this->a23*_m4.a32 + this->a24*_m4.a42,
	this->a21*_m4.a13 + this->a22*_m4.a23 + this->a23*_m4.a33 + this->a24*_m4.a43,
	this->a21*_m4.a14 + this->a22*_m4.a24 + this->a23*_m4.a34 + this->a24*_m4.a44,

	this->a31*_m4.a11 + this->a32*_m4.a21 + this->a33*_m4.a31 + this->a34*_m4.a41,
	this->a31*_m4.a12 + this->a32*_m4.a22 + this->a33*_m4.a32 + this->a34*_m4.a42,
	this->a31*_m4.a13 + this->a32*_m4.a23 + this->a33*_m4.a33 + this->a34*_m4.a43,
	this->a31*_m4.a14 + this->a32*_m4.a24 + this->a33*_m4.a34 + this->a34*_m4.a44,

	this->a41*_m4.a11 + this->a42*_m4.a21 + this->a43*_m4.a31 + this->a44*_m4.a41,
	this->a41*_m4.a12 + this->a42*_m4.a22 + this->a43*_m4.a32 + this->a44*_m4.a42,
	this->a41*_m4.a13 + this->a42*_m4.a23 + this->a43*_m4.a33 + this->a44*_m4.a43,
	this->a41*_m4.a14 + this->a42*_m4.a24 + this->a43*_m4.a34 + this->a44*_m4.a44 );
	}

//OPERATING WITH THE CURRENT MATRIX
void TMatrix4::makeRotateAroundPointX( TVector3 aPoint, float aAngle)
	{
	*this *= getRotateAroundPointX( aPoint, aAngle);
	}

void TMatrix4::makeRotateX( float aAngle )
	{
	*this *= getRotateXInstance( aAngle );
	}

void TMatrix4::makeRotateY( float aAngle)
	{
	*this *= getRotateYInstance( aAngle);
	}

void TMatrix4::makeRotateZ( float aAngle )
	{
	*this *= getRotateZInstance( aAngle );
	}

void TMatrix4::makeRotateAxisAngle( const TVector3& aAxis, float aAngle)
	{
	*this *= getRotateAxisAngleInstance( aAxis, aAngle);
	}

void TMatrix4::makeTranslate( float aX, float aY, float aZ )
	{
	*this *= getTranslateInstance( aX, aY, aZ );
	}

void TMatrix4::makeScale( float aScale )
	{
	*this *= getScaleInstance( aScale );
	}

void TMatrix4::makeScale( float aX, float aY, float aZ )
	{
	*this *= getScaleInstance( aX, aY, aZ );
	}

void TMatrix4::makeSimplePerpectiveZ( float aDistance )
	{
	*this *= getSimplePerpectiveZInstance( aDistance );
	}

TMatrix4 TMatrix4::operator=(const TMatrix4& aMatrix)
	{
	this->set( aMatrix );
	return *this;
	}

TMatrix4 TMatrix4::operator*=(const TMatrix4& aMatrix)
	{
	this->set( (*this)*aMatrix );
	return *this;
	}

TMatrix4::~TMatrix4(void)
	{
	}

