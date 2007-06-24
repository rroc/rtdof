#include "TAngles.h"

const float pi2( 6.283185307179586476925286766559f );

/** \brief The default constructor
*
*	The default consctuctor that creates a zero angle
*	x = 0 <br>
*	y = 0 <br>
*	z = 0;
*
*/
TAngles::TAngles(void)
	: iRotX(0.0)
	, iRotY(0.0)
	, iRotZ(0.0)
	{
	//empty on purpose
	}

/** \brief Set constructor
*
*	The set constructor that takes three arguments
*
* \param _x the x component
* \param _y the y component
* \param _z the z component
*
*/
TAngles::TAngles( float _x, float _y, float _z )
	: iRotX( _x )
	, iRotY( _y )
	, iRotZ( _z )
	{
	VerifyAngles();
	}

/** \brief Copy constructor
*
*	Copy Constructor
*	\param _a the TAngles that should be copied
*/
TAngles::TAngles(const TAngles& _a)
	: iRotX( _a.iRotX )
	, iRotY( _a.iRotY )
	, iRotZ( _a.iRotZ )
	{
	//empty on purpose
	}

TAngles::~TAngles(void)
	{
	}

TAngles* TAngles::VerifyAngles()
	{
#ifdef USE_RADIANS
	//make the angles less or equal to 2pi
	while( this->iRotX > pi2) this->iRotX-=pi2;
	while( this->iRotY > pi2) this->iRotY-=pi2;
	while( this->iRotZ > pi2) this->iRotZ-=pi2;

	//make the angles atleast or equal to -2pi
	while( this->iRotX < -pi2) this->iRotX += pi2;
	while( this->iRotY < -pi2) this->iRotY += pi2;
	while( this->iRotZ < -pi2) this->iRotZ += pi2;

#else //#elif defined USE_DEGREES
	//make the angles less or equal to 360
	while( this->iRotX > 360) this->iRotX-=360;
	while( this->iRotY > 360) this->iRotY-=360;
	while( this->iRotZ > 360) this->iRotZ-=360;

	//make the angles atleast or equal to -360
	while( this->iRotX < -360) this->iRotX += 360;
	while( this->iRotY < -360) this->iRotY += 360;
	while( this->iRotZ < -360) this->iRotZ += 360;

#endif
	return this;
	}

TAngles operator+( TAngles& _a1, TAngles& _a2 )
	{
	TAngles ang
		(
		(_a1.iRotX + _a2.iRotX),
		(_a1.iRotY + _a2.iRotY),
		(_a1.iRotZ + _a2.iRotZ)
		);
	return *ang.VerifyAngles();
	}

TAngles operator-( TAngles& _a1, TAngles& _a2 )
	{
	TAngles ang
		(
		(_a1.iRotX - _a2.iRotX),
		(_a1.iRotY - _a2.iRotY),
		(_a1.iRotZ - _a2.iRotZ)
		);
	return *ang.VerifyAngles();
	}

TAngles operator+=( TAngles& _a1, TAngles& _a2 )
	{
	TAngles ang( _a1 + _a2 );
	_a1=ang;
	return ang;
	}

TAngles operator-=( TAngles& _a1, TAngles& _a2 )
	{
	TAngles ang( _a1 - _a2 );
	_a1=ang;
	return ang;
	}

///Overloaded comparison operator
bool operator==( TAngles& _v1, TAngles& _v2 )
	{
	if(    _v1.iRotX == _v2.iRotX
		&& _v1.iRotY == _v2.iRotY
		&& _v1.iRotZ == _v2.iRotZ
		)
		return true;
	else
		return false;

	}
///Overloaded negative comparison operator
bool operator!=( TAngles& _v1, TAngles& _v2 )
	{
	return !(_v1==_v2);
	}

