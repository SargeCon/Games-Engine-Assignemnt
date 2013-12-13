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
	make = NULL;
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
    dynamicsWorld->setGravity(btVector3(0,-10,0));

	make = make_shared<Create>(dynamicsWorld);
	
	make->CreateWall(glm::vec3(10,10,10), 3,5, 25);
	make->CreateWall(glm::vec3(40,10,10), 3,5, 100);
	make->CreateWall(glm::vec3(70,10,10), 3,5, 500);

	//physicsFactory2 = make_shared<Create>(dynamicsWorld);
	make->CreateCameraPhysics();
	make->CreateGroundPhysics();

	if (!Game::Initialise()) {
		return false;
	}

	
	camera->GetController()->position = glm::vec3(0,10, 20);
	
	return true;
}



btScalar maxMotorImpulse = 1400.f;
bool checksnow = false;


void Assignment::Update(float timeDelta)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();
	Game * game = Game::Instance();

	float timeToPass = 1.0f / f;

	//spawn a car when space is pressed
	if (keyState[SDL_SCANCODE_T] && (e > timeToPass))
	{
		make->CreateCar(glm::vec3(100,0,50), 100, 25);
		e = 0.0f;
	}
	else
	{
		e += timeDelta;
	}


	if (keyState[SDL_SCANCODE_UP])
	{
		make->MoveCarForward(1000.0f);
		PrintText("Up");
		
	}
	
	if (keyState[SDL_SCANCODE_R] && (e > timeToPass))
	{

		;
		make->CreateDoll(glm::vec3(20.0f, 20.0f, 0.0f));
		e = 0.0f;
	}
	else
	{
		e += timeDelta;
	}
	
	if (keyState[SDL_SCANCODE_F])
	{

		snow = make_shared<SnowEffect>();
		snow->position = glm::vec3(-30, 0, 0);
		snow->diffuse = glm::vec3(1,1, 0);
		Attach(snow);
		
		if(checksnow == false)
		{
			snow->Initialise();
			checksnow = true;
		}
	}


	dynamicsWorld->stepSimulation(timeDelta,100);
	Game::Update(timeDelta);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}
