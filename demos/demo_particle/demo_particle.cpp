/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifndef _ANDROID
#ifndef _WINRT
#define RESOURCE_PATH "../media/"
#else
#define RESOURCE_PATH "media/"
#endif
#else
#define RESOURCE_PATH "./"
#endif

#include <math.h>

#include <april/april.h>
#include <april/main.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/UpdateDelegate.h>
#include <april/Window.h>
#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Affectors.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/Space.h>
#include <aprilparticle/System.h>
#include <aprilparticle/Texture.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#define AFFECTOR_FORCE_1 "force1"
#define AFFECTOR_FORCE_2 "force2"
#define AFFECTOR_FORCE_3 "force3"
#define AFFECTOR_FORCE_4 "force4"

grect drawRect(0.0f, 0.0f, 800.0f, 600.0f);
grect viewport = drawRect;

april::Texture* redParticle = NULL;
april::Texture* greenParticle = NULL;
april::Texture* blueParticle = NULL;

aprilparticle::System* flame = NULL;
aprilparticle::System* bubbles = NULL;
aprilparticle::System* vortex = NULL;
aprilparticle::System* rain = NULL;
aprilparticle::System* quazar = NULL;
aprilparticle::System* milkyWay = NULL;
aprilparticle::System* twirl = NULL;

april::ColoredVertex grid[44];

void setupGrid(float spacing)
{
	int i = 0;
	for (float s = -5 * spacing; s <= 5 * spacing; i++, s += spacing)
	{
		grid[i * 4 + 0].set(-5 * spacing, 0.0f, s);
		grid[i * 4 + 0].color = 0xFFFFFFFF;
		grid[i * 4 + 1].set(5 * spacing, 0.0f, s);
		grid[i * 4 + 1].color = 0xFFFFFFFF;
		grid[i * 4 + 2].set(s, 0.0f, -5 * spacing);
		grid[i * 4 + 2].color = 0xFFFFFFFF;
		grid[i * 4 + 3].set(s, 0.0f, 5 * spacing);
		grid[i * 4 + 3].color = 0xFFFFFFFF;
	}
}

void drawGrid()
{
	april::rendersys->render(april::RO_LINE_LIST, grid, 44);
}

class UpdateDelegate : public april::UpdateDelegate
{
public:
	UpdateDelegate() : angle(0.0f), count(0)
	{
	}

	bool onUpdate(float timeSinceLastFrame)
	{
		april::rendersys->clear(true, false);
		this->angle += timeSinceLastFrame * 90.0f;
    
		if (hmodf(this->angle - timeSinceLastFrame * 90.0f, 90.0f) > hmodf(this->angle, 90.0f))
		{
			this->counts.clear();
			this->counts += flame->getParticleCount();
			this->counts += bubbles->getParticleCount();
			this->counts += vortex->getParticleCount();
			this->counts += rain->getParticleCount();
			this->counts += quazar->getParticleCount();
			this->counts += milkyWay->getParticleCount();
			this->counts += twirl->getParticleCount();
			this->count = 0;
			foreach (int, it, this->counts)
			{
				this->count += (*it);
			}
			this->counts += this->count;
			printf("Particles: %s\n", this->counts.cast<hstr>().join(" ").c_str());
		}
    
		april::rendersys->setPerspective(60.0f, 1 / drawRect.getAspect(), 0.1f, 100.0f);
	
		gvec3 pos(0.0f, 18.0f, 25.0f);
		gmat3 rot;
		rot.setRotation3D(0.0f, 1.0f, 0.0f, this->angle * 0.2f);
		pos = rot * pos;
		april::rendersys->lookAt(pos, gvec3(0.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
		drawGrid();
		
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_1)->setDirection(gvec3(sin(this->angle * 0.06666667f), 0.0f, cos(this->angle * 0.03333333f)) * 10.0f);
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_2)->setDirection(gvec3(sin(this->angle * 0.06666667f), sin(this->angle * 0.03333333f), 0.0f) * 10.0f);
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_3)->setDirection(gvec3(cos(this->angle * 0.03333333f), 0.0f, sin(this->angle * 0.06666667f)) * 10.0f);
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_4)->setDirection(gvec3(sin(this->angle * 0.03333333f), sin(this->angle * 0.06666667f), 0.0f) * 10.0f);
	
		flame->update(timeSinceLastFrame);
		bubbles->update(timeSinceLastFrame);
		vortex->update(timeSinceLastFrame);
		rain->update(timeSinceLastFrame);
		quazar->update(timeSinceLastFrame);
		milkyWay->update(timeSinceLastFrame);
		twirl->update(timeSinceLastFrame);
		
		flame->draw(pos);
		bubbles->draw(pos);
		vortex->draw(pos);
		rain->draw(pos);
		quazar->draw(pos);
		milkyWay->draw(pos);
		twirl->draw(pos);
	
		return true;
	}

