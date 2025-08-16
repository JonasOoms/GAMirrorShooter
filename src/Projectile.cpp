#include "Projectile.h"
#include "utils.h"
#include "SDL_opengl.h"
Projectile::Projectile(const ThreeBlade& position, float size, Pillar* target):
	m_Position{ ThreeBlade{position[0], position[1], 0}},
	m_CollisionBox{ m_Position[0] - size/2, m_Position[1] - size/2 ,size,size },
	m_Motor{ 1,0,0,0,0,0,0,0 },
	m_TrackedTarget{target}
{
	
}

void Projectile::UpdateTransform(float elapsedSec)
{
	const ThreeBlade& pillarPosition = m_TrackedTarget->GetPosition();
	const ThreeBlade& position = ThreeBlade(m_Position[0], m_Position[1],0);
	TwoBlade line = TwoBlade::LineFromPoints(pillarPosition[0], pillarPosition[1],0, position[0], position[1], 0);
	m_Motor = Motor::Translation(10.f, TwoBlade{-line[3], -line[4], 0,0,0,0});
	m_Position = (m_Motor * (m_Position) * (~m_Motor)).Grade3();
	m_CollisionBox = CollisionBox{ m_Position[0] - m_CollisionBox.width/ 2, m_Position[1] - m_CollisionBox.height / 2 , m_CollisionBox.width,m_CollisionBox.height};
}

void Projectile::SetPosition(const ThreeBlade& position)
{
	m_Position = position;
}

void Projectile::SetMotor(const Motor& motor)
{
	m_Motor = motor;
}

const ThreeBlade& Projectile::GetPosition() const
{
	return m_Position;
}

const Motor& Projectile::GetMotor() const
{
	return m_Motor;
}

bool Projectile::CollideCheck(const ICollidable& collider)
{
	return false;
}

void Projectile::SetCollisionBox(const CollisionBox& collisionBox)
{
	m_CollisionBox = collisionBox;
}

const CollisionBox& Projectile::GetCollisionBox() const
{
	return m_CollisionBox;
}

void Projectile::Render()
{
	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::FillEllipse(m_Position[0], m_Position[1], m_CollisionBox.width, m_CollisionBox.height);
}








EnemyProjectile::EnemyProjectile(const ThreeBlade& position, float size, const ThreeBlade& trackedPosition):
	Projectile(position, size, nullptr),
	m_TrackedPosition{ trackedPosition }
{
}

void EnemyProjectile::UpdateTransform(float elapsedSec)
{
	const ThreeBlade& pillarPosition = m_TrackedPosition;
	const ThreeBlade& position = ThreeBlade(m_Position[0], m_Position[1], 0);
	TwoBlade line = TwoBlade::LineFromPoints(pillarPosition[0], pillarPosition[1], 0, position[0], position[1], 0);
	m_Motor = Motor::Translation(0.3f, TwoBlade{ -line[3], -line[4], 0,0,0,0 });
	m_Position = (m_Motor * (m_Position) * (~m_Motor)).Grade3();
	m_CollisionBox = CollisionBox{ m_Position[0] - m_CollisionBox.width / 2, m_Position[1] - m_CollisionBox.height / 2 , m_CollisionBox.width,m_CollisionBox.height };
}

void EnemyProjectile::Render()
{
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::FillEllipse(m_Position[0], m_Position[1], m_CollisionBox.width, m_CollisionBox.height);
}
