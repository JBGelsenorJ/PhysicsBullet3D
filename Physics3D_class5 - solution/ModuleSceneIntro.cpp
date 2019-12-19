#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//Initialize map elements
	bodyA = nullptr;
	bodyB = nullptr;
	hinge = nullptr;

	axis = Cube(1, 3, 1);
	helix = Cube(7, 2, 1);
	firstRamp = Cube(10, 2, 40);
	secondRamp = Cube(-10, 2, 40);

	road.size.Set(2, 4, 2);
	roadForCurve.size.Set(1, 4, 1);
	road.color = Turquoise;
	roadForCurve.color.Set(0.3f, 0.32f, 0.4f);
	roadWidth = 30;
	radius = 20;

	MercaWall1pb = nullptr;
	MercaWall2pb = nullptr;
	MercaWall3pb = nullptr;
	MercaWall4pb = nullptr;
	MercaEntrancepb = nullptr;
	startCube = nullptr;
	MercaWall1 = Cube(400, 40, 1);
	MercaWall2 = Cube(400, 40, 1);
	MercaWall3 = Cube(210, 40, 1);
	MercaEntrance = Cube(15, 15, 10);
	MercaEntrance.color = MercaWall1.color = MercaWall2.color = MercaWall3.color = LimeGreen;

	start = Cube(32, 5, 0.4);
	start.color = LimeGreen;

}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->audio->PlayMusic("music/mercadonaremix.ogg");

	LOG("Loading Intro assets");
	bool ret = true;


	Cube MercaWall4(100, 40, 1);

	MercaWall1.SetPos(-185, 0, -405);
	MercaWall1.transform.rotate(90, vec3(0, 1, 0));
	MercaWall1pb = App->physics->AddBody(MercaWall1, 0.0f);
	
	MercaWall2.SetPos(0, 0, -405);
	MercaWall2.transform.rotate(90, vec3(0, 1, 0));
	MercaWall2pb = App->physics->AddBody(MercaWall2, 0.0f);

	MercaWall3.SetPos(-80, 0, -605);
	MercaWall3pb = App->physics->AddBody(MercaWall3, 0.0f);
	
	MercaEntrance.SetPos(-90,0,-200);
	MercaEntrancepb = App->physics->AddBody(MercaEntrance, 0.0f);

	Cube shelves(20, 25, 8);
	shelves.color.Set(0.6f, 0.0f, 0.0f);

	CreateRect(-145, 0, -205, 110, 1, MercaWall4, ORIENTATION::NORTH);
	CreateRect(-145, 0, -235, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-145, 0, -215, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-65, 0, -235, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-65, 0, -215, 30, 20, shelves, ORIENTATION::SOUTH);

	//Create circuit
	CreateRect(-10.0f, 0, -150, roadWidth, 175, road, ORIENTATION::NORTH);
	CreateCurve(-30.0f, 0, 200, roadWidth, 0, 90, roadForCurve, radius);
	CreateRect(-140, 0, 220, roadWidth, 55, road, ORIENTATION::WEST);
	CreateCurve(-140.0f, 0.0f, 200, roadWidth, 90, 270, roadForCurve, radius);
	CreateRect(-140, 0, 150, roadWidth, 30, road, ORIENTATION::WEST);
	CreateCurve(-80, 0, 130, roadWidth, -90, 90, roadForCurve, radius);
	CreateRect(-190, 0, 80, roadWidth, 55, road, ORIENTATION::WEST);
	CreateCurve(-190.0f, 0.0f, 60, roadWidth, 90, 270, roadForCurve, radius);
	CreateRect(-190, 0, 10, roadWidth, 35, road, ORIENTATION::WEST);
	CreateCurve(-120, 0, -10, roadWidth, -90, 90, roadForCurve, radius);
	CreateRect(-180, 0, -60, roadWidth, 30, road, ORIENTATION::WEST);
	CreateCurve(-180.0f, 0.0f, -80, roadWidth, 90, 270, roadForCurve, radius);
	CreateRect(-180, 0, -130, roadWidth, 30, road, ORIENTATION::WEST);
	CreateCurve(-120.0f, 0, -150, roadWidth, 0, 90, roadForCurve, radius);
	CreateRect(-100.0f, 0, -200, roadWidth, 25, road, ORIENTATION::NORTH);

	//Create CheckPoints
	CreateCheckPoint({ -50.0f, 2.5f, 135.0f }, 90.0f);
	CreateCheckPoint({ -100.0f, 2.5f, -40.0f }, 90.0f);
	CreateCheckPoint({ -90.0f, 2.5f, -250.0f }, 90.0f);

	//Create Boxes
	SetBoxes({ 0.0f,10.0f,0.0f });

	//Hinges
	CreateHinge({ 5.0f, 1.0f, 40.0f }, 1.0f, 1);
	CreateHinge({ 5.0f, 1.0f, 100.0f }, 1.0f, -1);
	CreateHinge({ 5.0f, 1.0f, 160.0f }, 1.0f, 1);
	CreateHinge({ -8.0f, 1.0f,225.0f }, 1.0f, -1);
	CreateHinge({ -68.0f, 1.0f, 235.0f }, 2.0f, 1);
	CreateHinge({ -175.0f, 1.0f, 200.0f }, 2.0f, -1);
	CreateHinge({-118.0f, 1.0f, 165.0f}, 2.0f, -1);
	CreateHinge({ -65.0f, 1.0f, 165.0f }, 2.0f, 1);
	CreateHinge({ -55.0f, 1.0f, 110.0f }, 2.0f, -1);
	CreateHinge({ -100.0f, 1.0f, 95.0f }, 3.0f, -1);
	CreateHinge({ -180.0f, 1.0f, 95.0f }, 3.0f, 1);
	CreateHinge({ -220.0f, 1.0f, 65.0f }, 4.0f, -1);
	CreateHinge({ -180.0f, 1.0f, 26.0f }, 4.0f, 1);
	CreateHinge({ -100.0f, 1.0f, 15.0f }, 5.0f, 1);
	CreateHinge({ -90.0f, 1.0f, -20.0f }, 5.0f, -1);
	CreateHinge({ -120.0f, 1.0f, -45.0f }, 5.0f, -1);
	CreateHinge({ -200.0f, 1.0f, -50.0f }, 5.0f, -1);
	CreateHinge({ -210.0f, 1.0f, -100.0f }, 5.0f, -1);
	CreateHinge({ -150.0f, 1.0f, -115.0f }, 5.0f, -1);
	CreateHinge({ -110.0f, 1.0f, -120.0f }, 5.0f, -1);
	

	//Create bridge
	CreateRamp(5.0f, 0.25f, -70.0f, 0.0f, -25.0f, { 1, 0, 0 }, true);
	CreateRamp(5.0f, 0.25f, -20.0f, 0.0f, 25.0f, { 1, 0, 0 }, true);

	CreateRamp(-85.0f, 0.25f, -150.0f, 0.0f, 15.0f, { 1, 0, 0 }, true);
	CreateRamp(-90.0f, 3.7f, -212.0f, 0.0f, -25.0f, { 1, 0, 0 }, true);

	//Start 
	Cylinder bar = CreateCylinder(0.5f, 35.0f, vec3(-10, 0, -111), Turquoise, true, 0.0f, vec3(0, 0, 0));
	Cylinder bar2 = CreateCylinder(0.5f, 35.0f, vec3(20, 0, -111), Turquoise, true, 0.0f, vec3(0, 0, 0));
	
	start.SetPos(5, 20,-111);
	startCube = App->physics->AddBody(start, 0.0f);
	
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	//Delete lists
	for (uint i = 0; i < cubes.Count(); ++i) delete cubes[i];

	ramp.clear();
	cylinders.clear();

	//TODO: MEMORY LEAKS

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Cube ground(2000, 1, 2000);
	ground.SetPos(0, -1, 0);
	ground.Render();

	
	//Draw map elements
	Cube shop2(205, 400, 1);
	shop2.SetPos(-90, 20, -405);

	shop2.transform.rotate(90, vec3(1, 0, 0));
	shop2.color = LimeGreen;
	shop2.Render();

	MercaWall1.Render();
	MercaWall2.Render();
	MercaWall3.Render();
	MercaEntrance.Render();
	start.Render();

	//Render Map 
	for (int i = 0; i < cubes.Count(); i++) cubes[i]->Render();
	//Render CheckPoints
	for (int i = 0; i < CheckPoints_List.Count(); i++) CheckPoints_List[i].Render();
	//Render traps
	for (int i = 0; i < Traps.Count(); i++) Traps[i].Render();
	//Update traps position
	for (int i = 1; i < Traps.Count(); i = i + 2) {
		HingesBody[i]->GetTransform(transform.M);
		Traps[i].transform = transform;
	}
	//Render ramps
	for (p2List_item<Cube>* ramp_item = ramp.getFirst(); ramp_item != nullptr; ramp_item = ramp_item->next) ramp_item->data.Render();
	for (p2List_item<Cylinder>* cylinders_item = cylinders.getFirst(); cylinders_item != nullptr; cylinders_item = cylinders_item->next) cylinders_item->data.Render();
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

	if (body1 == SavePoints[0] && body2 == (PhysBody3D*)App->player->vehicle)
	{
		CheckPoints_List[0].color = Yellow;
		checkpoints_index = 1;
	};

	if (body1 == SavePoints[1] && body2 == (PhysBody3D*)App->player->vehicle)
	{
		CheckPoints_List[1].color = Yellow;
		checkpoints_index = 2;
	};

	if (body1 == SavePoints[2] && body2 == (PhysBody3D*)App->player->vehicle)
	{
		CheckPoints_List[2].color = Yellow;
		checkpoints_index = 3;
	};
}

