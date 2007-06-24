#include "TColorRGB.h"

// **** Constructors
/** \brief Default constructor
*
*/
TColorRGB::TColorRGB()
	:r(1), g(1), b(1)
	{
	}

/** \brief Constructor given red, green and blue.
*
*	The value for r,g,b has to be in the intervall 0 > < 1
*   but this you have to ensure in your code, you can store
*	any float in this object, but if the value is not
*	between 0 and 1 the face/line will not show up.
*
*/
TColorRGB::TColorRGB(float _r, float _g, float _b)
	:r(_r), g(_g), b(_b)
	{
	}

TColorRGB::TColorRGB(float _r, float _g, float _b, float _a)
	:r(_r), g(_g), b(_b), a(_a)
	{
	}


/** \brief Constructor for grayscale value (r=g=b)
*
*/
TColorRGB::TColorRGB(float _g)
	:r(_g), g(_g), b(_g)
	{
	}

TColorRGB::TColorRGB(const TColorRGB& _c)
	:r(_c.r), g(_c.g), b(_c.b)
	{
	}

// **** Public methods

/** \brief Set the red green and blue
*
*/
void TColorRGB::set(float _r, float _g , float _b)
	{
	this->r=_r;
	this->g=_g;
	this->b=_b;
	this->a=1;
	}

void TColorRGB::set(float _r, float _g , float _b, float _a)
	{
	this->r=_r;
	this->g=_g;
	this->b=_b;
	this->a=_a;
	}


void TColorRGB::set( const TColorRGB& aColor )
	{
	this->r=aColor.r;
	this->g=aColor.g;
	this->b=aColor.b;
	this->a=aColor.a;
	}

/** \brief Return back the red value
*
*/
float TColorRGB::getR() const
	{
	return this->r;
	}

/** \brief Return back the green value
*
*/
float TColorRGB::getG() const
	{
	return this->g;
	}

/** \brief Return back the blue value
*
*/
float TColorRGB::getB() const
	{
	return this->b;
	}

/** \brief Return back the alpha value
*
*/
float TColorRGB::getA() const
	{
	return this->a;
	}

/** \brief Method that return a TColorRGB with random color
*
* You have to ensure that the values that is return is in the interval
* between 0 and 1. Feel free to implement new methods if you think that will help you.
*
*/
TColorRGB TColorRGB::randomColors()
	{
	//TColorRGB c;
	this->r = float(rand()%256)/255;
	this->g = float(rand()%256)/255;
	this->b = float(rand()%256)/255;
	this->a = 1.0f;
	return *this;
	}

/** \brief Method that return a TColorRGB with random color
*
* You have to ensure that the values that is return is in the interval
* between 0 and 1. Feel free to implement new methods if you think that will help you.
*
*/
TColorRGB TColorRGB::randomColors(int aSeed)
	{
	srand( aSeed );
	return randomColors();
	}

// **** Deconstructor
TColorRGB::~TColorRGB(void)
	{
	}

