#ifndef GENERIC_SHAPE_EFFECT_H
#define GENERIC_SHAPE_EFFECT_H
#pragma once

#include "core/error.h"
#include "globalincs/pstypes.h"
#include "particle/ParticleEffect.h"
#include "particle/ParticleManager.h"
#include "particle/util/ParticleProperties.h"
#include <particle/util/EffectTiming.h>
#include <math/RandomRange.h>
#include <parse/parselo.h>
namespace particle {
namespace effects {

	enum class ConeDirection { Incoming, Normal, Reflected, Reverse };

	/**
	 * @brief A generic particle effect with a customizable shape
	 *
	 * @ingroup particleEffects
	 */
	template <typename TShape>
	class GenericShapeEffect : public ParticleEffect {
	  private:
		util::ParticleProperties m_particleProperties;

		ConeDirection m_direction = ConeDirection::Incoming;
		::random::UniformFloatRange m_velocity;
		::random::UniformUIntRange m_particleNum;

		ParticleEffectHandle m_particleTrail = ParticleEffectHandle::invalid();

		util::EffectTiming m_timing;

		TShape m_shape;

		vec3d getNewDirection(const ParticleSource* source) const
		{
			switch (m_direction) {
			case ConeDirection::Incoming:
				return source->getOrientation()->getDirectionVector(source->getOrigin());
			case ConeDirection::Normal: {
				vec3d normal;
				if (!source->getOrientation()->getNormal(&normal)) {
					mprintf(
					    ("Effect '%s' tried to use normal direction for source without a normal!\n", m_name.c_str()));
					return source->getOrientation()->getDirectionVector(source->getOrigin());
				}

				return normal;
			}
			case ConeDirection::Reflected: {
				vec3d out = source->getOrientation()->getDirectionVector(source->getOrigin());
				vec3d normal;
				if (!source->getOrientation()->getNormal(&normal)) {
					mprintf(
					    ("Effect '%s' tried to use normal direction for source without a normal!\n", m_name.c_str()));
					return out;
				}

				// Compute reflect vector as R = V - 2*(V dot N)*N where N
				// is the normal and V is the incoming direction

				auto dot = 2 * vm_vec_dot(&out, &normal);

				vm_vec_scale(&normal, dot);
				vm_vec_sub(&out, &out, &normal);

				return out;
			}
			case ConeDirection::Reverse: {
				vec3d out = source->getOrientation()->getDirectionVector(source->getOrigin());
				vm_vec_scale(&out, -1.0f);
				return out;
			}
			default:
				core::Error(LOCATION, "Unhandled direction value!");
				return vmd_zero_vector;
			}
		}

	  public:
		explicit GenericShapeEffect(const std::string& name) : ParticleEffect(name) {}

		bool processSource(ParticleSource* source) override
		{
			if (!m_timing.continueProcessing(source)) {
				return false;
			}

			// This uses the internal features of the timing class for determining if and how many effects should be
			// triggered this frame
			util::EffectTiming::TimingState time_state;
			while (m_timing.shouldCreateEffect(source, time_state)) {
				auto num = m_particleNum.next();

				vec3d dir = getNewDirection(source);
				matrix dirMatrix;
				vm_vector_2_matrix(&dirMatrix, &dir, nullptr, nullptr);
				for (uint i = 0; i < num; ++i) {
					matrix velRotation = m_shape.getDisplacementMatrix();

					matrix rotatedVel;
					vm_matrix_x_matrix(&rotatedVel, &dirMatrix, &velRotation);

					particle_info info;

					source->getOrigin()->applyToParticleInfo(info);

					info.vel = rotatedVel.vec.fvec;
					if (TShape::scale_velocity_deviation()) {
						// Scale the vector with a random velocity sample and also multiply that with cos(angle between
						// info.vel and sourceDir) That should produce good looking directions where the maximum
						// velocity is only achieved when the particle travels directly on the normal/reflect vector
						vm_vec_scale(&info.vel, vm_vec_dot(&info.vel, &dir));
					}
					vm_vec_scale(&info.vel, m_velocity.next());

					if (m_particleTrail.isValid()) {
						auto part = m_particleProperties.createPersistentParticle(info);

						auto trailSource = ParticleManager::get()->createSource(m_particleTrail);
						trailSource.moveToParticle(part);

						trailSource.finish();
					} else {
						// We don't have a trail so we don't need a persistent particle
						m_particleProperties.createParticle(info);
					}
				}
			}

			return true;
		}

		void parseValues(bool nocreate) override;

		void initializeSource(ParticleSource& source) override { m_timing.applyToSource(&source); }

		EffectType getType() const override { return m_shape.getType(); }

		void pageIn() override { m_particleProperties.pageIn(); }
	};
} // namespace effects
} // namespace particle
#endif // GENERIC_SHAPE_EFFECT_H
