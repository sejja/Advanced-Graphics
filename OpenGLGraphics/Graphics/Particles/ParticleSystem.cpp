#include<iostream>
#include"Graphics/Particles/ParticleSystem.h"
#include "ParticleSystem.h"


namespace Graphics
{
	namespace Particles
	{
		ParticleSystem::ParticleSystem()
		{
		}

		ParticleSystem::~ParticleSystem()
		{
		}

		/** Particle System update
		* 
		*/
		void ParticleSystem::Update() 
		{
			std::cout << "UNIMPLEMENTED METHOD \n";
		}

		/** Render all the particles
		*  
		*/
		void ParticleSystem::Render()
		{
			std::cout << "UNIMPLEMENTED METHOD \n";
		}



		/** Initialites a kind of particles
		* 
		*/
		void ParticleSystem::initializeParticles() 
		{
			std::cout << "UNIMPLEMENTED METHOD \n";
		}

		/** Regenerate the particles
		*
		*/
		void ParticleSystem::respawnParticle(Particle& particle) 
		{
			std::cout << "UNIMPLEMENTED METHOD \n";
		}
	}
}