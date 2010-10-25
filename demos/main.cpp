#include "april/RenderSystem.h"
#include "aprilparticle/ParticleEmitter.h"
#include "aprilparticle/DirectionalForceAffector.h"
#include "april/Timer.h"
#include "math.h"

#include <iostream>

April::ParticleEmitter *emiter, *plasma;

bool render(float time_increase)
{
	April::rendersys->clear(true, false);
	static float angle=0;
	angle+=time_increase*90;
    
	April::rendersys->setPerspective(60,800/600.,0.1f,100.0f);
	//April::rendersys->setTexture(tex);
	
	plasma->position.x = cos(angle/8)/2;
	plasma->position.z = sin(angle/8)/2;
	
	//emiter->update(time_increase);
	//emiter->draw();
	
	plasma->update(time_increase);
	plasma->draw();
	
	April::rendersys->lookAt(gvec3(2,2,-5),gtypes::Vector3(0,0,0),gtypes::Vector3(0,1,0));
	//April::rendersys->rotate(angle,0,1,0);
    
	return true;
}

int main()
{
	April::init("April",800,600,0,"April: 3D Demo");
	April::rendersys->registerUpdateCallback(render);
	
	emiter = new April::ParticleEmitter(2.0, 1.0, 0.8, 0.05);
	emiter->setTexture("../media/partikl.png");
	plasma = new April::ParticleEmitter(3.0, 0.2, 0.01, 0.05);
	plasma->setTexture("../media/plasma_ball.png");
	
	April::DirectionalForceAffector force1(0.02, 2.2, gvec3(0,0,0), gvec3(0,1,0));
	April::DirectionalForceAffector force2(0.4, 2.2, gvec3(0,1,0), gvec3(-1,0,0));
	
	//emiter->addAffctor(&force1);
	//emiter->addAffctor(&force2);
	plasma->addAffctor(&force1);
	//plasma->addAffctor(&force2);

	//tex = April::rendersys->loadTexture("../media/texture.jpg");
    
	April::rendersys->enterMainLoop();
    
	April::destroy();
	delete emiter;
	delete plasma;
	return 0;
}
