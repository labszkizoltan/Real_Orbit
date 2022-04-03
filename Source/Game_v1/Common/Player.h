
#ifndef PLAYER_H
#define PLAYER_H

#include <core/scene/CoreComponents.h>
#include <Game_v1/Components/GameComponents.h>


class Player
{
public:
	Player();
	~Player() = default;

	void Update(Timestep ts, Vec3D acceleration);
	void DrawStatsOnScreen();

	void FillReserves();

private:
	Vec3D CalculateColour(float ratio);

public:
	TransformComponent m_Transform;
	DynamicPropertiesComponent m_DynamicProps;
	// some of these should be float, like fuel, because sim speed should affect burn rate
	float m_Health, m_MaxHealth;
	int m_Fuel, m_MaxFuel;
	int m_BulletCount, m_MaxBulletCount;
	float m_BulletSpeed;
	int m_MissilleCount, m_MaxMissilleCount;
	int m_SalvoCount;
	int m_FillRate;

};

#endif // PLAYER_H
