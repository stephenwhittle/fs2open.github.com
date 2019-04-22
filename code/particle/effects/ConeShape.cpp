#include "ConeShape.h"

#include <particle/ParticleManager.h>
#include <math/floating.h>
#include <parse/parselo.h>
#include <math/RandomRange.h>

void particle::effects::ConeShape::parse(bool nocreate)
{
	if (internal::required_string_if_new("+Deviation:", nocreate)) {
		float deviation;
		stuff_float(&deviation);

		m_normalDeviation = ::random::NormalFloatRange(0.0, fl_radians(deviation));
	}
}
