#include "GravGun.h"
#include "Game.h"
#include "Box.h"
#include <iostream>
#include <sstream>
#include "Utils.h"
using namespace BGE;

GravGun::GravGun(Create * physicsFactory):PhysicsController()
{

	pickedUp = NULL;
	tag = "Physics Camera";
	this->physicsFactory = physicsFactory;
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

	string what = "Nothing";
	// Handle the gravity gun
	if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(3))
	{
		float dist = 1000.0f;
		if (pickedUp == NULL)
		{		
			btVector3 rayFrom = GLToBtVector(parent->position + (parent->look * 4.0f)); // Has to be some distance in front of the camera otherwise it will collide with the camera all the time
			btVector3 rayTo = GLToBtVector(parent->position + (parent->look * dist));

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
			physicsFactory->dynamicsWorld->rayTest(rayFrom, rayTo, rayCallback);
			
			if (rayCallback.hasHit())
			{
				pickedUp = reinterpret_cast<PhysicsController*>(rayCallback.m_collisionObject->getUserPointer());
				if (pickedUp->parent == game->GetGround().get())
				{
					pickedUp = NULL;
				}
			}
		}
		if (pickedUp != NULL)
		{
			float powerfactor = 4.0f; // Higher values causes the targets moving faster to the holding point.
            float maxVel = 3.0f;      // Lower values prevent objects flying through walls.
			float holdDist = 30.0f;

            // Calculate the hold point in front of the camera
			glm::vec3 holdPos = parent->position + (parent->look * holdDist);

            glm::vec3 v = holdPos - pickedUp->position; // direction to move the Target
            v *= powerfactor; // powerfactor of the GravityGun

            if (v.length() > maxVel)
            {
                // if the correction-velocity is bigger than maximum
				v = glm::normalize(v);
                v *= maxVel; // just set correction-velocity to the maximum
            }
			pickedUp->rigidBody->setLinearVelocity(GLToBtVector(v));    
			pickedUp->rigidBody->activate();		
			what = pickedUp->tag;	
		}
	}
	else
	{    
		pickedUp = NULL;
	}
	stringstream ss;
	ss << "Picked up: " << what;
	game->PrintText(ss.str());
}

