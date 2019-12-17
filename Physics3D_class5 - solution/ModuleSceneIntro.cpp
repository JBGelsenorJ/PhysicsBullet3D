#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	App->audio->PlayMusic("music/mercadonaremix.ogg");

	LOG("Loading Intro assets");
	bool ret = true;

	//Road creation
	Cube road(2, 4, 2);
	road.color.Set(0.0f, 1.0f, 0.0f);
	int roadWidth = 20;
	int radius = 20;

	/*
	Cube road(2, 5, 2);
	Cube shop1(100, 40, 1);
	road.color.Set(0.0f, 0.0f, 0.0f);
	int roadWidth = 20;
	
	CreateRect(-10.0f, 0, 0, roadWidth, 50, road, ORIENTATION::NORTH);
	CreateRect(-55, 0, 100, 110, 1, shop1, ORIENTATION::NORTH);
	
	
	//Boxes
	p2List_item<Cube*>* Box_List;
	Cube box(5,5,5);
	box.SetPos(0, 0, 0);
	box.color = Red;
	App->physics->AddBody(box, 1.0f);
	*/

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
	CreateRect(-100.0f, 0, -245, roadWidth, 70, road, ORIENTATION::NORTH);
	//CreateCurve(-30.0f, 0, -235, roadWidth, 180, 360, road, radius);
	CreateRect(-10.0f, 0, -210, roadWidth, 110, road, ORIENTATION::NORTH);
	
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
	/*Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();*/

	Cube ground(1000, 2, 1000);
	ground.SetPos(0, -1, 0);
	ground.Render();

	/*Cube shop2(100, 40, 1);
	shop2.SetPos(100, 10, 155);
	// para techo
	//shop2.transform.rotate(90, vec3(1, 0, 0));
	map.PushBack(&shop2);
	shop2.transform.rotate(90, vec3(0, 1, 0));
	shop2.Render();
	*/

	//Render Map 
	for (int i = 0; i < map.Count(); i++)
	{
		map[i]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
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

void ModuleSceneIntro::SetBoxes(const float& x, const float& y, const float&z) {
	Cube* box = new Cube;
	box->SetPos(x, y, z);
	box->color.Set(1.0f, 0.0f, 0.0f);
	Boxes_List.add(box);
}

void ModuleSceneIntro::CheckBoxes() {

	//This function will check last box that have been triggered and will store data of spawn if user press any key 
	//WIP
	//p2List_item<Cube*>* Actual_Cubes = Boxes_List.start;
}