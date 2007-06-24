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
		TAngles();

		/**
		* Constructor
		* Initializes the three angles.
		* @param _x rotation angle around X-axis
		* @param _y rotation angle around Y-axis
		* @param _z rotation angle around Z-axis
		*/
		TAngles( float _x, float _y, float _z );

		/**
		* Copy constructor.
		*/
		TAngles(const TAngles&);

		/**
		* Destructor.
		*/
		~TAngles();

		/**
		* Verifies the angles to +/- maximum
		*/
		inline TAngles* VerifyAngles();

	public:
		float iRotX; ///< The x-component of the Angle
		float iRotY; ///< The y-component of the Angle
		float iRotZ; ///< The z-component of the Angle
	};

///Overloaded + operator to add angles
TAngles operator+ ( TAngles& _v1, TAngles& _v2 );
///Overloaded - operator to subtract angles
TAngles operator- ( TAngles& _v1, TAngles& _v2 );
///Overloaded += operator to insert add angles
TAngles operator+=( TAngles& _v1, TAngles& _v2 );
///Overloaded -= operator to insert subtract angles
TAngles operator-=( TAngles& _v1, TAngles& _v2 );

///Overloaded comparison operator
bool operator==( TAngles& _v1, TAngles& _v2 );
///Overloaded negative comparison operator
bool operator!=( TAngles& _v1, TAngles& _v2 );


#endif
