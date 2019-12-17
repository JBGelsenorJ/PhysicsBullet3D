#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

enum class ORIENTATION
{
	NORTH,		//positive Z axis
	SOUTH,		//negative Z axis
	EAST,		//negative X azis
	WEST,		//positive X axis

	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void CreateRect(const float& x, const float& y, const float& z, const float& width, const float& length, const Cube& cube, ORIENTATION orientation = ORIENTATION::NORTH);
	void CreateCurve(const float& x, const float& y, const float& z, const float& width, const float& angle_i, const float& angle_f, const Cube& cube, const float& radius);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	//This function will create and set all boxes in map, later will be added to a item list
	void SetBoxes(const vec3);
	void CheckBoxes();
	void CreateCheckPoint(const vec3 Position, float angle);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	p2DynArray <Cube*> map;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	//Will be used to store all box information that we will spawn in map. Color_Pos_Sensors...
	p2List<Cube*> Boxes_List;

	//Hinges
	PhysBody3D* bodyA = nullptr;
	Cube axis;
	PhysBody3D* bodyB;
	Cube helix;
	btHingeConstraint* hinge = nullptr;

private:

	Cube road;
	Sphere CheckPointLight;
	int roadWidth;
	int radius;
};
