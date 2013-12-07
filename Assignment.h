#pragma once
#include "Game.h"
#include "PhysicsController.h"
<<<<<<< HEAD

=======
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
#include "PhysicsFactory.h"
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

	public:
		Assignment(void);
		~Assignment(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		
		// The world.
		std::shared_ptr<PhysicsFactory> physicsFactory;
		btDiscreteDynamicsWorld * dynamicsWorld;
<<<<<<< HEAD
		float e;
		float f;
=======
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
	};
}