#ifndef ANGLES_H
#define ANGLES_H

#include "Basic.h"

/** \brief Class that defines a three-component angles
*
*/
class TAngles
	{
	public:
		/**
		* Default constructor.
		* Initializes the three angles.
		*/
		TAngles()	: iRotX( 0 ), iRotY( 0 ), iRotZ( 0 ) {};

		/**
		* Constructor
		* Initializes the three angles.
		* @param aX rotation angle around X-axis
		* @param aY rotation angle around Y-axis
		* @param aX rotation angle around Z-axis
		*/
		TAngles( float aX, float aY, float aZ ) : iRotX( aX ), iRotY( aY ), iRotZ( aZ )
			{
			VerifyAngles();
			};

		/**
		* Copy constructor.
		*/
		TAngles(const TAngles& aAngle): iRotX( aAngle.iRotX ), iRotY( aAngle.iRotY ), iRotZ( aAngle.iRotZ ) {};

		/**
		* Destructor.
		*/
		~TAngles() {};

		//PUBLIC OPERATORS
		TAngles operator-(){ return TAngles(-iRotX, -iRotY, -iRotZ); };

		//assignments
		TAngles& operator+=( const TAngles& aV ){ iRotX+=aV.iRotX; iRotY+=aV.iRotY; iRotZ+=aV.iRotZ; return *this; };
		TAngles& operator-=( const TAngles& aV ){ iRotX-=aV.iRotX; iRotY-=aV.iRotY; iRotZ-=aV.iRotZ; return *this; };

		//math operators
		TAngles operator+( const TAngles& aV2 ) { return TAngles(*this)+=aV2; };
		TAngles operator-( const TAngles& aV2 ) { return TAngles(*this)-=aV2; };

		bool operator==( const TAngles& aV ) { return ( (iRotX==aV.iRotX)&&(iRotY==aV.iRotY)&&(iRotZ==aV.iRotZ) )? true:false;; };
		bool operator!=( const TAngles& aV ) { return !(*this==aV); };

		/**
		* Verifies the angles to +/- maximum
		*/
		inline TAngles* VerifyAngles();

	public:
		float iRotX; ///< The x-component of the Angle
		float iRotY; ///< The y-component of the Angle
		float iRotZ; ///< The z-component of the Angle
	};


inline
TAngles* TAngles::VerifyAngles()
	{
#ifdef USE_RADIANS
	const float pi2( 6.283185307179586476925286766559f );
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
#endif
