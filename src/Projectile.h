#pragma once
#include "Entity.h"

class Projectile : public IGeometricTransformable, public ICollidable, public IRenderable
{
public:

	Projectile(const ThreeBlade& position, float size, Pillar* target);

	// IGeometricTransformable
	virtual void UpdateTransform(float elapsedSec) override;
	virtual void SetPosition(const ThreeBlade& position) override;
	virtual void SetMotor(const Motor& motor) override;
	virtual const ThreeBlade& GetPosition() const override;
	virtual const Motor& GetMotor() const override;

	// ICollidable

	virtual bool CollideCheck(const ICollidable& collider) override;
	virtual void SetCollisionBox(const CollisionBox& collisionBox) override;
	virtual const CollisionBox& GetCollisionBox() const override;

	// IRenderable

	virtual void Render() override;

protected:
	float m_Speed{ 1000.f };
	ThreeBlade m_Position{}; // x,y of which Z is mirror energy
	Motor m_Motor{};
	CollisionBox m_CollisionBox{};
	Pillar* m_TrackedTarget;
};

class EnemyProjectile : public Projectile
{
public:
	EnemyProjectile(const ThreeBlade& position, float size, const ThreeBlade& trackedPosition);

	// IGeometricTransformable
	virtual void UpdateTransform(float elapsedSec) override;
	virtual void Render() override;
	virtual const ThreeBlade& GetTrackedPosition() {
		return m_TrackedPosition;
	};

	ThreeBlade m_TrackedPosition;

};