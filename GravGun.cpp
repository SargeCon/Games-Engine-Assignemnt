#include "GravGun.h"
#include "Game.h"
#include "Box.h"
#include <iostream>
#include <sstream>
#include "Utils.h"
using namespace BGE;

GravGun::GravGun(Create * dWorld):PhysicsController()
{
	elapsed = 10000.0f;
	fireRate = 5.0f;
	LookForObject = NULL;
	tag = "Physics Camera";
	this->make = dWorld;
}


GravGun::~GravGun(void)
{
}

void GravGun::getWorldTransform(btTransform &worldTrans) const
{
	worldTrans.setOrigin(GLToBtVector(parent->position));
	worldTrans.setRotation(GLToBtQuat(parent->orientation));
}

void GravGun::setWorldTransform(const btTransform &worldTrans)
{
}

void GravGun::Update(float timeDelta)
{

	const Uint8 * keyState = Game::Instance()->GetKeyState();
	Game * game = Game::Instance();

	float moveSpeed = speed;
	float timeToPass = 1.0f / fireRate;
	string what = "Nothing";

	if ((keyState[SDL_SCANCODE_C]) && (elapsed > timeToPass))
	{
		glm::vec3 pos = parent->position + (parent->look * 5.0f);
		glm::quat q(RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat());
		glm::normalize(q);
		make->CreateBalloon(pos, 10);

		elapsed = 0.0f;
	}

	//Spawn in balls and fire them
	if ((keyState[SDL_SCANCODE_Q]) && (elapsed > timeToPass))
	{
		glm::vec3 pos = parent->position + (parent->look * 5.0f);
		glm::quat q(RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat());
		glm::normalize(q);
		shared_ptr<PhysicsController> ball = make->CreateSphere(2,1,pos,q);

		float force = 10000.0f;
		ball->rigidBody->applyCentralForce(GLToBtVector(parent->look) * force);
		elapsed = 0.0f;
	}

	//Spawn in ragdoll
	if ((keyState[SDL_SCANCODE_E]) && (elapsed > timeToPass))
	{
		glm::vec3 pos = parent->position + (parent->look * 20.0f);
		make->CreateDoll(pos);
		elapsed = 0.0f;
	}
	else
	{
		elapsed += timeDelta;
	}

	//When a mouse button is pressed it pushes objects away
	if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		float dist = 1000.0f;
		if (LookForObject == NULL)
		{		
			btVector3 rayFrom = GLToBtVector(parent->position + (parent->look * 4.0f)); 
			btVector3 rayTo = GLToBtVector(parent->position + (parent->look * dist));

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
			make->dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);
			//Using rayscasting to check for objects
			if (rayCallback.hasHit())
			{
				LookForObject = reinterpret_cast<PhysicsController*>(rayCallback.m_collisionObject->getUserPointer());
				if (LookForObject->parent == game->GetGround().get())
				{
					LookForObject = NULL;
				}
			}
		}
		//if there is an object detected
		if (LookForObject != NULL)
		{
			float powerfactor = 4.0f;
            float maxVel = 3.0f;      
			float holdDist = 30.0f;
			float force = 5000.0f;

			LookForObject->rigidBody->applyCentralForce(GLToBtVector(parent->look) * force);
			LookForObject->rigidBody->activate();		
			what = LookForObject->tag;	
		}
	}
	//Same as above, except it grabs objects
	if (keyState[SDL_SCANCODE_SPACE])
	{
		float dist = 1000.0f;
		if (LookForObject == NULL)
		{		
			btVector3 rayFrom = GLToBtVector(parent->position + (parent->look * 4.0f)); 
			btVector3 rayTo = GLToBtVector(parent->position + (parent->look * dist));

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
			make->dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);
			
			if (rayCallback.hasHit())
			{
				LookForObject = reinterpret_cast<PhysicsController*>(rayCallback.m_collisionObject->getUserPointer());
				if (LookForObject->parent == game->GetGround().get())
				{
					LookForObject = NULL;
				}
			}
		}
		if (LookForObject != NULL)
		{
			float powerfactor = 4.0f; 
            float maxVel = 3.0f;     
			float holdDist = 30.0f;
			

			glm::vec3 holdPos = parent->position + (parent->look * holdDist);

			glm::vec3 v = holdPos - LookForObject->position; 
            v *= powerfactor; 

            if (v.length() > maxVel)
            {
                
				 v = glm::normalize(v);
                 v *= maxVel; 
            }
			LookForObject->rigidBody->setLinearVelocity(GLToBtVector(v));
			LookForObject->rigidBody->activate();		
			what = LookForObject->tag;	
		}
	}
	else
	{    
		LookForObject = NULL;
	}

	stringstream ss;
	ss << "Picked up: " << what;
	game->PrintText(ss.str());
}