protected:
	float angle;
	harray<int> counts;
	int count;

};

void setupFlame()
{
	flame = new aprilparticle::System();
	// spaces
	aprilparticle::Space* space = new aprilparticle::Space();
	flame->registerSpace(space);
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	space->registerEmitter(emitter);
	// textures
	aprilparticle::Texture* fire = aprilparticle::loadTexture(RESOURCE_PATH "fire_particle");
	flame->registerTexture(fire);
	emitter->setTexture(fire->getTexture());
	// affectors
	aprilparticle::Affectors::ColorChangerTimed* colorChanger = new aprilparticle::Affectors::ColorChangerTimed();
	flame->registerAffector(colorChanger);
	space->addAffector(colorChanger);
	aprilparticle::Affectors::LinearForce* linearForce = new aprilparticle::Affectors::LinearForce(gvec3(0.0f, 0.5f, 0.0f));
	flame->registerAffector(linearForce);
	space->addAffector(linearForce);
	aprilparticle::Affectors::Rotator* rotator = new aprilparticle::Affectors::Rotator(50.0f);
	flame->registerAffector(rotator);
	space->addAffector(rotator);
	aprilparticle::Affectors::Scaler* scaler = new aprilparticle::Affectors::Scaler(1.0f, 0.5f);
	flame->registerAffector(scaler);
	space->addAffector(scaler);

	space->setPosition(-10.0f, 0.0f, 0.0f);
	space->setPreUpdate(5.0f);
	
	emitter->setType(aprilparticle::Emitter::HollowSphere);
	emitter->setDimensions(2.0f, 4.0f, 2.0f);
	emitter->setBlendMode(april::BM_ADD);
	emitter->setEmissionRate(32.0f);
	emitter->setLimit(128);
	emitter->setLife(4.0f);
	emitter->setDirection(gvec3(0.0f, 0.5f, 0.0f));
	emitter->setSizeRange(gvec2(2.4f, 2.4f), gvec2(4.4f, 4.4f));
	emitter->setAngleRange(0.0f, 360.0f);

	hmap<float, april::Color> timings;
	timings[0.0f] = april::Color(0xFF000000);
	timings[0.1f] = april::Color(0xFF6432F3);
	timings[0.4f] = april::Color(0xFF7F3FAF);
	timings[0.75f] = april::Color(0x7F7F7FAF);
	timings[1.0f] = april::Color(0x7F7F7F00);
	colorChanger->setTimings(timings);
}

