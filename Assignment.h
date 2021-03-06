#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "Create.h"
#include "GravGun.h"
#include "SnowEffect.h"
#include "GameComponent.h"
#include "FountainEffect.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	class Assignment :
		public Game
	{
	private:
		btBroadphaseInterface* broadphase;
 
		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration * collisionConfiguration;
		btCollisionDispatcher * dispatcher;
 
		// The actual physics solver
		btSequentialImpulseConstraintSolver * solver;

		float e;
		float f;

		shared_ptr<SnowEffect> snow;
		shared_ptr<FountainEffect> fire;
	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void createCar();
		
		// The world.
		btDiscreteDynamicsWorld * dynamicsWorld;
		std::shared_ptr<Create> make;

	};
}