#include "Entity.h"
#include "EntityManager.h"
#include "utils.h"

Player::Player(const ThreeBlade& position, float size, int MaxHealth, Pillar* orbit):
	m_Position{ position },
	m_CollisionBox{ m_Position[0], m_Position[1] ,size,size },
	m_Motor{ 1,0,0,0,0,0,0,0 },
	m_MaxHealth{MaxHealth},
	m_Health{ MaxHealth },
	m_OrbitPillar{orbit}
{
}

//IGeometricTransformable
void Player::UpdateTransform(float elapsedSec)
{
	m_Position = (m_Motor * m_Position * (~m_Motor)).Grade3();

	// energy system
	float translateAmount{ 10.f };
	if (m_SpeedMode)
	{
		if (!m_MirrorMode)
		{
			translateAmount = -20.f;
		}
		else
		{
			translateAmount = 20.f;
		}
	}

	Motor energyTranslator = Motor::Translation(elapsedSec * translateAmount, TwoBlade{ 0,0,1,0,0,0 });
	m_Position = (energyTranslator * m_Position * (~energyTranslator)).Grade3();
	if (m_MirrorMode)
	{
		m_Position[2] = SDL_clamp(m_Position[2], -100.f, 0.f);
	}
	else
	{
		m_Position[2] = SDL_clamp(m_Position[2], 0.0f, 100.f);
	}
	if (m_Position[2] == 0 && m_MirrorMode)
	{
		Mirror();
	}
	if (m_Position[2] > 70)
	{
		m_SpeedModeCooldown = false;
	}

	// collision check, credit to Arthur Tirez 2GD11

	OneBlade LeftBorder{ 0,1,0,0 };
	OneBlade RightBorder{ 1280, -1,0,0 };
	OneBlade TopBorder{ 720 , 0, -1,0 };
	OneBlade BottomBorder{ 0,0,1,0 };

	if ((m_Position & LeftBorder) < 0.1f) {
		m_angleMultiplier *= -1;
	}
	if ((m_Position & RightBorder) < 0.1f) {
		m_angleMultiplier *= -1;
	}
	if ((m_Position & BottomBorder) < 0.1f) {
		m_angleMultiplier *= -1;
	}
	if ((m_Position & TopBorder) < 0.1f) {
		m_angleMultiplier *= -1;
		
	}

	const ThreeBlade& orbitPillarPosition = m_OrbitPillar->GetPosition();
	TwoBlade rotationAxis = TwoBlade::LineFromPoints(orbitPillarPosition[0], orbitPillarPosition[1], 0, orbitPillarPosition[0], orbitPillarPosition[1], 1);
	Motor pillarRotator = Motor::Rotation(1 * m_angleMultiplier * m_Speed, rotationAxis);
	Motor pillarTranslator = Motor::Translation(orbitPillarPosition.VNorm(), TwoBlade{ orbitPillarPosition[0],orbitPillarPosition[1],0,0,0,0 });
	m_Motor = pillarTranslator * pillarRotator * (~pillarTranslator);

	m_Position = (m_Motor * m_Position * (~m_Motor)).Grade3();

	SetCollisionBox(CollisionBox{ m_Position[0] - m_CollisionBox.width / 2, m_Position[1] - m_CollisionBox.height / 2, m_CollisionBox.width, m_CollisionBox.height });
}

void Player::SetPosition(const ThreeBlade& position)
{
	m_Position = position;
}


void Player::SetMotor(const Motor& motor)
{
	m_Motor = motor;
}

const ThreeBlade& Player::GetPosition() const
{
	return m_Position;
}

const Motor& Player::GetMotor() const
{
	return m_Motor;
}

// ICollidable

bool Player::CollideCheck(const ICollidable& collider)
{
	CollisionBox a = this->GetCollisionBox();
	CollisionBox b = collider.GetCollisionBox();

	bool overlapX = (a.left < (b.left + b.width) && (a.left + a.width) > b.left);
	bool overlapY = (a.bottom < (b.bottom + b.height)) && (a.bottom + a.height > b.bottom);

	// Collision occurs if both axes overlap
	return overlapX && overlapY;
}

void Player::SetCollisionBox(const CollisionBox& collisionBox)
{
	m_CollisionBox = collisionBox;
}

const CollisionBox& Player::GetCollisionBox() const
{
	return m_CollisionBox;
}

// IRenderable