void setupBubbles()
{
	bubbles = new aprilparticle::System();
	// spaces
	aprilparticle::Space* space = new aprilparticle::Space();
	bubbles->registerSpace(space);
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	space->registerEmitter(emitter);
	// textures
	aprilparticle::Texture* bubble = aprilparticle::loadTexture(RESOURCE_PATH "bubble");
	bubbles->registerTexture(bubble);
	emitter->setTexture(bubble->getTexture());
	// affectors
	aprilparticle::Affectors::ColorChangerTimed* colorChanger = new aprilparticle::Affectors::ColorChangerTimed();
	bubbles->registerAffector(colorChanger);
	space->addAffector(colorChanger);
	aprilparticle::Affectors::ResizerTimed* resizer = new aprilparticle::Affectors::ResizerTimed();
	bubbles->registerAffector(resizer);
	space->addAffector(resizer);
	aprilparticle::Affectors::ForceField* forceField = new aprilparticle::Affectors::ForceField(gvec3(0.0f, 1.0f, 0.0f), 10.0f, gvec3(0.0f, 5.0f, 0.0f));
	bubbles->registerAffector(forceField);
	space->addAffector(forceField);

	emitter->setType(aprilparticle::Emitter::Cylinder);
	emitter->setDimensions(5.0f, 4.0f, 5.0f);
	emitter->setBlendMode(april::BM_DEFAULT);
	emitter->setEmissionRate(50.0f);
	emitter->setLimit(300);
	emitter->setLife(6.0f);
	emitter->setDuration(2.0f);
	emitter->setDelay(1.0f);
	emitter->setLoopDelay(2.0f);
	emitter->setLoops(3);
	emitter->setScaleRange(0.4f, 0.8f);

	hmap<float, april::Color> timings1;
	timings1[0.99f] = april::Color(0xFFFFFFFF);
	timings1[1.0f] = april::Color(0xFFFFFF00);
	colorChanger->setTimings(timings1);

	hmap<float, gvec2> timings2;
	timings2[0.0f] = gvec2(1.0f, 1.0f);
	timings2[0.05f] = gvec2(1.0f, 1.6f);
	timings2[0.1f] = gvec2(1.6f, 1.0f);
	timings2[0.15f] = gvec2(1.0f, 1.4f);
	timings2[0.2f] = gvec2(1.4f, 1.0f);
	timings2[0.25f] = gvec2(1.0f, 1.2f);
	timings2[0.3f] = gvec2(1.2f, 1.0f);
	timings2[0.35f] = gvec2(1.0f, 1.0f);
	timings2[0.99f] = gvec2(1.0f, 1.0f);
	timings2[1.0f] = gvec2(1.75f, 1.75f);
	resizer->setTimings(timings2);
}

void setupVortex()
{
	vortex = new aprilparticle::System();
	// spaces
	aprilparticle::Space* space = new aprilparticle::Space();
	bubbles->registerSpace(space);
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	space->registerEmitter(emitter);
	// textures
	emitter->setTexture(redParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::Revolutor* revolutor = new aprilparticle::Affectors::Revolutor(gvec3(0.0f, 0.0f, 0.0f), 8.0f, gvec3(0.0f, 1.0f, 0.0f), 0.2f, true);
	vortex->registerAffector(revolutor);
	space->addAffector(revolutor);
	aprilparticle::Affectors::Attractor* attractor = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 0.0f), 4.0f, 1.0f, 2.0f);
	vortex->registerAffector(attractor);
	space->addAffector(attractor);
	
	space->setPosition(10.0f, 0.0f, 0.0f);

	emitter->setType(aprilparticle::Emitter::Sphere);
	emitter->setDimensions(8.0f, 0.0f, 8.0f);
	emitter->setBlendMode(april::BM_ADD);
	emitter->setEmissionRate(200.0f);
	emitter->setLimit(800);
	emitter->setLife(2.0f);
	emitter->setScale(0.4f);
}

void setupRain()
{
	rain = new aprilparticle::System();
	// spaces
	aprilparticle::Space* space = new aprilparticle::Space();
	bubbles->registerSpace(space);
	// emitters
	aprilparticle::Emitter* emitter = new aprilparticle::Emitter();
	space->registerEmitter(emitter);
	// textures
	aprilparticle::Texture* rainDrop = aprilparticle::loadTexture(RESOURCE_PATH "rain_drop");
	rain->registerTexture(rainDrop);
	emitter->setTexture(rainDrop->getTexture());
	// making the emitter responsible for these affectors
	aprilparticle::Affectors::LinearForce* gravity = new aprilparticle::Affectors::LinearForce(gvec3(0.0f, -0.5f, 0.0f));
	rain->registerAffector(gravity);
	space->addAffector(gravity);

	space->setPosition(-10.0f, -10.0f, 6.0f);

	emitter->setType(aprilparticle::Emitter::Box);
	emitter->setDimensions(6.0f, 0.0f, 6.0f);
	emitter->setBlendMode(april::BM_ADD);
	emitter->setEmissionRate(60.0f);
	emitter->setLimit(240);
	emitter->setLife(4.0f);
	emitter->setDirection(gvec3(0.0f, -0.5f, 0.0f));
	emitter->setSizeRange(gvec2(0.1f, 0.1f), gvec2(0.4f, 0.4f));
}

