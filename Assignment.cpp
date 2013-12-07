#include "Assignment.h"
#include "PhysicsController.h"
#include "Sphere.h"
<<<<<<< HEAD
=======
#include "PhysicsCamera.h"
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
<<<<<<< HEAD
#include "PhysicsCamera.h"
=======
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"

using namespace BGE;

Assignment::Assignment(void)
{
<<<<<<< HEAD
	e = 10000.0f;
	f = 1.0f;
=======
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
	physicsFactory = NULL;
	dynamicsWorld = NULL;
	broadphase = NULL;
	dispatcher = NULL;
	solver = NULL;
	fullscreen = false;

<<<<<<< HEAD

=======
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
}

Assignment::~Assignment(void)
{
}

bool Assignment::Initialise() 
{
<<<<<<< HEAD

=======
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
	collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
    // The world.
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	broadphase = new btAxisSweep3(worldMin,worldMax);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

	physicsFactory = make_shared<PhysicsFactory>(dynamicsWorld);

	physicsFactory->CreateGroundPhysics();
	physicsFactory->CreateCameraPhysics();

	shared_ptr<PhysicsController> box1 = physicsFactory->CreateBox(1,1,4, glm::vec3(5, 5, 0), glm::quat()); 

	if (!Game::Initialise()) {
		return false;
	}

	camera->GetController()->position = glm::vec3(0,10, 20);
	
	return true;
}

<<<<<<< HEAD
shared_ptr<PhysicsController> wheel1;
shared_ptr<PhysicsController> wheel2;
shared_ptr<PhysicsController> wheel3;
shared_ptr<PhysicsController> wheel4;

void Assignment::Update(float timeDelta)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();
	Game * game = Game::Instance();

	float timeToPass = 1.0f / f;
	//spawn a car when space is pressed
	if (keyState[SDL_SCANCODE_F] && (e > timeToPass))
	{
		PrintFloat("elapsed ", timeToPass);
		glm::vec3 position(20,20,0);
		float width = 15;
		float height = 2;
		float length = 5;
		float wheelWidth = 1;
		float wheelRadius = 2;
		float wheelOffset = 2.0f;

		shared_ptr<PhysicsController> chassis = physicsFactory->CreateBox(width, height, length, position, glm::quat());

		
		glm::quat q =  glm::angleAxis(glm::half_pi<float>(), glm::vec3(1, 0, 0));

		glm::vec3 offset;
		btHingeConstraint * hinge;

		offset = glm::vec3(- (width / 2 - wheelRadius), 0, - (length / 2 + wheelOffset));
		wheel1= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, position + offset, q);	 
		hinge = new btHingeConstraint(* chassis->rigidBody, * wheel1->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge);

		offset = glm::vec3(+ (width / 2 - wheelRadius), 0, - (length / 2 + wheelOffset));
		wheel2= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, glm::vec3(position.x + (width / 2) - wheelRadius, position.y, position.z - (length / 2) - wheelWidth), q);
		hinge = new btHingeConstraint(* chassis->rigidBody, * wheel2->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge);

		offset = glm::vec3(- (width / 2 - wheelRadius), 0, + (length / 2 + wheelOffset));
		wheel3= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, position + offset, q);	 
		hinge = new btHingeConstraint(* chassis->rigidBody, * wheel3->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge);

		offset = glm::vec3(+ (width / 2 - wheelRadius), 0, + (length / 2 + wheelOffset));
		wheel4= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, position + offset, q);	 
		hinge = new btHingeConstraint(* chassis->rigidBody, * wheel4->rigidBody, GLToBtVector(offset),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge);

		

		e = 0.0f;
	}
	else
	{
		e += timeDelta;
	}

	if (keyState[SDL_SCANCODE_UP])
	{
		PrintText("Up");
		wheel1->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,100.0f)));
		wheel2->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,100.0f)));
		wheel3->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,100.0f)));
		wheel4->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,100.0f)));
		
	}
	if(keyState[SDL_SCANCODE_DOWN])
	{
		PrintText("Down");
		wheel1->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,-10.0f)));
		
	}

=======
void BGE::Assignment::Update(float timeDelta)
{
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
	dynamicsWorld->stepSimulation(timeDelta,100);
	Game::Update(timeDelta);
}

<<<<<<< HEAD
void Assignment::Cleanup()
=======
void BGE::Assignment::Cleanup()
>>>>>>> 5f3c7c81281629a46c97c2f81b371d2208ba04fe
{
	Game::Cleanup();
}