void ModuleSceneIntro::CreateRect(const float& x, const float& y, const float& z, const float& width, const float& length, const Cube& cube, ORIENTATION orientation)
{
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	PhysBody3D* phys1 = nullptr;
	PhysBody3D* phys2 = nullptr;

	for (int i = 0; i < length; i = i + 3)
	{
		c1 = new Cube(cube);
		c2 = new Cube(cube);

		switch (orientation)
		{
		case ORIENTATION::NORTH:
			c1->SetPos(x, y, z + cube.size.x * i);
			c2->SetPos(x + width, y, z + cube.size.x * i);
			break;
		case ORIENTATION::SOUTH:
			c1->SetPos(x, y, z - cube.size.x * i);
			c2->SetPos(x + width, y, z - cube.size.x * i);
			break;
		case ORIENTATION::EAST:
			c1->SetPos(x - cube.size.x * i, y, z);
			c2->SetPos(x - cube.size.x * i, y, z + width);
			break;
		case ORIENTATION::WEST:
			c1->SetPos(x + cube.size.x * i, y, z);
			c2->SetPos(x + cube.size.x * i, y, z + width);
			break;
		case ORIENTATION::NORTHEAST:
			c1->SetPos(x - cube.size.x * i, y, z + cube.size.x * i);
			c2->SetPos(x + width - cube.size.x * i, y, z + cube.size.x * i);
			break;
		case ORIENTATION::NORTHWEST:
			c1->SetPos(x + cube.size.x * i, y, z + cube.size.x * i);
			c2->SetPos(x + width + cube.size.x * i, y, z + cube.size.x * i);
			break;
		case ORIENTATION::SOUTHEAST:
			c1->SetPos(x - cube.size.x * i, y, z - cube.size.x * i);
			c2->SetPos(x - cube.size.x * i + width, y, z - cube.size.x * i);
			break;
		case ORIENTATION::SOUTHWEST:
			c1->SetPos(x + cube.size.x * i, y, z - cube.size.x * i);
			c2->SetPos(x + cube.size.x * i + width, y, z - cube.size.x * i);
			break;
		default:
			break;
		}

		phys1 = App->physics->AddBody(*c1, 0.0F);
		phys2 = App->physics->AddBody(*c2, 0.0F);

		cubes.PushBack(c1);
		cubes.PushBack(c2);
	}
}

