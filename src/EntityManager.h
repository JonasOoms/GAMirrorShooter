#pragma once
#include "Entity.h"
#include <vector>
#include "Projectile.h"

class EntityManager final
{
public:
	static EntityManager* Get();
	
	void Update(float elapsedSec);
	void Render();

	void AddPillar(const ThreeBlade& position, float size);
	void AddPlayer(const ThreeBlade& position, float size, int MaxHealth);
	void AddProjectile(const ThreeBlade& position, Pillar* target);
	void AddEnemyProjectile(const ThreeBlade& position, const ThreeBlade& trackedPosition);

	Player* GetPlayer() { return m_Player; };

private:
	EntityManager();
	static EntityManager* INSTANCE;

	Player* m_Player;
	Pillar* m_Pillar;
	std::vector<Projectile> m_Projectiles;
	std::vector<EnemyProjectile> m_EnemyProjectiles;

	float m_projectileRingTimer{};

};