#include "BaseAffector.h"

namespace April
{

	BaseAffector::BaseAffector()
	{
	}

	BaseAffector::~BaseAffector()
	{
	}
	
	void BaseAffector::update(float k)
	{
	}
	
	void BaseAffector::assignEmitter(April::ParticleEmitter *emitter)
	{
		this->asignedEmitters.push_back(emitter);
	}

}

