#include "Entity.h"
#include "Utils.h"

Pillar::Pillar(const ThreeBlade& position, float size) :
	m_Position{ position },
	m_CollisionBox{m_Position[0], m_Position[1] ,size,size},
	m_Motor{1,0,0,0,0,0,0,0}
{
}

void Pillar::UpdateTransform(float elapsedSec)
{
	m_Position = (m_Motor * m_Position * (~m_Motor)).Grade3();
	m_CollisionBox = { m_Position[0], m_Position[1] ,m_CollisionBox.width,m_CollisionBox.height };
}

void Pillar::SetPosition(const ThreeBlade& position)
{
	m_Position = position;
}

void Pillar::SetMotor(const Motor& motor)
{
	m_Motor = motor;
}

const ThreeBlade& Pillar::GetPosition() const
{
	return m_Position;
}

const Motor& Pillar::GetMotor() const
{
	return m_Motor;
}

bool Pillar::CollideCheck(const ICollidable& collider)
{
	CollisionBox a = this->GetCollisionBox();
	CollisionBox b = collider.GetCollisionBox();

	bool overlapX = (a.left < (b.left + b.width) && (a.left + a.width) > b.left);
	bool overlapY = (a.bottom < (b.bottom+b.height)) && (a.bottom+a.height > b.bottom);

	// Collision occurs if both axes overlap
	return overlapX && overlapY;
}

void Pillar::SetCollisionBox(const CollisionBox& collisionBox)
{
	m_CollisionBox = collisionBox;
}

const CollisionBox& Pillar::GetCollisionBox() const
{
	return m_CollisionBox;
}

void Pillar::Render()
{
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::FillRect(m_CollisionBox.left - m_CollisionBox.width/2, m_CollisionBox.bottom - m_CollisionBox.height/2, m_CollisionBox.width, m_CollisionBox.height);

	utils::SetColor(Color4f{ 0.5,0.5,0.5,1 });
	utils::FillRect(m_CollisionBox.left - m_CollisionBox.width/2 - 10, m_CollisionBox.bottom + 20, m_CollisionBox.width + 20, 10);

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::FillRect(m_CollisionBox.left - m_CollisionBox.width / 2 - 10, m_CollisionBox.bottom + 20, (m_CollisionBox.width + 20)*((float) m_Health/m_MaxHealth), 10);

	utils::SetColor(Color4f{ 1,1,1,1 });
}

void Pillar::Damage(int damage)
{
	m_Health = SDL_clamp(m_Health - damage, 0, m_MaxHealth);
}

int Pillar::GetHealth() const
{
	return m_Health;
}