void ModuleSceneIntro::CreateCurve(const float& x, const float& y, const float& z, const float& width, const float& angle_i, const float& angle_f, const Cube& cube, const float& radius)
{
	Cube* c1 = nullptr;
	Cube* c2 = nullptr;
	PhysBody3D* curve1 = nullptr;
	PhysBody3D* curve2 = nullptr;

	for (int i = angle_i; i <= angle_f; i = i + cube.size.x * 5) {
		c1 = new Cube(cube);
		c2 = new Cube(cube);

		c1->SetPos(x + radius * cos(i * 3.1415 / 180), y, z + radius * sin(i * 3.1415 / 180));
		c2->SetPos(x + (radius + width) * cos(i * 3.1415 / 180), y, z + (radius + width) * sin(i * 3.1415 / 180));

		curve1 = App->physics->AddBody(*c1, 0.0F);
		curve2 = App->physics->AddBody(*c2, 0.0F);

		cubes.PushBack(c1);
		cubes.PushBack(c2);
	}

}

void ModuleSceneIntro::CreateRamp(const float x, const float y, const float z, float mass, float angle, vec3 angle_rot, bool is_collider)
{

	Cube cube;
	cube.size.Set(20, 0.5, 20);
	cube.color = Turquoise;
	cube.SetPos(x, y, z);

	if (angle != 0.0f)
	{
		cube.SetRotation(angle, angle_rot);
	}

	if (is_collider)
	{
		App->physics->AddBody(cube, mass);
	}

	ramp.add(cube);

}

