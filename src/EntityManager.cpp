#include "EntityManager.h"
#include <random>

EntityManager* EntityManager::INSTANCE = nullptr;

EntityManager::EntityManager()
{

}

EntityManager* EntityManager::Get()
{
	if (INSTANCE)
	{
		return INSTANCE;
	}
	INSTANCE = new EntityManager();
}

void EntityManager::Update(float elapsedSec)
{

	m_projectileRingTimer += elapsedSec;

	m_Pillar->UpdateTransform(elapsedSec);
	m_Player->KeyBoardState();
	m_Player->UpdateTransform(elapsedSec);

	for (std::vector<Projectile>::iterator it = m_Projectiles.begin(); it != m_Projectiles.end();)
	{
		(*it).UpdateTransform(elapsedSec);
		if (m_Pillar->CollideCheck(*it))
		{
			it = m_Projectiles.erase(it);
			m_Pillar->Damage(1);
		}
		else
		{
			++it;
		}
	}

	for (std::vector<EnemyProjectile>::iterator it = m_EnemyProjectiles.begin(); it != m_EnemyProjectiles.end();)
	{
		(*it).UpdateTransform(elapsedSec);

		float distance = TwoBlade::LineFromPoints(it->GetPosition()[0], it->GetPosition()[1], 0, it->GetTrackedPosition()[0], it->GetTrackedPosition()[1], 0).Norm();

		if (m_Player->CollideCheck(*it))
		{
			it = m_EnemyProjectiles.erase(it);
			m_Player->Damage(1);
		}
		else if (distance < 0.2f)
		{
			it = m_EnemyProjectiles.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (m_projectileRingTimer > 10.f)
	{
		m_projectileRingTimer = 0.f;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(40, 60);

		std::bernoulli_distribution d(0.5); 

		
		bool randomBool = d(gen);

		int multiplier = randomBool ? -1 : 1;
		
		ThreeBlade pillarPosition = m_Pillar->GetPosition();
		ThreeBlade randomCenter{pillarPosition[0] + multiplier*(distrib(gen)), pillarPosition[1], 0};

		float angleIncrement = 2 * M_PI / 20;
		float radius = 400;

		for (int i{}; i < 60; ++i)
		{
			float theta = i * angleIncrement;
			float x = randomCenter[0] + radius * cosf(theta);
			float y = randomCenter[1] + radius * sinf(theta);

			AddEnemyProjectile(ThreeBlade{ x,y,0 }, randomCenter);

		}

	}

}

void EntityManager::Render()
{
	m_Pillar->Render();
	m_Player->Render();

	for (Projectile& projectile : m_Projectiles)
	{
		projectile.Render();
	}

	for (EnemyProjectile& projectile : m_EnemyProjectiles)
	{
		projectile.Render();
	}
}

void EntityManager::AddPillar(const ThreeBlade& position, float size)
{
	m_Pillar = new Pillar(position, size);
}

void EntityManager::AddPlayer(const ThreeBlade& position, float size, int MaxHealth)
{
	m_Player = new Player(position, size, MaxHealth, m_Pillar);
}

void EntityManager::AddProjectile(const ThreeBlade& position, Pillar* target)
{
	m_Projectiles.emplace_back(position, 10, target);
}

void EntityManager::AddEnemyProjectile(const ThreeBlade& position, const ThreeBlade& trackedPosition)
{
	m_EnemyProjectiles.emplace_back(position, 10, trackedPosition);
}