void Player::Render()
{

	utils::SetColor(Color4f{ 0.5,0.5,0.5,1.f });
	utils::FillRect(Rectf{ 10,10, 300,40 });

	if (m_MirrorMode)
	{
		utils::SetColor(Color4f{ 1,1,1,((float) m_Health/m_MaxHealth) });
	}
	else
	{
		utils::SetColor(Color4f{ 0,0,0, ((float) m_Health / m_MaxHealth) });
	}
	utils::FillRect(m_Position[0]-m_CollisionBox.width/2, m_Position[1] - m_CollisionBox.height/2, m_CollisionBox.width, m_CollisionBox.height);

	float percentage = fabsf(m_Position[2] / 100.f);
	utils::FillRect(Rectf{ 11,11, (300*percentage)-1,39});

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawPoint(m_Position[0], m_Position[1], 3.f);

	
	
}

// IInputListener
void Player::KeyBoardEvent(const SDL_KeyboardEvent& event)
{
	if (event.keysym.sym == SDLK_UP)
	{
		if (!m_MirrorMode && m_Position[2] < 100) return;
		Mirror();
	}
	if (event.keysym.sym == SDLK_DOWN) {
		if (m_MirrorMode)
		{
			EntityManager::Get()->AddProjectile(m_Position, m_OrbitPillar);
		}
	}
}
 
void Player::KeyBoardState()
{
	const Uint8* keyBoardState = SDL_GetKeyboardState(nullptr);

	bool IsLeftDown = keyBoardState[SDL_SCANCODE_LEFT];
	bool IsRightDown = keyBoardState[SDL_SCANCODE_RIGHT];
	bool isShiftDown = keyBoardState[SDL_SCANCODE_LSHIFT];

	bool A = keyBoardState[SDL_SCANCODE_A];
	bool D = keyBoardState[SDL_SCANCODE_D];

	m_angleMultiplier = 0;

	if (IsLeftDown) m_angleMultiplier = 1;
	if (IsRightDown) m_angleMultiplier = -1;

	if (isShiftDown && !m_SpeedModeCooldown)
	{
		if (!m_Position[2] == 0)
		{
			m_SpeedMode = true;
			m_Speed = 2.f;
		}
		else
		{
			m_SpeedModeCooldown = true;
		}
	}
	else
	{
		m_SpeedMode = false;
		m_Speed = 1.f;
	}

	if (A)
	{
		if (!m_MirrorMode)
		{
			m_OrbitPillar->SetMotor(Motor::Translation(1.f, TwoBlade{ -1,0,0,0,0,0 }));
			Motor energyTranslator = Motor::Translation(-10, TwoBlade{ 0,0,1,0,0,0 });
			m_Position = (energyTranslator * m_Position * (~energyTranslator)).Grade3();
		}

	}
	else if (D)
	{
		if (!m_MirrorMode)
		{
			m_OrbitPillar->SetMotor(Motor::Translation(1.f, TwoBlade{ 1,0,0,0,0,0 }));
			Motor energyTranslator = Motor::Translation(-10, TwoBlade{ 0,0,1,0,0,0 });
			m_Position = (energyTranslator * m_Position * (~energyTranslator)).Grade3();
		}
	}
	else
	{

	m_OrbitPillar->SetMotor(Motor{1,0,0,0,0,0,0,0});
	}

	const ThreeBlade& orbitPillarPosition = m_OrbitPillar->GetPosition();
	TwoBlade rotationAxis = TwoBlade::LineFromPoints(orbitPillarPosition[0], orbitPillarPosition[1], 0, orbitPillarPosition[0], orbitPillarPosition[1], 1);
	Motor pillarRotator = Motor::Rotation(1 * m_angleMultiplier * m_Speed, rotationAxis);
	Motor pillarTranslator = Motor::Translation(orbitPillarPosition.VNorm(), TwoBlade{orbitPillarPosition[0],orbitPillarPosition[1],0,0,0,0});
	m_Motor = pillarTranslator * pillarRotator * (~pillarTranslator);

	



}

// IDamagable

void Player::Damage(int damage)
{
	m_Health = SDL_clamp(m_Health - damage, 0, m_MaxHealth);
}

int Player::GetHealth() const
{
	return m_Health;
}

bool Player::IsMirrorMode()
{
	return m_MirrorMode;
}

void Player::Mirror()
{
	const ThreeBlade& orbitPillarPosition = m_OrbitPillar->GetPosition();
	TwoBlade normalLine = orbitPillarPosition & m_Position;
	OneBlade mirrorPlane = (normalLine | orbitPillarPosition).Normalized();
	m_Position = (mirrorPlane * m_Position * (~mirrorPlane)).Grade3();
	m_MirrorMode = !m_MirrorMode;
}