void setupQuazar()
{
	quazar = new aprilparticle::System();
	// spaces
	aprilparticle::Space* discSpace = new aprilparticle::Space();
	quazar->registerSpace(discSpace);
	aprilparticle::Space* jetTopSpace = new aprilparticle::Space();
	quazar->registerSpace(jetTopSpace);
	aprilparticle::Space* jetBottomSpace = new aprilparticle::Space();
	quazar->registerSpace(jetBottomSpace);
	// emitters
	aprilparticle::Emitter* discEmitter = new aprilparticle::Emitter();
	discSpace->registerEmitter(discEmitter);
	aprilparticle::Emitter* jetTopEmitter = new aprilparticle::Emitter();
	jetTopSpace->registerEmitter(jetTopEmitter);
	aprilparticle::Emitter* jetBottomEmitter = new aprilparticle::Emitter();
	jetBottomSpace->registerEmitter(jetBottomEmitter);
	// textures
	discEmitter->setTexture(greenParticle); // texture is shared among several particle systems
	jetTopEmitter->setTexture(blueParticle); // texture is shared among several particle systems
	jetBottomEmitter->setTexture(blueParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::Attractor* gravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 0.0f), 4.0f, 1.0f, 2.0f);
	quazar->registerAffector(gravity);
	discSpace->addAffector(gravity);
	aprilparticle::Affectors::Revolutor* spin = new aprilparticle::Affectors::Revolutor(gvec3(0.0f, 0.0f, 0.0f), 8.0f, gvec3(1.0f, 0.0f, 0.0f), 0.2f, true);
	quazar->registerAffector(spin);
	discSpace->addAffector(spin);
	aprilparticle::Affectors::LinearForce* top = new aprilparticle::Affectors::LinearForce(gvec3(1.0f, 0.0f, 0.0f));
	quazar->registerAffector(top);
	jetTopSpace->addAffector(top);
	aprilparticle::Affectors::Attractor* attractorTop = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 0.0f), 15.0f, 4.0f, 2.0f);
	quazar->registerAffector(attractorTop);
	jetTopSpace->addAffector(attractorTop);
	aprilparticle::Affectors::LinearForce* bottom = new aprilparticle::Affectors::LinearForce(gvec3(-1.0f, 0.0f, 0.0f));
	quazar->registerAffector(bottom);
	jetBottomSpace->addAffector(bottom);
	aprilparticle::Affectors::Attractor* attractorBottom = new aprilparticle::Affectors::Attractor(gvec3(-10.0f, 0.0f, 0.0f), 15.0f, 4.0f, 2.0f);
	quazar->registerAffector(attractorBottom);
	jetBottomSpace->addAffector(attractorBottom);
	aprilparticle::Affectors::Attractor* inverseGravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 0.0f), 6.0f, -4.0f, 2.0f);
	quazar->registerAffector(inverseGravity);
	jetTopSpace->addAffector(inverseGravity); // affector is shared among several emitters
	jetBottomSpace->addAffector(inverseGravity); // affector is shared among several emitters

	discSpace->setPosition(0.0f, 0.0f, 10.0f);
	jetTopSpace->setPosition(0.0f, 0.0f, 10.0f);
	jetBottomSpace->setPosition(0.0f, 0.0f, 10.0f);

	discEmitter->setType(aprilparticle::Emitter::HollowSphere);
	discEmitter->setDimensions(1.0f, 6.0f, 6.0f);
	discEmitter->setBlendMode(april::BM_ADD);
	discEmitter->setEmissionRate(120.0f);
	discEmitter->setLimit(360);
	discEmitter->setLife(3.0f);
	discEmitter->setScale(0.3f);
	
	jetTopEmitter->setType(aprilparticle::Emitter::Sphere);
	jetTopEmitter->setPosition(0.5f, 0.0f, 0.0f);
	jetTopEmitter->setDimensions(1.0f, 3.0f, 3.0f);
	jetTopEmitter->setBlendMode(april::BM_ADD);
	jetTopEmitter->setEmissionRate(100.0f);
	jetTopEmitter->setLimit(300);
	jetTopEmitter->setLife(3.0f);
	jetTopEmitter->setScale(0.3f);
	
	jetBottomEmitter->setType(aprilparticle::Emitter::Sphere);
	jetBottomEmitter->setPosition(-0.5f, 0.0f, 0.0f);
	jetBottomEmitter->setDimensions(1.0f, 3.0f, 3.0f);
	jetBottomEmitter->setBlendMode(april::BM_ADD);
	jetBottomEmitter->setEmissionRate(100.0f);
	jetBottomEmitter->setLimit(300);
	jetBottomEmitter->setLife(3.0f);
	jetBottomEmitter->setScale(0.3f);
}

