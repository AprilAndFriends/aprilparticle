/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a color changer. This class is more light-weight compared to multi-color changer.

#ifndef APRILPARTICLE_AFFECTORS_COLOR_CHANGER_H
#define APRILPARTICLE_AFFECTORS_COLOR_CHANGER_H

#include <april/Color.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ColorChanger : public Affector
		{
			APRILPARTICLE_CLONEABLE(ColorChanger);
		public:
			ColorChanger(chstr name = "");
			ColorChanger(const april::Color& endColor, chstr name = "");
			ColorChanger(const april::Color& startColor, const april::Color& endColor, chstr name = "");
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(april::Color, startColor, StartColor);
			void setStartSymbolicColor(chstr value);
			HL_DEFINE_GETSET(april::Color, endColor, EndColor);
			void setEndSymbolicColor(chstr value);
				
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			april::Color startColor;
			april::Color endColor;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _ratio;

		};

	}
}
#endif
