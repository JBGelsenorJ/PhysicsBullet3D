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

	MercaWall1pb = nullptr;
	MercaWall2pb = nullptr;
	MercaWall3pb = nullptr;
	MercaWall4pb = nullptr;
	MercaWall1 = Cube(400, 40, 1);
	MercaWall2 = Cube(400, 40, 1);
	MercaWall3 = Cube(210, 40, 1);
}

ModuleSceneIntro::~ModuleSceneIntro()
{

}

// Load assets
bool ModuleSceneIntro::Start()
{
	/*App->audio->PlayMusic("music/mercadonaremix.ogg");*/

	LOG("Loading Intro assets");
	bool ret = true;


	Cube shop1(100, 40, 1);
	road.color.Set(0.0f, 0.0f, 0.0f);

	MercaWall1.SetPos(-185, 0, -405);
	MercaWall1.transform.rotate(90, vec3(0, 1, 0));
	MercaWall1pb = App->physics->AddBody(MercaWall1, 0.0f);
	
	MercaWall2.SetPos(0, 0, -405);
	MercaWall2.transform.rotate(90, vec3(0, 1, 0));
	MercaWall2pb = App->physics->AddBody(MercaWall2, 0.0f);

	MercaWall3.SetPos(-80, 0, -605);
	MercaWall3pb = App->physics->AddBody(MercaWall3, 0.0f);


	CreateRect(-10.0f, 0, 0, roadWidth, 50, road, ORIENTATION::NORTH);
	Cube shelves(20, 25, 8);
	shelves.color.Set(0.6f, 0.0f, 0.0f);

	CreateRect(-145, 0, -205, 110, 1, shop1, ORIENTATION::NORTH);
	CreateRect(-145, 0, -235, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-145, 0, -215, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-65, 0, -235, 30, 20, shelves, ORIENTATION::SOUTH);
	CreateRect(-65, 0, -215, 30, 20, shelves, ORIENTATION::SOUTH);

	//Create circuit
	//CreateRect(-10.0f, 0, 0, roadWidth, 100, road, ORIENTATION::NORTH);
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
	SetBoxes({0.0f,0.0f,10.0f});
	
	//Hinges
	CreateHinge({ 0.0f, 1.0f, 40.0f });
	CreateHinge({ 0.0f, 1.0f, 100.0f });
	CreateHinge({ 0.0f, 1.0f, 160.0f });
	CreateHinge({ -8.0f, 1.0f, 220.0f });
	CreateHinge({ -68.0f, 1.0f, 230.0f });

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	for (uint i = 0; i < map.Count(); ++i) delete map[i];

	//for (uint i = 0; i < HingesBody.Count(); ++i) delete HingesBody[i];
	//for (uint i = 0; i < CheckPoints_List.Count(); ++i) delete CheckPoints_List[i];
	//for (uint i = 0; i < Traps.Count(); ++i) delete Traps[i];
	//for (uint i = 0; i < Boxes_List.count(); i++) delete Boxes_List[i];

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	Cube ground(2000, 1, 2000);

	ground.SetPos(0, -1, 0);
	ground.Render();

	Cube shop2(205, 400, 1);
	shop2.SetPos(-90, 20, -405);
	// roof
	shop2.transform.rotate(90, vec3(1, 0, 0));
	shop2.Render();

	MercaWall1.Render();
	MercaWall2.Render();
	MercaWall3.Render();

	//Render Map 
	for (int i = 0; i < map.Count(); i++) map[i]->Render();
	//Render CheckPoints
	for (int i = 0; i < CheckPoints_List.Count(); i++) CheckPoints_List[i].Render();
	//Render traps
	for (int i = 0; i < Traps.Count(); i++) Traps[i].Render();

	//Update traps position
	for (int i = 1; i < Traps.Count(); i = i + 2) {
		HingesBody[i]->GetTransform(transform.M);
		Traps[i].transform = transform;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->is_sensor == true) {
		
		for (uint i = 0; i < SavePoints.Count(); i++){
			if (body1 == SavePoints[i]) {
				CheckPoints_List[i].color = Red;
				body1->is_sensor = false;
			}
			if (body1 == SavePoints[SavePoints.Count() - 1]) break;	
		
		}
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
	box->color = Blue;
	Boxes_List.add(box);

	phys = App->physics->AddBody(*box, this, 0.0F, true);
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

void ModuleSceneIntro::CreateHinge(vec3 Position) {

	Cube axis;
	axis.size.Set(1, 3, 1);
	axis.color = White;
	axis.SetPos(Position.x, Position.y, Position.z);
	PhysBody3D* bodyA = App->physics->AddBody(axis, 0.0f);

	Cube helix;
	helix.size.Set(7, 2, 1);
	helix.color = Red;
	helix.SetPos((Position.x + 10.0f), (Position.y + 3.5f), (Position.z + 10.0f));
	PhysBody3D* bodyB = App->physics->AddBody(helix, 4.0f);

	HingesBody.PushBack(bodyA);
	HingesBody.PushBack(bodyB);

	Traps.PushBack(axis);
	Traps.PushBack(helix);

	hinge = App->physics->AddConstraintHinge(*bodyA, *bodyB, vec3(0, 0, 0), vec3(5, 0, 0), vec3(0, 1, 0), vec3(0, 1, 0), true);
	hinge->enableAngularMotor(true, 4.0f, INFINITE);
}