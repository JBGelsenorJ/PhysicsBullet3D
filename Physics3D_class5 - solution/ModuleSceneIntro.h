#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;
enum class PBType;

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

	Cylinder CreateCylinder(float radius, float height, vec3 pos, Color color, bool flip, float angle, vec3 u);
	void CreateRect(const float& x, const float& y, const float& z, const float& width, const float& length, const Cube& cube, ORIENTATION orientation = ORIENTATION::NORTH);	
	void CreateRamp(const float x, const float y, const float z, float mass, float angle, vec3 angle_rot, bool is_collider);
	void CreateCurve(const float& x, const float& y, const float& z, const float& width, const float& angle_i, const float& angle_f, const Cube& cube, const float& radius);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void SetBox(const vec3);
	void CreateCheckPoint(const vec3 Position, float angle);
	void CreateHinge(vec3 Position, float speed, int way);
	void TimeToLoseUpdate(float dt);

public:

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2DynArray<Sphere> CheckPoints_List;
	p2DynArray<PhysBody3D*> SavePoints;
	p2DynArray<Cube> Traps;
	p2DynArray<PhysBody3D*> HingesBody;
	p2List<Cube> ramp;

	p2DynArray <Cube*> cubes;
	p2List<Cylinder> cylinders;

	Cube MercaWall1;
	PhysBody3D* MercaWall1pb = nullptr;
	Cube MercaWall2;
	PhysBody3D* MercaWall2pb = nullptr;
	Cube MercaWall3;
	PhysBody3D* MercaWall3pb = nullptr;
	Cube MercaWall4;
	PhysBody3D* MercaWall4pb = nullptr;
	Cube MercaEntrance;
	PhysBody3D* MercaEntrancepb = nullptr;
	Cube start;
	PhysBody3D* startCube = nullptr;
	Cube box;
	PhysBody3D* pb_box = nullptr;
	int box_position = 0;

	//Hinges
	PhysBody3D* bodyA = nullptr;
	Cube axis;
	PhysBody3D* bodyB;
	Cube helix;
	btHingeConstraint* hinge = nullptr;
	mat4x4 transform;
	Cube firstRamp;
	Cube secondRamp;
	uint counter;
	uint checkpoints_index = 0;
	int boxes = 5;

	int minutes = 0;
	float seconds = 0;
	bool endTime = false;
	bool startCountdown = false;


private:

	Cube road;
	Cube roadForCurve;
	Sphere CheckPointLight;
	int roadWidth;
	int radius;

};
