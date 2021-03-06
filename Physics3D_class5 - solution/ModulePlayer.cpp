#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	//Vehicle_Sensor.size.Set(3.0f, 3.0f, 3.0f);
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(4, 0.8, 6);
	car.chassis_offset.Set(0, 0.8, 0);
	car.cabin_size.Set(4, 3, 0.2);
	car.cabin_offset.Set(0, 2, 3);
	car.backward_size.Set(4, 3, 0.2);
	car.backward_offset.Set(0, 2, -3);
	car.right_size.Set(0.2, 3, 6);
	car.right_offset.Set(-2, 2, 0);
	car.left_size.Set(0.2, 3, 6);
	car.left_offset.Set(2, 2, 0);
	car.rightHandle_size.Set(0.3, 0.3, 1);
	car.rightHandle_offset.Set(1.5, 3, -3.3);
	car.leftHandle_size.Set(0.3, 0.3, 1);
	car.leftHandle_offset.Set(-1.5, 3, -3.3);
	car.handle_radius = 0.2;
	car.handle_height = 4;
	car.handle_offset.Set(0, 3, -4);
	

	car.mass = 200.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.5f;
	float wheel_width = 0.3f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(5, 5, -120);

	/*Vehicle_Sensor.size.Set(4.0f, 4.0f, 0.5f);
	Vehicle_Sensor.SetPos(5.0f, 2.0f, -116.5f);
	Vehicle_PB = App->physics->AddBody(Vehicle_Sensor, this, 0.0f, false, PBType::CAR);*/

	winFx = App->audio->LoadFx("audio/Homer.wav");
	loseFx = App->audio->LoadFx("audio/lose.wav");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	VehicleInfo car;
	
	if (App->input->GetKey(SDL_SCANCODE_F2)) 
	{
		RestartGame();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		switch (App->scene_intro->checkpoints_index)
		{
		case 0:
			App->player->RespawnVehicle(vec3(START_POINT), { 0, 0, 0, 1 });
			break;
		case 1:
			App->player->RespawnVehicle(vec3(CHECKPOINT_0), { 0, 1, 0, 0 });
			break;
		case 2:
			App->player->RespawnVehicle(vec3(CHECKPOINT_1), { 0, 1, 0, -1 });
			break;
		case 3:
			App->player->RespawnVehicle(vec3(CHECKPOINT_2), { 0, 1, 0, 0 });
			App->scene_intro->startCountdown = true;
			break;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() >= 150)
			acceleration = 0;
		else
			acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0) brake = BRAKE_POWER;
		else acceleration = -500;
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		Nitro();
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);


	vehicle->Render();
	
	char title[80];
	sprintf_s(title, "%.1f Km/h || Checkpoints: %i || Time: %.1f || Hamburguer To Collect: %i", vehicle->GetKmh(), App->scene_intro->checkpoints_index, App->scene_intro->seconds, App->scene_intro->HamburguerNumber);
	App->window->SetTitle(title);


	return UPDATE_CONTINUE;
}

void ModulePlayer::Nitro()
{
	if (nitro)
	{
		nitroTimer.Start();
		nitro = false;
		nitroTimer.Stop();
	}

	if (nitroTimer.Read() < 300)
	{
		if (vehicle->GetKmh() <= 150)
			acceleration = MAX_ACCELERATION * 10;
		else
			acceleration = 0;
	}
	if (nitroTimer.Read() < 2000)
	{
		nitro = true;
	}

}

void ModulePlayer::RespawnVehicle(vec3 newPos, btQuaternion rotation)
{
	vehicle->SetPos(newPos.x, newPos.y, newPos.z);
	vehicle->SetRotation(rotation);
	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });

}

void ModulePlayer::RestartGame() 
{
	App->player->RespawnVehicle(vec3(START_POINT), { 0, 0 , 0, 1 });
	App->scene_intro->HamburguerNumber = 1;
	App->scene_intro->StopRenderBurguer = false;
	App->scene_intro->checkpoints_index = 0;
	for (uint i = 0; i < App->scene_intro->CheckPoints_List.Count(); i++) App->scene_intro->CheckPoints_List[i].color = White;
	App->scene_intro->endTime = false;
	App->scene_intro->startCountdown = false;
	App->scene_intro->seconds = 30.0f;

	switch (App->scene_intro->BurguerPosition)
	{
	case 0:
		App->scene_intro->SetBurguer(vec3(POSITION_BURGUER_1));
		App->scene_intro->SetBurguerSensor(vec3(POSITION_BURGUER_1));
		break;
	case 1:
		App->scene_intro->SetBurguer(vec3(POSITION_BURGUER_2));
		App->scene_intro->SetBurguerSensor(vec3(POSITION_BURGUER_2));
		break;
	case 2:
		App->scene_intro->SetBurguer(vec3(POSITION_BURGUER_3));
		App->scene_intro->SetBurguerSensor(vec3(POSITION_BURGUER_3));
		break;

	}

	if (App->scene_intro->BurguerPosition == 2)
	{
		App->scene_intro->BurguerPosition = -1;
	}
}
