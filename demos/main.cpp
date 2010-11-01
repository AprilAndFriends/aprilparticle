#include "april/RenderSystem.h"
#include "aprilparticle/ParticleEmitter.h"
#include "april/Timer.h"
#include "aprilutil/StaticMesh.h"
#include "math.h"

#include "gtypes/Matrix3.h"
#include "gtypes/Vector3.h"

#include <iostream>

April::ParticleEmitter *emiter;
April::Texture *tex;
April::StaticMesh *msh;

April::Affectors::DirectionalForceField force1(4.0, gvec3(0,-1,0), gvec3(0,1,0));
April::Affectors::ColorAffector colorFader(0xFF1131FF, 0x00220000);
April::Affectors::MultiColorAffector colorAffector;
April::Affectors::LinearDirectionalForce linearforce1(gvec3(0,1,0), 0.008);
April::Affectors::Rotator rotator(5.0);
April::Affectors::Attractor attractor1(gvec3(0,4,0), 8.0);
April::Affectors::Attractor attractor2(gvec3(-4,4,0), 5.0);
April::Affectors::Attractor attractor3(gvec3(0,0,-4), 5.0);
April::Affectors::Attractor attractor4(gvec3(0,4,4), 5.0);

bool render(float time_increase)
{
	April::rendersys->clear(true, false);
	static float angle=0;
	angle+=time_increase*90;
    
	April::rendersys->setPerspective(60,800/600.,0.1f,100.0f);
	
	April::rendersys->lookAt(gvec3(10,6,-8),gtypes::Vector3(0,0,0),gtypes::Vector3(0,1,0));
	April::rendersys->rotate(angle*0.2,0,1,0);
	
	//emiter->mPosition.x = cos(angle/16) * 4;
	//emiter->mPosition.z = sin(angle/16) * 4;
	
	//attractor1.mOrigin.x = cos(angle/16) * 2;
	//attractor1.mOrigin.z = sin(angle/16) * 2;
	
	
	//force1.pos = gvec3(cos(angle/32), cos(angle/16), sin(angle/32));
	
	April::rendersys->setBlendMode(April::ADD);
	
	emiter->update(time_increase);
	
	emiter->draw();
	
	//emiter->drawAffectors();
	
	/*April::rendersys->setBlendMode(April::DEFAULT);
	April::rendersys->translate(0,0,5);
	April::rendersys->setTexture(tex);
	msh->draw(April::TriangleList);
	April::rendersys->translate(0,0,-5);*/
    
	return true;
}

int main()
{
	April::init("April",640,480,0,"April: 3D Demo");
	April::rendersys->registerUpdateCallback(render);
	
	April::rendersys->setBlendMode(April::ADD);
	
	msh = new April::StaticMesh("../media/mesh.obj");
	tex = April::rendersys->loadTexture("../media/texture.jpg");
	
	emiter = new April::ParticleEmitter(3.0, 48);
	emiter->setTexture("../media/partikl.png");
	emiter->mMaxParticles = 144;
	emiter->setEmitterType(April::ET_HollowSphere);
	emiter->setEmiterVolume(1,3,1);
	
	emiter->mRandomStartAngle = true;
	emiter->mRandomStartSize = true;
	emiter->mMinSize = 1.8;
	emiter->mMaxSize = 3.2;
	
	std::map<float, unsigned int> cols;
	cols[0.0]  = 0x00FF0000;
	cols[0.1] = 0xF3FF6432;
	cols[0.65] = 0xAF3F3F3F;
	cols[1.0]  = 0x00000000;
	
	colorAffector.setColors(cols);
	
	//emiter->addAffector(&force1);
	//emiter->addAffector(&colorFader);
	emiter->addAffector(&colorAffector);
	emiter->addAffector(&linearforce1);
	emiter->addAffector(&rotator);
	emiter->addAffector(&attractor1);
	//emiter->addAffector(&attractor2);
	//emiter->addAffector(&attractor3);
	//emiter->addAffector(&attractor4);
    
	April::rendersys->enterMainLoop();
    
	April::destroy();
	delete emiter;
	delete tex;
	delete msh;
	return 0;
}
