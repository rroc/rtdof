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
		TColorRGB();
		TColorRGB(float _r , float _g, float _b);

		// Init to gray shade
		TColorRGB(float _g);
		TColorRGB(const TColorRGB& _c);

		//destructor
		~TColorRGB(void);

		// methods
		void set( float _r, float _g, float _b );
		void set( const TColorRGB& aColor );

		float getR() const;
		float getG() const;
		float getB() const;

		TColorRGB randomColors();
		TColorRGB randomColors(int aSeed );

		//PUBLIC OPERATORS
		TColorRGB operator-(){ return TColorRGB(-r, -g, -b); };
		TColorRGB& operator+=( const TColorRGB& aV ){ r+=aV.r; g+=aV.g; b+=aV.b; return *this; };
		TColorRGB& operator-=( const TColorRGB& aV ){ r-=aV.r; g-=aV.g; b-=aV.b; return *this; };
		TColorRGB& operator*=( float aVal ){ r*=aVal; g*=aVal; b*=aVal; return *this; };
		TColorRGB& operator/=( float aVal ){ r/=aVal; g/=aVal; b/=aVal; return *this; };

		TColorRGB operator+( const TColorRGB& aV2 ) { return TColorRGB(*this)+=aV2; };
		TColorRGB operator-( const TColorRGB& aV2 ) { return TColorRGB(*this)-=aV2; };
		TColorRGB operator*( float aVal ) { return TColorRGB(*this)*=aVal; };
		TColorRGB operator/( float aVal ) { return TColorRGB(*this)/=aVal; };
		bool operator==( const TColorRGB& aV ) { return ( (r==aV.r)&&(g==aV.g)&&(b==aV.b) )? true:false;; };
		bool operator!=( const TColorRGB& aV ) { return !(*this==aV); };


	private:
		// functions

		/** \brief Private float colors
		*
		*/
		float r, g, b;
	};

#endif
