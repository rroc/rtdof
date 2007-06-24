/*
 *  TLight.cpp
 *
 */

#include "TLight.h"

/** \brief Default constructor
 *
 *	This constructor creates a light source
 *  TColorRGB and TVector3 is set with there objects default values
 *	use the TLight::setColor and TLight::setDirection to give
 *	lightsource a color and direction, or the corresponding get methods
 *  to receive the objects
 *
 *
 */
TLight::TLight()
	{
	//empty on purpose
	}

TLight::TLight(const TColorRGB& aColor, const TVector3& aDirection) 
	: color(aColor), direction(aDirection.normalize())
	{
	//empty on purpose
	}

// **** Public methods
/** \brief Method that assign the light a color
 *
 *	Give the light source a new color, using a TColorRGB object
 *
 * \param _c an TColorRGB object containing the new colors
 */
void TLight::setColor(const TColorRGB& _c)
	{
	this->color = _c;
	}

/** \brief Method that assign the light source a new direction
 *
 *	Give the light source a new direction, using the TVector3 object
 *
 * \param _v an TVector3 object containing the new directoin of the light source
 */
void TLight::setDirection(const TVector3& _v)
	{
	this->direction = _v.normalize();
	}

/** \brief Method to retrive the color of the light source
 *
 * Method that return the TColorRGB object that define the color of the light emited from
 * the light source
 *
 * \return TColorRGB object containing the light of the light source
 */
TColorRGB TLight::getColor() const
	{
	return this->color;
	}

/** \brief Method to retrive the direction of the light source
 *
 * Method that return the TVector3 object that define the direction the
 * light source is pointing
 *
 * \return TVector3 object with the direction of the light source
 */
TVector3 TLight::getDirection() const
	{
	return this->direction;
	}


/** \brief Method to transform/rotat the light source
 *
 *	Transform the light source direction by a transformation matrix.
 *	Any translational components of the transformation are ignored
 *
 *
 * \param _t TMatrix4 containing the transformation matrix
 */
void TLight::transformDirection(const TMatrix4& _t)
	{
	this->direction = (_t.mult(this->direction)).normalize();
	}


// **** Private methods


// **** Deconstructor
TLight::~TLight(void)
	{
	}


