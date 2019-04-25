#ifndef COMPOSIE_EFFECT_H
#define COMPOSIE_EFFECT_H
#pragma once

#include <core/pstypes.h>
#include "particle/ParticleEffect.h"
#include "particle/ParticleManager.h"
#include "math/RandomRange.h"

namespace particle {
namespace effects {
/**
 * @ingroup particleEffects
 */
class CompositeEffect: public ParticleEffect {
 private:
	std::vector<ParticleEffectPtr> m_childEffects;

 public:
	explicit CompositeEffect(const std::string& name);

	bool processSource(ParticleSource* source) override;

	void parseValues(bool nocreate) override;

	void pageIn() override;

	EffectType getType() const override { return EffectType::Composite; }

	const std::vector<ParticleEffectPtr>& getEffects() const { return m_childEffects; }

	void addEffect(ParticleEffectPtr effect);
};
}
}

#endif // COMPOSIE_EFFECT_H
