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
		
		std::shared_ptr<PhysicsController> frontright;
		std::shared_ptr<PhysicsController> backright;
		std::shared_ptr<PhysicsController> backleft;
		std::shared_ptr<PhysicsController> frontleft;
		btHingeConstraint * hinge1;
		btHingeConstraint * hinge2;
		btHingeConstraint * hinge3;
		btHingeConstraint * hinge4;

	public:
		Create(btDiscreteDynamicsWorld * dynamicsWorld);
		~Create(void);

		shared_ptr<PhysicsController> CreateBox(float width, float height, float depth, float m, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateSphere(float radius, float m, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateCylinder(float radius, float height, float m, glm::vec3 pos, glm::quat quat);
		shared_ptr<PhysicsController> CreateCameraPhysics();
		shared_ptr<PhysicsController> CreateGroundPhysics();
		shared_ptr<PhysicsController> CreateFromModel(string name, glm::vec3 pos, glm::quat quat, glm::vec3 scale = glm::vec3(1));
		
		void CreateDoll(glm::vec3 pos);
		void CreateCar(glm::vec3 p, float cm, float wm);
		void CreateWall(glm::vec3 startAt, float width, float height, float mass, float blockWidth = 5, float blockHeight = 5, float blockDepth = 5);
		void CreateBuilding(glm::vec3 startAt,float depth, float width, float height, float mass, float blockWidth = 5, float blockHeight = 5, float blockDepth = 5);
		void CreateBalloon(glm::vec3 p, int amount);

		void MoveCarForward(float speed);
		void turnLeft(float speed);

		btDiscreteDynamicsWorld * dynamicsWorld;
	};
}

