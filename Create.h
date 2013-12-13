#pragma once
#include <memory>
#include "PhysicsController.h"
#include "GameComponent.h"

using namespace std;

namespace BGE
{
	class Create
	{
	private:
		
	public:
		Create(btDiscreteDynamicsWorld * dynamicsWorld);
		~Create(void);

		shared_ptr<PhysicsController> CreateBox(float width, float height, float depth, float m, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateSphere(float radius, float m, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateCylinder(float radius, float height, float m, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateCameraPhysics();
		shared_ptr<PhysicsController> CreateGroundPhysics();
		shared_ptr<PhysicsController> CreateFromModel(string name, glm::vec3 pos, glm::quat quat, glm::vec3 scale = glm::vec3(1));
		
		std::shared_ptr<PhysicsController> frontright;
		std::shared_ptr<PhysicsController> backright;
		std::shared_ptr<PhysicsController> backleft;
		std::shared_ptr<PhysicsController> frontleft;

		void MoveCarForward(float speed);

		void CreateDoll(glm::vec3 pos);
		void CreateCar(glm::vec3 p, float cm, float wm);
		void CreateWall(glm::vec3 startAt, float width, float height, float mass, float blockWidth = 5, float blockHeight = 5, float blockDepth = 5);
		shared_ptr<PhysicsController> CreateRandomObject(glm::vec3 point, glm::quat q);
		btDiscreteDynamicsWorld * dynamicsWorld;
	};
}

