#ifndef LIGHT1_H
#define LIGHT1_H

#include "Basic.h"
#include "TVector3.h"
#include "TColorRGB.h"
#include "TMatrix4.h"

/** \brief Simple light source
*
* This class define a very simple light source, you can give
* the light source a direction and a color.
* The direction is set as a TVector3 and the color is stored in
* in a TColorRGB.
*
* If you want to do some more advanced light calculation
* this could be a good start.
*
*/
class TLight {
	public:
		// Constructors
		/** \brief Default constructor
		*
		*/
		TLight();
		TLight(const TColorRGB& aColor, const TVector3& aDirection);

		/** \brief Set a new color to the light source
		*
		*/
		void setColor(const TColorRGB&);

		/** \brief Set a new direction of the light source
		*
		*/
		void setDirection(const TVector3&);

		/** \brief Get the colors of the light source
		*
		*/
		TColorRGB getColor() const;

		/** \brief Get the direction of the light source
		*
		*/
		TVector3 getDirection() const;

		/** \brief Transform the direction of the light source
		*
		*/
		void transformDirection(const TMatrix4&);


		virtual ~TLight(void);

	private:

		/** \brief TColorRGB object with the color of the light source
		*
		*/
		TColorRGB color;

		/** \brief TVector3 object with the direction of the light source
		*
		*/
		TVector3 direction;
	};

#endif