void setupMilkyWay()
{
	milkyWay = new aprilparticle::System();
	// spaces
	aprilparticle::Space* space = new aprilparticle::Space();
	milkyWay->registerSpace(space);
	// emitters
	aprilparticle::Emitter* emitter1 = new aprilparticle::Emitter();
	space->registerEmitter(emitter1);
	aprilparticle::Emitter* emitter2 = new aprilparticle::Emitter();
	space->registerEmitter(emitter2);
	aprilparticle::Emitter* emitter3 = new aprilparticle::Emitter();
	space->registerEmitter(emitter3);
	aprilparticle::Emitter* emitter4 = new aprilparticle::Emitter();
	space->registerEmitter(emitter4);
	// textures
	emitter1->setTexture(blueParticle); // texture is shared among several particle systems
	emitter2->setTexture(blueParticle); // texture is shared among several particle systems
	emitter3->setTexture(blueParticle); // texture is shared among several particle systems
	emitter4->setTexture(blueParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::Revolutor* evolutor = new aprilparticle::Affectors::Revolutor(gvec3(0.0f, 0.0f, 0.0f), 8.0f, gvec3(0.0f, 1.0f, 0.0f), 0.4f, false);
	milkyWay->registerAffector(evolutor);
	space->addAffector(evolutor);
	aprilparticle::Affectors::Attractor* attractor = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 0.0f), 8.0f, 5.0f, 2.0f);
	milkyWay->registerAffector(attractor);
	space->addAffector(attractor);

	space->setPosition(-10.0f, 0.0f, -10.0f);

	emitter1->setType(aprilparticle::Emitter::Sphere);
	emitter1->setPosition(4.0f, 0.0f, 0.0f);
	emitter1->setDimensions(2.0f, 0.2f, 2.0f);
	emitter1->setBlendMode(april::BM_ADD);
	emitter1->setEmissionRate(200.0f);
	emitter1->setLimit(400);
	emitter1->setLife(2.0f);
	emitter1->setScale(0.3f);

	emitter2->setType(aprilparticle::Emitter::Sphere);
	emitter2->setPosition(-4.0f, 0.0f, 0.0f);
	emitter2->setDimensions(2.0f, 0.2f, 2.0f);
	emitter2->setBlendMode(april::BM_ADD);
	emitter2->setEmissionRate(200.0f);
	emitter2->setLimit(400);
	emitter2->setLife(2.0f);
	emitter2->setScale(0.3f);

	emitter3->setType(aprilparticle::Emitter::Sphere);
	emitter3->setPosition(0.0f, 0.0f, 4.0f);
	emitter3->setDimensions(0.5f, 0.1f, 0.5f);
	emitter3->setBlendMode(april::BM_ADD);
	emitter3->setEmissionRate(50.0f);
	emitter3->setLimit(75);
	emitter3->setLife(1.5f);
	emitter3->setScale(0.25f);

	emitter4->setType(aprilparticle::Emitter::Sphere);
	emitter4->setPosition(0.0f, 0.0f, -4.0f);
	emitter4->setDimensions(0.5f, 0.1f, 0.5f);
	emitter4->setBlendMode(april::BM_ADD);
	emitter4->setEmissionRate(50.0f);
	emitter4->setLimit(75);
	emitter4->setLife(1.5f);
	emitter4->setScale(0.25f);
}

