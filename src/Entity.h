#pragma once
#include "FlyFish.h"
#include "SDL.h"

struct CollisionBox
{
	float left;
	float bottom;
	float width{};
	float height{};
};

// entity interfaces

/// <summary>
/// Provides an interface for damagable entities
/// </summary>
class IDamagable
{
public:
	~IDamagable() = default;
	virtual void Damage(int damage) = 0;
	virtual int GetHealth() const = 0;
};


/// <summary>
/// Provides an interface for a worldtransform in GEOA. This isn't
/// only used for rendering, but also for physics and movement through
/// the use of a motor.
/// </summary>
class IGeometricTransformable
{
public:
	~IGeometricTransformable() = default;
	
	virtual void UpdateTransform(float elapsedSec) = 0;
	virtual void SetPosition(const ThreeBlade& position) = 0;
	virtual void SetMotor(const Motor& motor) = 0;
	virtual const ThreeBlade& GetPosition() const = 0;
	virtual const Motor& GetMotor() const = 0;
};

/// <summary>
/// Interface for collision objects
/// </summary>
class ICollidable
{
public:
	~ICollidable() = default;
	virtual bool CollideCheck(const ICollidable& collider) = 0;
	virtual void SetCollisionBox(const CollisionBox& collisionBox) = 0;
	virtual const CollisionBox& GetCollisionBox() const = 0;
};

/// <summary>
/// Provides an entity with an interface capable of rendering.
/// </summary>
class IRenderable
{
public:
	~IRenderable() = default;
	virtual void Render() = 0;
};

/// <summary>
/// Provides an entity with the capability of listening for input, especially key events
/// </summary>
class IInputListener
{
public:
	~IInputListener() = default;
	virtual void KeyBoardEvent(const SDL_KeyboardEvent& event) = 0;
	virtual void KeyBoardState() = 0;
};

class Pillar final : public IGeometricTransformable, public ICollidable, public IRenderable, public IDamagable
{
public:

	enum PillarPosition
	{
		LEFT,
		MIDDLE,
		RIGHT
	};

	Pillar(const ThreeBlade& position, float size);

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

	// IDamagable

	virtual void Damage(int damage) override;
	virtual int GetHealth() const override;

	// Pillar

	PillarPosition GetPillarPosition()
	{
		return m_PillarPosition;
	}

	void SetPillarPosition(PillarPosition pillarPosition)
	{
		m_PillarPosition = pillarPosition;
	}

private:
	ThreeBlade m_Position{}; // x,y of which Z is mirror energy
	Motor m_Motor{};
	CollisionBox m_CollisionBox{};
	PillarPosition m_PillarPosition{ PillarPosition::MIDDLE };

	const int m_MaxHealth{ 200 };
	int m_Health{200};

};

class Player final : public IGeometricTransformable, public ICollidable, public IRenderable, public IInputListener, public IDamagable
{
public:
	Player(const ThreeBlade& position, float size, int MaxHealth, Pillar* orbit);

	//IGeometricTransformable

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

	// IInputListener
	virtual void KeyBoardEvent(const SDL_KeyboardEvent& event) override;
	virtual void KeyBoardState() override;

	// IDamagable

	virtual void Damage(int damage) override;
	virtual int GetHealth() const override;

	bool IsMirrorMode();

private:

	void Mirror();

	ThreeBlade m_Position{}; // x,y of which Z is mirror energy
	Motor m_Motor{};
	CollisionBox m_CollisionBox{};
	Pillar* m_OrbitPillar;

	int m_angleMultiplier{};

	bool m_MirrorMode{};

	const int m_MaxHealth{30};
	int m_Health{ m_MaxHealth };

	float m_Speed{ 1.f };
	bool m_SpeedMode{ false };
	bool m_SpeedModeCooldown{ false };

};

