#include "Assignment.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "VectorDrawer.h"
#include "Utils.h"
#include "FountainEffect.h"
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

	fire = make_shared<FountainEffect>(500);
	fire->position = glm::vec3(20, 10, 0);
	fire->diffuse = glm::vec3(0,1,1);
	Attach(fire);

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
	
	//making walls and buildings
	make->CreateWall(glm::vec3(10,10,10), 1,5, 1);
	make->CreateWall(glm::vec3(40,10,10), 1,5, 10);
	make->CreateWall(glm::vec3(70,10,10), 1,5, 25);
	make->CreateWall(glm::vec3(70,10,200), 10,4, 25);
	make->CreateBuilding(glm::vec3(100,10,100), 3,3,5,10);
	make->CreateWall(glm::vec3(-20,3.8,63), 5,5,2);
	make->CreateWall(glm::vec3(-60,3.8,73), 5,5,10);
	make->CreateWall(glm::vec3(-90,3.8,83), 5,5,100);

	//make->CreateBuilding(glm::vec3(100,10,10), 3,3,5,25);

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

	//Press up to give car torgue
	if (keyState[SDL_SCANCODE_UP])
	{
		make->MoveCarForward(3000.0f);
		PrintText("Up");
		
	}
	//make it snow
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

	PrintText("Press R to spawn Car");
	PrintText("Press Right Mouse to force push");
	PrintText("Press Space to grab an object");
	PrintText("Press E to spawn doll");
	PrintText("Press Q to fire ball");

	/*stringstream car;
	stringstream push;
	stringstream grab;
	stringstream doll;
	stringstream ball;
	car << "Press T to spawn Car";
	push << "Press Right Mouse to force push";
	grab << "Press Space to grab an object";
	doll << "Press R to spawn doll";
	ball << "Press Q to fire ball";
	game->PrintText(car.str());
	game->PrintText(push.str());
	game->PrintText(grab.str());
	game->PrintText(doll.str());
	game->PrintText(ball.str());*/

	dynamicsWorld->stepSimulation(timeDelta,100);
	Game::Update(timeDelta);
}

void Assignment::Cleanup()
{
	Game::Cleanup();
}
