#pragma once
#include "Camera.h"
#include "PhysicsController.h"
#include <btBulletDynamicsCommon.h>
#include "Create.h"

namespace BGE

{
	class GravGun :
		public PhysicsController, public btMotionState 
	{
	private:
		PhysicsController * LookForObject;
	public:
		GravGun(Create * dWorld);
		~GravGun(void);

		//bool Initialise();
		void GravGun::Update(float timeDelta);

		void getWorldTransform(btTransform &worldTrans) const;
		void setWorldTransform(const btTransform &worldTrans);
		Create * make;
		float elapsed;
		float fireRate;
	};
}
