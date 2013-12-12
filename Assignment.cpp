#include "Assignment.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"
#include "Content.h"

using namespace BGE;

//std::shared_ptr<PhysicsFactory> physicsFactory;

Assignment::Assignment(void)
{
	e = 10000.0f;
	f = 1.0f;
	physicsFactory = NULL;
	dynamicsWorld = NULL;
	broadphase = NULL;
	dispatcher = NULL;
	solver = NULL;
	fullscreen = false;



}

Assignment::~Assignment(void)
{
}

bool Assignment::Initialise() 
{

	ship1 = make_shared<GameComponent>();
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 90.0f, glm::vec3(0,1,0))));
	ship1->position = glm::vec3(-150, 100, -250);
	ship1->scale = glm::vec3(10, 10, 10);
	Attach(ship1);

	collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
 
    // The world.
	btVector3 worldMin(-1000,-1000,-1000);
	btVector3 worldMax(1000,1000,1000);
	broadphase = new btAxisSweep3(worldMin,worldMax);
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,0,0));

	physicsFactory = make_shared<Create>(dynamicsWorld);
	
	//physicsFactory2 = make_shared<Create>(dynamicsWorld);
	physicsFactory->CreateCameraPhysics();
	physicsFactory->CreateGroundPhysics();

	if (!Game::Initialise()) {
		return false;
	}



	camera->GetController()->position = glm::vec3(0,10, 20);
	
	return true;
}



btScalar maxMotorImpulse = 1400.f;

void Assignment::Update(float timeDelta)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();
	Game * game = Game::Instance();

	float timeToPass = 1.0f / f;

	//spawn a car when space is pressed
	if (keyState[SDL_SCANCODE_T] && (e > timeToPass))
	{
		glm::vec3 position(100,10,0);
		float width = 12;
		float height = 2;
		float length = 5;
		float chassisMass = 100;
		float wheelWidth = 1;
		float wheelRadius = 1.5f;
		float wheelMass = 25;
		float wheelOffset = 1.0f;

		shared_ptr<PhysicsController> chassis = physicsFactory->CreateBox(width, height, length, chassisMass, position, glm::quat());

		frontright= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, wheelMass, glm::vec3(106,10,5), glm::quat());

		hinge1 = new btHingeConstraint(* chassis->rigidBody, * frontright->rigidBody, btVector3(6.0f,0.0f,5.0f), btVector3(0,0,0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge1);

		backright= physicsFactory->CreateCylinder(wheelRadius, wheelWidth,wheelMass, glm::vec3(106,10,5), glm::quat());
		hinge2 = new btHingeConstraint(* chassis->rigidBody, * backright->rigidBody, btVector3(6.0f,0.0f,-5.0f),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge2);

		backleft= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, wheelMass, glm::vec3(106,10,5), glm::quat()); 
		hinge3 = new btHingeConstraint(* chassis->rigidBody, * backleft->rigidBody, btVector3(-6.0f,0.0f,-5.0f),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge3);

		frontleft= physicsFactory->CreateCylinder(wheelRadius, wheelWidth, wheelMass, glm::vec3(106,10,5), glm::quat()); 
		hinge4 = new btHingeConstraint(* chassis->rigidBody, * frontleft->rigidBody, btVector3(-6.0f,0.0f,5.0f),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		dynamicsWorld->addConstraint(hinge4);
		PrintText("Test");
		e = 0.0f;
		
	}
	else
	{
		e += timeDelta;
	}

	if (keyState[SDL_SCANCODE_UP])
	{
		
		//wheel1->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,200.0f)));
		//wheel2->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,1000.0f)));
		backright->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,1000.0f)));
		backleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,1000.0f)));

		PrintText("Up");
		
	}
	if(keyState[SDL_SCANCODE_DOWN])
	{
		PrintText("Down");
		frontright->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,-50.0f)));
		backright->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,-50.0f)));
		backleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,-50.0f)));
		frontleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,-50.0f)));
		
	}
	if(keyState[SDL_SCANCODE_LEFT])
	{
		PrintText("Left");
		frontright->rigidBody->applyTorque(GLToBtVector(glm::vec3(2000.0f,0.0f,0.0f)));
		frontleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(2000.0f,0.0f,0.0f)));
		
	}
	if(keyState[SDL_SCANCODE_RIGHT])
	{
		PrintText("Right");
		frontright->rigidBody->applyTorque(GLToBtVector(glm::vec3(-2000.0f,0.0f,0.0f)));
		frontleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(-4000.0f,0.0f,0.0f)));
		
	}
	if (keyState[SDL_SCANCODE_R] && (e > timeToPass))
	{
		physicsFactory->CreateDoll();
		e = 0.0f;
	}
	else
	{
		e += timeDelta;
	}

	dynamicsWorld->stepSimulation(timeDelta,100);
	Game::Update(timeDelta);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}
