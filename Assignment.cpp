#include "Assignment.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"

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
	//physicsFactory2->CreateCameraPhysics();
	physicsFactory->CreateGroundPhysics();

	//shared_ptr<PhysicsController> box1 = physicsFactory->CreateBox(1,1,4, glm::vec3(5, 5, 0), glm::quat()); 
	head = physicsFactory->CreateSphere(2,30,glm::vec3(20, 16, 0), glm::quat());
	body = physicsFactory->CreateBox(2,6,3,20, glm::vec3(20, 10, 0), glm::quat()); 
	arm1 = physicsFactory->CreateBox(1,1,3,10, glm::vec3(20, 12, 5), glm::quat()); 
	//forearm1 = physicsFactory->CreateBox(1,1,4,10, glm::vec3(20, 12, 8), glm::quat()); 
	//arm2 = physicsFactory->CreateBox(1,1,3,10, glm::vec3(20, 12, -5), glm::quat()); 
	//forearm2 = physicsFactory->CreateBox(1,1,4,10, glm::vec3(20, 12, -8), glm::quat());
	/*leg1 = physicsFactory->CreateBox(1,1,3,15, glm::vec3(20, 10, -5), glm::quat()); 
	leg2 = physicsFactory->CreateBox(1,1,4,15, glm::vec3(20, 10, -10), glm::quat());*/

	
	btHingeConstraint * neck = new btHingeConstraint(* head->rigidBody, * body->rigidBody, btVector3(0,-1.5f,0),btVector3(0,4.5f,0), btVector3(0,1,0), btVector3(0,1,0), true);

	btPoint2PointConstraint * shoulder = new btPoint2PointConstraint(*body->rigidBody, *arm1->rigidBody, btVector3(0,1.0f,2.0f),btVector3(0,-1.0f,-2.0f));
	//btPoint2PointConstraint * elbow = new btPoint2PointConstraint(*arm1->rigidBody, *forearm1->rigidBody, btVector3(0,0,2.0f),btVector3(0,0,-2.0f));
	//btPoint2PointConstraint * shoulder2 = new btPoint2PointConstraint(*body->rigidBody, *arm2->rigidBody, btVector3(0,1.5f,-2.0f),btVector3(0,1.5f,-2.0f));
	//btPoint2PointConstraint * elbow2 = new btPoint2PointConstraint(*arm2->rigidBody, *forearm2->rigidBody, btVector3(0,0,2.0f),btVector3(0,0,-2.0f));
	dynamicsWorld->addConstraint(neck);
	dynamicsWorld->addConstraint(shoulder);
	//dynamicsWorld->addConstraint(elbow);
	//dynamicsWorld->addConstraint(shoulder2);
	//dynamicsWorld->addConstraint(elbow2);
	


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

	dynamicsWorld->stepSimulation(timeDelta,100);
	Game::Update(timeDelta);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}
