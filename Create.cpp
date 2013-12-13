#include "Create.h"
#include <stdlib.h>   
#include "Game.h"
#include "Sphere.h"
#include "Box.h"
#include "Cylinder.h"
#include "Ground.h"
#include "Content.h"
#include "GravGun.h"
#include "Model.h"
#include "dirent.h"
#include "Utils.h"

using namespace BGE;

Create::Create(btDiscreteDynamicsWorld * dynamicsWorld)
{
	this->dynamicsWorld = dynamicsWorld;
}


Create::~Create(void)
{
}

void Create::CreateWall(glm::vec3 startAt, float width, float height, float mass, float blockWidth, float blockHeight, float blockDepth)
{
	float x = startAt.x;
	float gap = 0.5f;

	for (int w = 0 ; w < width ; w ++)
	{
		for (int h = 0 ; h < height ; h ++)	
		{
			float z = startAt.z + (blockWidth * w);
			float y = ((blockHeight + gap) / 2.0f) + ((blockHeight + gap) * h);
			CreateBox(blockWidth, blockHeight, blockDepth, mass, glm::vec3(x, y, z), glm::quat());
		}
	}
}

void Create::CreateBuilding(glm::vec3 startAt,float depth, float width, float height, float mass, float blockWidth, float blockHeight, float blockDepth)
{
	
	float gap = 0.5f;

	for(int d = 0; d < depth; d++)
	{	
		for (int w = 0 ; w < width ; w ++)
		{
			float x = startAt.x;
			for (int h = 0 ; h < height ; h ++)	
			{
				float z = startAt.z + (blockWidth * w);
				float y = ((blockHeight + gap) / 2.0f) + ((blockHeight + gap) * h);
				CreateBox(blockWidth, blockHeight, blockDepth, mass*h, glm::vec3(x, y, z), glm::quat());
			}
		}
	}
}



shared_ptr<PhysicsController> Create::CreateSphere(float radius, float m, glm::vec3 pos, glm::quat quat)
{
	shared_ptr<GameComponent> sphere (new Sphere(radius));
	Game::Instance()->Attach(sphere);

	btDefaultMotionState * sphereMotionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));	

	btScalar mass = m;
	btVector3 sphereInertia(0,0,0);
	btCollisionShape * sphereShape = new btSphereShape(radius);

	sphereShape->calculateLocalInertia(mass,sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,sphereMotionState, sphereShape, sphereInertia);
	btRigidBody * body = new btRigidBody(fallRigidBodyCI);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	dynamicsWorld->addRigidBody(body);

	shared_ptr<PhysicsController> sphereController (new PhysicsController(sphereShape, body, sphereMotionState));	
	body->setUserPointer(sphereController.get());
	sphere->Attach(sphereController);
	sphereController->tag = "Sphere";	
	return sphereController;
}


shared_ptr<PhysicsController> Create::CreateBox(float width, float height, float depth, float m, glm::vec3 pos, glm::quat quat)
{
	btCollisionShape * boxShape = new btBoxShape(btVector3(width, height, depth) * 0.50);
	btScalar mass = m;
	btVector3 boxInertia(0,0,0);
	boxShape->calculateLocalInertia(mass,boxInertia);

	shared_ptr<Box> box = make_shared<Box>(width, height, depth);
	box->worldMode = GameComponent::from_child;
	box->position = pos;
	Game::Instance()->Attach(box);

	btDefaultMotionState * boxMotionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));			
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,  boxMotionState, boxShape, boxInertia);
	btRigidBody * body = new btRigidBody(fallRigidBodyCI);
	body->setFriction(567);
	dynamicsWorld->addRigidBody(body);

	shared_ptr<PhysicsController> boxController = make_shared<PhysicsController>(PhysicsController(boxShape, body, boxMotionState));
	boxController->tag = "Box";
	body->setUserPointer(boxController.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	box->Attach(boxController);

	return boxController;
}

