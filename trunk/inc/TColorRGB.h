#ifndef COLORRGB1_H
#define COLORRGB1_H

/*
 *  CMesh.h
 *
 */
#include "Basic.h"

/** \brief RGB color class
*
*/
class TColorRGB
	{
	public:
		// Constructors
		TColorRGB()	
			:iR(1), iG(1), iB(1), iA(1){};
		TColorRGB(float _r , float _g, float _b)
			:iR(_r), iG(_g), iB(_b), iA(1){};
		TColorRGB(float _r , float _g, float _b, float _a)
			:iR(_r), iG(_g), iB(_b), iA(_a){};

		// Init to gray shade
		TColorRGB(float _g)
			:iR(_g), iG(_g), iB(_g), iA(1){};
		//Copy constructor
		TColorRGB(const TColorRGB& _c)
			:iR(_c.iR), iG(_c.iG), iB(_c.iB), iA(1){};

		//destructor
		~TColorRGB(void){};

		inline
		TColorRGB randomColors()
			{
			//TColorRGB c;
			this->iR = float(rand()%256)/255;
			this->iG = float(rand()%256)/255;
			this->iB = float(rand()%256)/255;
			this->iA = 1.0f;
			return *this;
			};

		inline
		TColorRGB randomColors(int aSeed )
			{
			srand( aSeed );
			return randomColors();
			};

		//PUBLIC OPERATORS
		TColorRGB operator-(){ return TColorRGB(-iR, -iG, -iB); };
		TColorRGB& operator+=( float aV ){ iR+=aV; iG+=aV; iB+=aV; return *this; };
		TColorRGB& operator+=( const TColorRGB& aV ){ iR+=aV.iR; iG+=aV.iG; iB+=aV.iB; return *this; };
		TColorRGB& operator-=( const TColorRGB& aV ){ iR-=aV.iR; iG-=aV.iG; iB-=aV.iB; return *this; };
		TColorRGB& operator-=( float aV ){ iR-=aV; iG-=aV; iB-=aV; return *this; };
		TColorRGB& operator*=( float aVal ){ iR*=aVal; iG*=aVal; iB*=aVal; return *this; };
		TColorRGB& operator*=( TColorRGB aVal ){ iR*=aVal.iR; iG*=aVal.iG; iB*=aVal.iB; return *this; };
		TColorRGB& operator/=( float aVal ){ iR/=aVal; iG/=aVal; iB/=aVal; return *this; };

		TColorRGB operator+( const TColorRGB& aV2 ) { return TColorRGB(*this)+=aV2; };
		TColorRGB operator+( float aV2 ) { return TColorRGB(*this)+=aV2; };
		TColorRGB operator-( const TColorRGB& aV2 ) { return TColorRGB(*this)-=aV2; };
		TColorRGB operator-( float aV2 ) { return TColorRGB(*this)-=aV2; };
		TColorRGB operator*( float aVal ) { return TColorRGB(*this)*=aVal; };
		TColorRGB operator*( TColorRGB aVal ) { return TColorRGB(*this)*=aVal; };
		TColorRGB operator/( float aVal ) { return TColorRGB(*this)/=aVal; };
		bool operator==( const TColorRGB& aV ) { return ( (iR==aV.iR)&&(iG==aV.iG)&&(iB==aV.iB) )? true:false;; };
		bool operator!=( const TColorRGB& aV ) { return !(*this==aV); };

	public:
		// functions

		/** \brief Private float colors
		*
		*/
		float iR, iG, iB, iA;
	};

#endif
