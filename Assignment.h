#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "Create.h"
#include "GravGun.h"
#include "GameComponent.h"
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
		void createCar();
		
		// The world.
		btDiscreteDynamicsWorld * dynamicsWorld;
		std::shared_ptr<Create> physicsFactory;
		shared_ptr<GameComponent> ship1;
		float e;
		float f;
		std::shared_ptr<PhysicsController> frontright;
		std::shared_ptr<PhysicsController> backright;
		std::shared_ptr<PhysicsController> backleft;
		std::shared_ptr<PhysicsController> frontleft;
		btHingeConstraint * hinge1;
		btHingeConstraint * hinge2;
		btHingeConstraint * hinge3;
		btHingeConstraint * hinge4;
		
	};
}