#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	
	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);

	Cube cabin(info.cabin_size.x, info.cabin_size.y, info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);

	Cube forwardPart(info.backward_size.x, info.backward_size.y, info.backward_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&forwardPart.transform);

	Cube rightPart(info.right_size.x, info.right_size.y, info.right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rightPart.transform);

	Cube leftPart(info.left_size.x, info.left_size.y, info.left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&leftPart.transform);

	Cube rightHandle(info.rightHandle_size.x, info.rightHandle_size.y, info.rightHandle_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rightHandle.transform);

	Cube leftHandle(info.leftHandle_size.x, info.leftHandle_size.y, info.leftHandle_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&leftHandle.transform);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());

	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(info.backward_offset.x, info.backward_offset.y, info.backward_offset.z);
	offset3 = offset3.rotate(q3.getAxis(), q3.getAngle());

	btQuaternion q4 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset4(info.right_offset.x, info.right_offset.y, info.right_offset.z);
	offset4 = offset4.rotate(q4.getAxis(), q4.getAngle());

	btQuaternion q5 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset5(info.left_offset.x, info.left_offset.y, info.left_offset.z);
	offset5 = offset5.rotate(q5.getAxis(), q5.getAngle());

	btQuaternion q6 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset6(info.rightHandle_offset.x, info.rightHandle_offset.y, info.rightHandle_offset.z);
	offset6 = offset6.rotate(q6.getAxis(), q6.getAngle());

	btQuaternion q7 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset7(info.leftHandle_offset.x, info.leftHandle_offset.y, info.leftHandle_offset.z);
	offset7 = offset7.rotate(q7.getAxis(), q7.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	cabin.transform.M[12] += offset2.getX();
	cabin.transform.M[13] += offset2.getY();
	cabin.transform.M[14] += offset2.getZ();

	forwardPart.transform.M[12] += offset3.getX();
	forwardPart.transform.M[13] += offset3.getY();
	forwardPart.transform.M[14] += offset3.getZ();

	rightPart.transform.M[12] += offset4.getX();
	rightPart.transform.M[13] += offset4.getY();
	rightPart.transform.M[14] += offset4.getZ();

	leftPart.transform.M[12] += offset5.getX();
	leftPart.transform.M[13] += offset5.getY();
	leftPart.transform.M[14] += offset5.getZ();

	rightHandle.transform.M[12] += offset6.getX();
	rightHandle.transform.M[13] += offset6.getY();
	rightHandle.transform.M[14] += offset6.getZ();

	leftHandle.transform.M[12] += offset7.getX();
	leftHandle.transform.M[13] += offset7.getY();
	leftHandle.transform.M[14] += offset7.getZ();

	chassis.color = White;
	cabin.color = White;
	forwardPart.color = White;
	rightPart.color = White;
	leftPart.color = White;
	rightHandle.color = Black;
	leftHandle.color = Black;

	chassis.Render();
	cabin.Render();
	forwardPart.Render();
	rightPart.Render();
	leftPart.Render();
	rightHandle.Render();
	leftHandle.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}