void ModuleSceneIntro::SetBoxes(const vec3 Position) {
	Cube* box = new Cube;
	PhysBody3D* phys = nullptr;

	box->SetPos(Position.x, Position.y, Position.z);
	box->size.Set(2.0f, 2.0f, 2.0f);
	box->color = Blue;
	//Boxes_List.add(box);
	

	cubes.PushBack(box);

}

void ModuleSceneIntro::CreateCheckPoint(const vec3 Position, float angle) {

	Cube Sensor;
	Sensor.size.Set(5.0f, 5.0f, 10.0f);
	Sensor.SetPos(Position.x, Position.y, Position.z);
	Sensor.SetRotation(angle, { 0, 1, 0 });

	PhysBody3D* PhysBodySensor = nullptr;
	PhysBodySensor = App->physics->AddBody(Sensor, 0.0f, PBType::CHECKPOINT);
	PhysBodySensor->SetAsSensor(true);
	PhysBodySensor->collision_listeners.add(this);
	LOG("TYPE IS %i", PhysBodySensor->PhysBody_Type);

	Sphere CheckPointLight;
	CheckPointLight.radius = 1.0f;
	CheckPointLight.color = White;
	CheckPointLight.SetPos(Position.x, Position.y + 10.0f, Position.z);

	SavePoints.PushBack(PhysBodySensor);
	CheckPoints_List.PushBack(CheckPointLight);
}

void ModuleSceneIntro::CreateHinge(vec3 Position, float speed, int way) {

	Cube axis;
	axis.size.Set(1, 3, 1);
	axis.color = Black;
	axis.SetPos(Position.x, Position.y, Position.z);
	PhysBody3D* bodyA = App->physics->AddBody(axis, 0.0f);

	Cube helix;
	helix.size.Set(10, 2, 1);
	helix.color = LivingCoral;
	helix.SetPos((Position.x + 10.0f), (Position.y + 3.5f), (Position.z + 10.0f));
	PhysBody3D* bodyB = App->physics->AddBody(helix, 4.0f);

	HingesBody.PushBack(bodyA);
	HingesBody.PushBack(bodyB);

	Traps.PushBack(axis);
	Traps.PushBack(helix);

	hinge = App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), vec3(5, 0, 0), vec3(0, way, 0), vec3(0, 1, 0), true);
	hinge->enableAngularMotor(true, speed, INFINITE);
}

Cylinder ModuleSceneIntro::CreateCylinder(float radius, float height, vec3 pos, Color color, bool flip, float angle, vec3 u)
{
	Cylinder cy(radius, height);
	cy.SetPos(pos.x, pos.y, pos.z);
	cy.color = color;

	if (flip)
		cy.SetRotation(90.0f, vec3(0.0f, 0.0f, 1.0f));

	if (angle != 0.0f)
		cy.SetRotation(angle, vec3(u.x, u.y, u.z));

	cylinders.add(cy);

	return cy;
}
