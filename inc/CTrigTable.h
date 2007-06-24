#ifndef TRIGTABLE_H
#define TRIGTABLE_H

//INCLUDES
#include "Basic.h"

//CONSTANTS
#ifdef USE_SYMBIAN_STYLE_CONSTANTS
	const float pi ( 3.1415926535897932384626433832795f );	///< pi
	const float pi2( 2*pi );		///< 2*pi
	const float pi_div2( (pi)/2 );	///< pi/2
	const float KDeg2Rad = 180/pi;
#else
	#define pi		3.1415926535897932384626433832795f
	#define pi2		6.283185307179586476925286766559f
	#define pi_div2 1.5707963267948966192313216916398f
	#define KDeg2Rad 180/pi;
#endif


/** \brief Precalculated trigonometric table
*
* This class should only have one instance, which is created at the software runtime.
* When an object is made, a table of sinus values is calculated and stored into a table.
*/
class CTrigTable
	{
	//PUBLIC FUNCTIONS
	public:
		/**
		* Default constructor.
		* Initializes the sinus-table
		*/
		CTrigTable();

		/**
		* Destructor.
		* Frees the memory from sinus-table
		*/
		~CTrigTable();

		/**
		* Calculate sinus for an angle given in radians.
		* @param aAngle angle in radians
		* @return approximated value of sin(aAngle)
		*/
		float SinR( float aAngle ) const;

		/**
		* Calculate cosinus for an angle given in radians.
		* @param aAngle angle in radians
		* @return approximated value of cos(aAngle)
		*/
		float CosR( float aAngle ) const;

	//PUBLIC STATIC DATA
	public:
		static CTrigTable* iTrigTable; ///< A static pointer to the current CTrigTable.

	//PRIVATE DATA
	private:
		float* iTable; ///< A pointer to the table of sinuses.
	};

#endif

