add_library( particle STATIC
	
	particle/effects/BeamPiercingEffect.cpp
	particle/effects/BeamPiercingEffect.h
	particle/effects/CompositeEffect.cpp
	particle/effects/CompositeEffect.h
	particle/effects/ConeShape.h
	particle/effects/GenericShapeEffect.h
	particle/effects/ParticleEmitterEffect.cpp
	particle/effects/ParticleEmitterEffect.h
	particle/effects/SingleParticleEffect.cpp
	particle/effects/SingleParticleEffect.h
	particle/effects/SphereShape.h
	particle/particle.cpp
	particle/particle.h
	particle/ParticleEffect.h
	particle/ParticleManager.cpp
	particle/ParticleManager.h
	particle/ParticleSource.cpp
	particle/ParticleSource.h
	particle/ParticleSourceWrapper.cpp
	particle/ParticleSourceWrapper.h
	particle/util/EffectTiming.cpp
	particle/util/EffectTiming.h
	particle/util/ParticleProperties.cpp
	particle/util/ParticleProperties.h
)
target_include_directories(particle PUBLIC ${CMAKE_CURRENT_LIST_DIR})
TARGET_INCLUDE_DIRECTORIES(particle PRIVATE ${GENERATED_SOURCE_DIR})
set_target_properties(particle PROPERTIES FOLDER "modules")
