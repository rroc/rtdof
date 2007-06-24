#include "CTrigTable.h"

//CONSTANTS
#ifdef USE_RADIANS
 const float KFullTurn(pi2);
 const float KQuarterTurn(pi_div2);
 const int   KMultiplier( 1<<10 ); //Multiplier to get better values for radians 

#else //USE_DEGREES
 const int	 KFullTurn(360);
 const int   KQuarterTurn(45);
 const int   KMultiplier( 4 ); // 4=resolution of 0.25 degrees 
#endif

const int   KTableSize( static_cast<int>(KFullTurn * KMultiplier)+1 );	//(0-360 degrees)
const float KDegreeStep(KFullTurn / (KTableSize-1));			///< table resolution


//INIT STATIC DATA
CTrigTable* CTrigTable::iTrigTable = 0;


//CLASS DEFINITION

//Constructor
CTrigTable::CTrigTable()
	{
	iTable = new float[KTableSize];
	for(int i=0; i<KTableSize; i++)
		{
#ifdef USE_RADIANS
		*(iTable+i) = static_cast<float>( sin( i*KDegreeStep ) );
#else //USE_DEGREES
		*(iTable+i) = static_cast<float>( sin( i*KDegreeStep*KDeg2Rad ) );
#endif
		}
	}

//Constructor
CTrigTable::~CTrigTable()
	{
	delete [] iTable;
	iTable = 0;
	}

float CTrigTable::SinR( float aAngle ) const
	{
	//Check the sign
	while( aAngle < 0)			aAngle += KFullTurn;
	//Check that fits the 2pi area
	while( aAngle > KFullTurn)	aAngle -= KFullTurn;

	return (*(iTable + static_cast<int>( aAngle*KMultiplier )) );
	}

float CTrigTable::CosR( float aAngle ) const
	{
	//cos to sin
	aAngle += KQuarterTurn;
	return SinR( aAngle );
	}