void setupTwirl()
{
	twirl = new aprilparticle::System();
	// spaces
	aprilparticle::Space* spaceLeft = new aprilparticle::Space();
	twirl->registerSpace(spaceLeft);
	aprilparticle::Space* spaceRight = new aprilparticle::Space();
	twirl->registerSpace(spaceRight);
	// emitters
	aprilparticle::Emitter* left = new aprilparticle::Emitter();
	spaceLeft->registerEmitter(left);
	aprilparticle::Emitter* right = new aprilparticle::Emitter();
	spaceRight->registerEmitter(right);
	// textures
	left->setTexture(redParticle); // texture is shared among several particle systems
	right->setTexture(greenParticle); // texture is shared among several particle systems
	// affectors
	aprilparticle::Affectors::Attractor* gravity1 = new aprilparticle::Affectors::Attractor(gvec3(-10.0f, 0.0f, 0.0f), 15.0f, 4.0f, 2.0f);
	twirl->registerAffector(gravity1);
	spaceLeft->addAffector(gravity1); // affector is shared among several emitters
	spaceRight->addAffector(gravity1); // affector is shared among several emitters
	aprilparticle::Affectors::Attractor* gravity2 = new aprilparticle::Affectors::Attractor(gvec3(-5.0f, 0.0f, 5.0f), 25.0f, 6.0f, 2.0f);
	twirl->registerAffector(gravity2);
	spaceLeft->addAffector(gravity2); // affector is shared among several emitters
	spaceRight->addAffector(gravity2); // affector is shared among several emitters
	aprilparticle::Affectors::Attractor* gravity3 = new aprilparticle::Affectors::Attractor(gvec3(-5.0f, 0.0f, -5.0f), 25.0f, 6.0f, 2.0f);
	twirl->registerAffector(gravity3);
	spaceLeft->addAffector(gravity3); // affector is shared among several emitters
	spaceRight->addAffector(gravity3); // affector is shared among several emitters
	// affectors that are being manipulated
	aprilparticle::Affectors::ForceField* force1 = new aprilparticle::Affectors::ForceField(gvec3(0.0f, 0.0f, 0.0f), 6.0f, gvec3(0.0f, 0.0f, 0.0f), AFFECTOR_FORCE_1);
	twirl->registerAffector(force1);
	spaceLeft->addAffector(force1);
	aprilparticle::Affectors::ForceField* force2 = new aprilparticle::Affectors::ForceField(gvec3(0.0f, 4.0f, 0.0f), 6.0f, gvec3(0.0f, 0.0f, 0.0f), AFFECTOR_FORCE_2);
	twirl->registerAffector(force2);
	spaceLeft->addAffector(force2);
	aprilparticle::Affectors::ForceField* force3 = new aprilparticle::Affectors::ForceField(gvec3(0.0f, 0.0f, 0.0f), 6.0f, gvec3(0.0f, 0.0f, 0.0f), AFFECTOR_FORCE_3);
	twirl->registerAffector(force3);
	spaceRight->addAffector(force3);
	aprilparticle::Affectors::ForceField* force4 = new aprilparticle::Affectors::ForceField(gvec3(0.0f, 4.0f, 0.0f), 6.0f, gvec3(0.0f, 0.0f, 0.0f), AFFECTOR_FORCE_4);
	twirl->registerAffector(force4);
	spaceRight->addAffector(force4);

	spaceLeft->setPosition(10.0f, 0.0f, -10.0f);
	spaceRight->setPosition(10.0f, 0.0f, -10.0f);

	left->setType(aprilparticle::Emitter::Point);
	left->setBlendMode(april::BM_ADD);
	left->setEmissionRate(100.0f);
	left->setLimit(600);
	left->setLife(6.0f);
	left->setDirection(gvec3(-0.5f, 0.0f, 0.0f));
	left->setScaleRange(0.3f, 0.5f);
	
	right->setType(aprilparticle::Emitter::Point);
	right->setBlendMode(april::BM_ADD);
	right->setEmissionRate(100.0f);
	right->setLimit(600);
	right->setLife(6.0f);
	right->setDirection(gvec3(-0.5f, 0.0f, 0.0f));
	right->setScaleRange(0.3f, 0.5f);
}

