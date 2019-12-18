#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//Initialize hinge elements
	bodyA = nullptr;
	bodyB = nullptr;
	hinge = nullptr;
	axis = Cube(1, 3, 1);
	helix = Cube(7, 2, 1);

	//Initialize road variables
	road.size.Set(2, 4, 2);
	road.color.Set(0.0f, 1.0f, 0.0f);
	roadWidth = 20;
	radius = 20;
}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	//App->audio->PlayMusic("music/mercadonaremix.ogg");

	LOG("Loading Intro assets");
	bool ret = true;


	Cube shop1(100, 40, 1);
	road.color.Set(0.0f, 0.0f, 0.0f);
	
	CreateRect(-10.0f, 0, 0, roadWidth, 50, road, ORIENTATION::NORTH);
	Cube shelves(20, 15, 8);
	shelves.color.Set(0.6f, 0.0f, 0.0f);

	CreateRect(-145, 0, -205, 110, 1, shop1, ORIENTATION::NORTH);
	CreateRect(-145, 0, -235, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-145, 0, -215, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-65, 0, -235, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-65, 0, -215, 30, 20, shelves, ORIENTATION::SOUTH);

	//Create circuit
	CreateRect(-10.0f, 0, 0, roadWidth, 100, road, ORIENTATION::NORTH);
	CreateCurve(-30.0f, 0, 200, roadWidth, 0, 90, road, radius);
	CreateRect(-115, 0, 220, roadWidth, 40, road, ORIENTATION::WEST);
	CreateCurve(-115.0f, 0.0f, 200, roadWidth, 90, 270, road, radius);
	CreateRect(-115, 0, 160, roadWidth, 30, road, ORIENTATION::WEST);
	CreateCurve(-60, 0, 140, roadWidth, -90, 90, road, radius);
	CreateRect(-220, 0, 100, roadWidth, 80, road, ORIENTATION::WEST);
	CreateCurve(-220.0f, 0.0f, 80, roadWidth, 90, 270, road, radius);
	CreateRect(-220, 0, 40, roadWidth, 20, road, ORIENTATION::WEST);
	CreateCurve(-180, 0, 20, roadWidth, -90, 90, road, radius);
	CreateRect(-220, 0, -20, roadWidth, 20, road, ORIENTATION::WEST);
	CreateCurve(-220.0f, 0.0f, -40, roadWidth, 90, 270, road, radius);
	CreateRect(-220, 0, -80, roadWidth, 50, road, ORIENTATION::WEST);
	CreateCurve(-120.0f, 0, -100, roadWidth, 0, 90, road, radius);
	CreateRect(-100.0f, 0, -200, roadWidth, 50, road, ORIENTATION::NORTH);
	CreateRect(-10.0f, 0, -210, roadWidth, 110, road, ORIENTATION::NORTH);

	//Create CheckPoints
	CreateCheckPoint({0.0f,0.0f,20.0f}, 90.0f);
	CreateCheckPoint({ 0.0f, 0.0f, 120.0f }, 90.0f);

	//Create Boxes
	SetBoxes({0.0f,10.0f,0.0f});
	
	//Hinges
	axis.SetPos(0, 1, 40);
	axis.color = White;
	bodyA = App->physics->AddBody(axis, 0.0f);

	helix.SetPos(10, 3.5, 50);
	helix.color = Red;
	bodyB = App->physics->AddBody(helix, 4.0f);

	hinge = App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), vec3(5, 0, 0), vec3(0, 1, 0), vec3(0, 1, 0), true);
	hinge->enableAngularMotor(true, 4.0f, INFINITE);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	for (uint i = 0; i < map.Count(); ++i)
		delete map[i];

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Cube ground(1000, 2, 1000);
	ground.SetPos(0, -1, 0);
	ground.Render();

	Cube shop2(205, 200, 1);
	shop2.SetPos(-90, 20, -305);
	// para techo
	shop2.transform.rotate(90, vec3(1, 0, 0));
	shop2.Render();


	//Render Map 
	for (int i = 0; i < map.Count(); i++) map[i]->Render();
	//Render CheckPoints
	for (int i = 0; i < CheckPoints_List.Count(); i++) CheckPoints_List[i].Render();


	//Render Hinges
	axis.Render();
	helix.Render();

	mat4x4 transform;
	bodyB->GetTransform(transform.M);
	helix.transform = transform;



	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->is_sensor == true) {
		LOG("HOLA ESTOY CHOCANDO");
	}
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

		map.PushBack(c1);
		map.PushBack(c2);
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

		map.PushBack(c1);
		map.PushBack(c2);
	}

}

void ModuleSceneIntro::SetBoxes(const vec3 Position) {
	Cube* box = new Cube;
	PhysBody3D* phys = nullptr;

	box->SetPos(Position.x, Position.y, Position.z);
	box->size.Set(2.0f, 2.0f, 2.0f);
	box->color.Set(1.0f, 0.0f, 0.0f);
	Boxes_List.add(box);

	phys = App->physics->AddBody(*box, 0.0F);
	map.PushBack(box);
}

void ModuleSceneIntro::CheckBoxes() {

	//This function will check last box that have been triggered and will store data of spawn if user press any key 
	//p2List_item<Cube*> Actual_Cubes = Boxes_List;

	//TODO: SHOULD READ EACH CHECKPOINT STORED
}

void ModuleSceneIntro::CreateCheckPoint(const vec3 Position, float angle) {

	Cube Sensor;
	Sensor.size.Set(4.0f, 4.0f, roadWidth-5.0f);
	Sensor.SetPos(Position.x, Position.y, Position.z);
	Sensor.SetRotation(angle, { 0, 1, 0 });

	PhysBody3D* PhysBodySensor = App->physics->AddBody(Sensor, this, 0.0f, true);

	Sphere CheckPointLight;
	CheckPointLight.radius = 1.0f;
	CheckPointLight.color = White;
	CheckPointLight.SetPos(Position.x, Position.y + 10.0f, Position.z);

	SavePoints.PushBack(PhysBodySensor);
	CheckPoints_List.PushBack(CheckPointLight);
}