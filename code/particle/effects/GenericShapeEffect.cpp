#include "GenericShapeEffect.h"
#include <particle/effects/ConeShape.h>
#include <particle/effects/SphereShape.h>
#include <parse/parselo.h>
#include <math/RandomRange.h>

//todo::refactor so we can leave this in the header and hide the impl details
namespace particle {

	namespace effects {
	template <typename TShape>
	void GenericShapeEffect<TShape>::parseValues(bool nocreate)
	{
		m_particleProperties.parse(nocreate);

		m_shape.parse(nocreate);

		if (internal::required_string_if_new("+Velocity:", nocreate)) {
			m_velocity = ::random::parseUniformRange<float>();
		}

		if (internal::required_string_if_new("+Number:", nocreate)) {
			m_particleNum = ::random::parseUniformRange<uint>();
		}

		if (optional_string("+Direction:")) {
			std::string dirStr;
			stuff_string(dirStr, F_NAME);

			if (!stricmp(dirStr.c_str(), "Incoming")) {
				m_direction = ConeDirection::Incoming;
			} else if (!stricmp(dirStr.c_str(), "Normal")) {
				m_direction = ConeDirection::Normal;
			} else if (!stricmp(dirStr.c_str(), "Reflected")) {
				m_direction = ConeDirection::Reflected;
			} else if (!stricmp(dirStr.c_str(), "Reverse")) {
				m_direction = ConeDirection::Reverse;
			} else {
				error_display(0, "Unknown direction name '%s'!", dirStr.c_str());
			}
		}

		bool saw_deprecated_effect_location = false;
		if (optional_string("+Trail effect:")) {
			// This is the deprecated location since this introduces ambiguities in the parsing process
			m_particleTrail                = internal::parseEffectElement();
			saw_deprecated_effect_location = true;
		}

		m_timing = util::EffectTiming::parseTiming();

		if (optional_string("+Trail effect:")) {
			// This is the new and correct location. This might create duplicate effects but the warning should be clear
			// enough to avoid that
			if (saw_deprecated_effect_location) {
				error_display(0, "Found two trail effect options! Specifying '+Trail effect:' before '+Duration:' is "
				                 "deprecated since that can cause issues with conflicting effect options.");
			}
			m_particleTrail = internal::parseEffectElement();
		}
	}
	template void GenericShapeEffect<class ConeShape>::parseValues(bool);
	template void GenericShapeEffect<class SphereShape>::parseValues(bool);

	} // namespace effects
    }
