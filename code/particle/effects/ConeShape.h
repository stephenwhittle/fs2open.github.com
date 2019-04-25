#ifndef CONE_GENERATOR_EFFECT_H
#define CONE_GENERATOR_EFFECT_H
#pragma once

#include <core/pstypes.h>
#include "math/RandomRange.h"
#include <particle/ParticleEffect.h>
#include <math/vecmat.h>
#include <scp_compiler_detection.h>
namespace particle {
namespace effects {

/**
 * @ingroup particleEffects
 */
class ConeShape {
	::random::NormalFloatRange m_normalDeviation;
 public:
	ConeShape() {}

	matrix getDisplacementMatrix() {
		angles angs;

		angs.b = 0.0f;

		angs.h = m_normalDeviation.next();
		angs.p = m_normalDeviation.next();

		matrix m;

		vm_angles_2_matrix(&m, &angs);

		return m;
	}

	void parse(bool nocreate);

	EffectType getType() const { return EffectType::Cone; }

	/**
	 * @brief Specifies if the velocities of the particles should be scaled with the deviation from the direction
	 * @return @c true
	 */
	static SCP_CONSTEXPR bool scale_velocity_deviation() {
		return true;
	}
};

}
}

#endif // CONE_GENERATOR_EFFECT_H