static UpdateDelegate* updateDelegate = NULL;

void april_init(const harray<hstr>& args)
{
#ifdef __APPLE__
	// On MacOSX, the current working directory is not set by
	// the Finder, since you are expected to use Core Foundation
	// or ObjC APIs to find files. 
	// So, when porting you probably want to set the current working
	// directory to something sane (e.g. .../Resources/ in the app
	// bundle).
	// In this case, we set it to parent of the .app bundle.
	{

		CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		// let's hope chdir() will be happy with utf8 encoding
		const char* cpath = CFStringGetCStringPtr(path, kCFStringEncodingUTF8);
		char* cpath_alloc = NULL;
		if (cpath == NULL)
		{
			// CFStringGetCStringPtr is allowed to return NULL. bummer.
			// we need to use CFStringGetCString instead.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			CFStringGetCString(path, cpath_alloc, CFStringGetLength(path) + 1, kCFStringEncodingUTF8);
		}
		else
		{
			// even though it didn't return NULL, we still want to slice off bundle name.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			strcpy(cpath_alloc, cpath);
		}
		// just in case / is appended to .app path for some reason
		if (cpath_alloc[CFStringGetLength(path) - 1] == '/')
		{
			cpath_alloc[CFStringGetLength(path) - 1] = 0;
		}
		// replace pre-.app / with a null character, thus
		// cutting off .app's name and getting parent of .app.
		strrchr(cpath_alloc, '/')[0] = 0;
		// change current dir using posix api
		chdir(cpath_alloc);
		free(cpath_alloc); // even if null, still ok
		CFRelease(path);
		CFRelease(url);
	}
#endif
	updateDelegate = new UpdateDelegate();
#if defined(_ANDROID) || defined(_IOS)
	drawRect.setSize(april::getSystemInfo().displayResolution);
#endif
	april::init(april::RS_DEFAULT, april::WS_DEFAULT);
	april::createRenderSystem();
	april::createWindow((int)drawRect.w, (int)drawRect.h, false, "AprilParticle Demo");
	aprilparticle::init();
	april::window->setUpdateDelegate(updateDelegate);
	setupGrid(2.0f);
	// textures used by more than one system
	redParticle = april::rendersys->createTextureFromResource(RESOURCE_PATH "red_particle");
	greenParticle = april::rendersys->createTextureFromResource(RESOURCE_PATH "green_particle");
	blueParticle = april::rendersys->createTextureFromResource(RESOURCE_PATH "blue_particle");
	// setting up every system
	setupFlame();
	setupBubbles();
	setupVortex();
	setupRain();
	setupQuazar();
	setupMilkyWay();
	setupTwirl();
}

void april_destroy()
{
	delete flame;
	delete bubbles;
	delete vortex;
	delete rain;
	delete quazar;
	delete milkyWay;
	delete twirl;
	delete redParticle;
	delete greenParticle;
	delete blueParticle;
	aprilparticle::destroy();
	april::destroy();
	delete updateDelegate;
	updateDelegate = NULL;
}

