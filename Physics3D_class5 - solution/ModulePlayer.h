#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Timer.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 500.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 100.0f
#define START_POINT 5, 5, -120
#define CHECKPOINT_0 -50, 2.5, 135 
#define CHECKPOINT_1 -100, 2.5, -40.0 
#define CHECKPOINT_2 -90.0, 2.5, -250

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	void Nitro();
	bool CleanUp();
	void RestartGame();
	void RespawnVehicle(vec3 newPos, btQuaternion rotation);


public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	Timer nitroTimer;
	bool nitro = true;


	//Cube Vehicle_Sensor;
	//PhysBody3D* Vehicle_PB;

};