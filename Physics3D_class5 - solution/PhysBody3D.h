#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class btRigidBody;
class Module;

enum class PBType {
	NONE = -1,
	CAR,
	CHECKPOINT,
	BOX
};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetAsSensor(bool is_sensor);
	bool IsSensor() const;
	void SetRotation(btQuaternion newQ);

	btRigidBody* GetBody() const;

private:
	btRigidBody* body = nullptr;
	

public:
	p2List<Module*> collision_listeners;
	bool is_sensor;
	PBType PhysBody_Type = PBType::NONE;
};

#endif // __PhysBody3D_H__