#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "Create.h"
//#include "PhysicsFactory.h"
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
		std::shared_ptr<Create> physicsFactory;
		//std::shared_ptr<PhysicsFactory> physicsFactory2;
		btDiscreteDynamicsWorld * dynamicsWorld;
		float e;
		float f;
		std::shared_ptr<PhysicsController> frontright;
		std::shared_ptr<PhysicsController> backright;
		std::shared_ptr<PhysicsController> backleft;
		std::shared_ptr<PhysicsController> frontleft;
		btHingeConstraint * hinge1;
		btHingeConstraint * hinge2;
		btHingeConstraint * hinge3;
		std::shared_ptr<PhysicsController> body;
		std::shared_ptr<PhysicsController> arm1;
		std::shared_ptr<PhysicsController> forearm1;
		std::shared_ptr<PhysicsController> arm2;
		std::shared_ptr<PhysicsController> forearm2;
		std::shared_ptr<PhysicsController> head;
		std::shared_ptr<PhysicsController> leg1;
		std::shared_ptr<PhysicsController> leg2;

		btHingeConstraint * hinge4;
		
	};
}