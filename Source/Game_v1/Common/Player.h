
#ifndef PLAYER_H
#define PLAYER_H

#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>


class Player
{
public:
	Player() = default;
	~Player() = default;

	void Update();
	void DrawStatsOnScreen();

public:
	TransformComponent m_Transform;
	DynamicPropertiesComponent m_DynamicProps;
	int m_BulletCount, m_MaxBulletCount, m_BulletSpeed;
	int m_MissilleCount, m_MaxMissilleCount;
	int m_Health, m_MaxHealth;

};

#endif // PLAYER_H