shared_ptr<PhysicsController> Create::CreateCylinder(float radius, float height, float m, glm::vec3 pos, glm::quat quat)
{
	btCollisionShape * shape = new btCylinderShape(btVector3(radius, height * 0.5f, radius));
	btScalar mass = m;
	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass,inertia);

	shared_ptr<GameComponent> cyl = make_shared<GameComponent>(Cylinder(radius, height));
	cyl->position = pos;
	Game::Instance()->Attach(cyl);

	btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(GLToBtQuat(quat)
		,GLToBtVector(pos)));			
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass,  motionState, shape, inertia);
	btRigidBody * body = new btRigidBody(rigidBodyCI);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	dynamicsWorld->addRigidBody(body);

	shared_ptr<PhysicsController> component = make_shared<PhysicsController>(PhysicsController(shape, body, motionState));
	body->setUserPointer(component.get());
	component->tag = "Cylinder";
	cyl->Attach(component);

	return component;
}

shared_ptr<PhysicsController> Create::CreateCameraPhysics()
{
	btVector3 inertia;
	btCollisionShape * cameraCyl = new btCylinderShape(btVector3(0.5f, 5.0f, 2.5f));
	cameraCyl->calculateLocalInertia(1, inertia);
	shared_ptr<GravGun> physicsCamera = make_shared<GravGun>(this);

	shared_ptr<Camera> camera = Game::Instance()->camera;
	camera->Attach(physicsCamera);

	btRigidBody::btRigidBodyConstructionInfo cameraCI(10,physicsCamera.get(), cameraCyl, inertia);  
	btRigidBody * body = new btRigidBody(cameraCI);
	physicsCamera->SetPhysicsStuff(cameraCyl, body, physicsCamera.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(body);
	return physicsCamera;
}


shared_ptr<PhysicsController> Create::CreateGroundPhysics()
{
	shared_ptr<Ground> ground = make_shared<Ground>();

	btCollisionShape * groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState * groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* body = new btRigidBody(groundRigidBodyCI);
	body->setFriction(100);
	dynamicsWorld->addRigidBody(body);
	body->setUserPointer(ground.get());
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	shared_ptr<PhysicsController> groundComponent (new PhysicsController(groundShape, body, groundMotionState));
	groundComponent->tag = "Ground";
	ground->Attach(groundComponent);	
	Game::Instance()->SetGround(ground);
	return groundComponent;
}



void Create::CreateDoll(glm::vec3 pos)
{

	std::shared_ptr<PhysicsController> body;
	std::shared_ptr<PhysicsController> arm1;
	std::shared_ptr<PhysicsController> forearm1;
	std::shared_ptr<PhysicsController> arm2;
	std::shared_ptr<PhysicsController> forearm2;
	std::shared_ptr<PhysicsController> head;
	std::shared_ptr<PhysicsController> leg1;
	std::shared_ptr<PhysicsController> leg2;
 
	head = CreateSphere(2,30,glm::vec3(pos.x, pos.y + 6, pos.z), glm::quat());
	body = CreateBox(2,6,3,20, pos, glm::quat()); 
	arm1 = CreateBox(1,1,3,10, glm::vec3(pos.x, pos.y + 2, pos.z + 5), glm::quat()); 
	forearm1 = CreateBox(1,1,4,10, glm::vec3(pos.x, pos.y + 2, pos.z + 8), glm::quat()); 
	arm2 = CreateBox(1,1,3,10, glm::vec3(pos.x, pos.y + 2, pos.z - 5), glm::quat()); 
	forearm2 = CreateBox(1,1,4,10, glm::vec3(pos.x, pos.y + 2, pos.z - 8), glm::quat());
	leg1 = CreateBox(1,4,1,15, glm::vec3(pos.x, pos.y - 6.5, pos.z -0.7), glm::quat()); 
	leg2 = CreateBox(1,4,1,15, glm::vec3(pos.x, pos.y - 6.5, pos.z + 0.7), glm::quat());

	head->tag = "Head";
	body->tag = "Body";
	arm1->tag = "Arm";
	arm2->tag = "Arm";
	forearm1->tag = "Forearm";
	forearm2->tag = "Forearm";
	leg1->tag = "Leg";
	leg2->tag = "Leg";

	btHingeConstraint * neck = new btHingeConstraint(* head->rigidBody, * body->rigidBody, btVector3(0,-1.5f,0),btVector3(0,4.5f,0), btVector3(0,1,0), btVector3(0,1,0), true);

	btPoint2PointConstraint * shoulder = new btPoint2PointConstraint(*body->rigidBody, *arm1->rigidBody, btVector3(0,2.0f,2.0f),btVector3(0,-2.0f,-2.0f));
	btPoint2PointConstraint * elbow = new btPoint2PointConstraint(*arm1->rigidBody, *forearm1->rigidBody, btVector3(0,0,2.0f),btVector3(0,0,-2.0f));
	btPoint2PointConstraint * shoulder2 = new btPoint2PointConstraint(*body->rigidBody, *arm2->rigidBody, btVector3(0,2.0f,-2.0f),btVector3(0,-1.0f,-2.0f));
	btPoint2PointConstraint * elbow2 = new btPoint2PointConstraint(*arm2->rigidBody, *forearm2->rigidBody, btVector3(0,0,2.0f),btVector3(0,0,-2.0f));
	btPoint2PointConstraint * hip1 = new btPoint2PointConstraint(*body->rigidBody, *leg1->rigidBody, btVector3(0,-3.0f,-1.0f),btVector3(0,3.0f,0));
	btPoint2PointConstraint * hip2 = new btPoint2PointConstraint(*body->rigidBody, *leg2->rigidBody, btVector3(0,-3.0f,1.0f),btVector3(0,3.0f,0));

	dynamicsWorld->addConstraint(neck);
	dynamicsWorld->addConstraint(hip1);
	dynamicsWorld->addConstraint(hip2);
	dynamicsWorld->addConstraint(shoulder);
	dynamicsWorld->addConstraint(elbow);
	dynamicsWorld->addConstraint(shoulder2);
	dynamicsWorld->addConstraint(elbow2);
}
void Create::CreateCar(glm::vec3 p, float cm, float wm)
{

		glm::vec3 position = p;
		float width = 12;
		float height = 2;
		float length = 5;
		float chassisMass = cm;
		float wheelWidth = 1;
		float wheelRadius = 1.5f;
		float wheelMass = wm;
		float wheelOffset = 1.0f;

		shared_ptr<PhysicsController> chassis = CreateBox(width, height, length, chassisMass, position, glm::quat());
		chassis->tag = "Chassis";

		frontright= CreateCylinder(wheelRadius, wheelWidth, wheelMass, glm::vec3(94.5,0,45.5), glm::quat());
		hinge1 = new btHingeConstraint(* chassis->rigidBody, * frontright->rigidBody, btVector3(-5.5f,0.0f,-4.5f), btVector3(0,0,0), btVector3(0,0,1), btVector3(0,1,0), true);
		frontright->tag = "Wheel";
		dynamicsWorld->addConstraint(hinge1);

		backright= CreateCylinder(wheelRadius, wheelWidth,wheelMass, glm::vec3(105.5,0,45.5), glm::quat());
		hinge2 = new btHingeConstraint(* chassis->rigidBody, * backright->rigidBody, btVector3(5.5f,0.0f,-4.5f),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		backright->tag = "Wheel";
		dynamicsWorld->addConstraint(hinge2);

		backleft= CreateCylinder(wheelRadius, wheelWidth, wheelMass, glm::vec3(94.5,0,54.5), glm::quat()); 
		hinge3 = new btHingeConstraint(* chassis->rigidBody, * backleft->rigidBody, btVector3(-5.5f,0.0f,4.5f),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		backleft->tag = "Wheel";
		dynamicsWorld->addConstraint(hinge3);

		frontleft= CreateCylinder(wheelRadius, wheelWidth, wheelMass, glm::vec3(105.5,0,54.5), glm::quat()); 
		hinge4 = new btHingeConstraint(* chassis->rigidBody, * frontleft->rigidBody, btVector3(5.5f,0.0f,4.5f),btVector3(0,0, 0), btVector3(0,0,1), btVector3(0,1,0), true);
		frontleft->tag = "Wheel";
		dynamicsWorld->addConstraint(hinge4);
}

void Create::CreateBalloon(glm::vec3 p, int amount)
{
	for(int i = 0; i <= amount; i ++)
	{	
		float randnumx = rand() % 10;
		float randnumz = rand() % 10;
		shared_ptr<PhysicsController> balloon = CreateSphere(1.5,1,glm::vec3(p.x +(i*randnumx),p.y,p.z + (i*randnumz)),glm::quat());
		balloon->rigidBody->setGravity(btVector3(0,2,0));
		balloon->tag = "balloon";
	}
}

void Create::MoveCarForward(float speed)
{
		frontright->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,speed)));
		backright->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,speed)));
		backleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,speed)));
		frontleft->rigidBody->applyTorque(GLToBtVector(glm::vec3(0.0f,0.0f,speed)));
}




