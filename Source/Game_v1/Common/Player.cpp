
#include "Player.h"
#include <Game_v1/GameApp_v1.h>

Player::Player()
{
	m_Transform.location = Vec3D(-1.0, 5.2, -1.0);
	m_Transform.orientation.f1 = Vec3D(-0.0333719775, -0.998181820, -0.0501891039);
	m_Transform.orientation.f2 = Vec3D(0.102339417, 0.0465401821, -0.993658841);
	m_Transform.orientation.f3 = Vec3D(0.994188845, -0.0382963605, 0.100600392);
	m_DynamicProps.velocity = Vec3D(0,0,0);
	m_DynamicProps.inertial_mass = 1.0f;
	m_DynamicProps.angular_velocity = Vec3D(0, 0, 0);

	m_Health = m_MaxHealth = 1000;
	m_Fuel = m_MaxFuel = 10000;
	m_BulletCount = m_MaxBulletCount = 5000;
	m_BulletSpeed = 0.05f;
	m_MissilleCount = m_MaxMissilleCount = 180;
	m_SalvoCount = 0;
	m_FillRate = 10;
}

void Player::Update(Timestep ts, Vec3D acceleration)
{
	m_Transform.location += ts * m_DynamicProps.velocity;
	m_DynamicProps.velocity += ts * acceleration;
}

void Player::DrawStatsOnScreen()
{
	static int windowHeight = GameApplication::Get().GetWindow().GetHeight();

	GameApplication::Game_DrawText("Health: " + std::to_string(m_Health) + " / " + std::to_string(m_MaxHealth),
		Vec3D(10, windowHeight - 110, 0),
		CalculateColour((float)m_Health / m_MaxHealth),
		0.5f);
	GameApplication::Game_DrawText("Fuel: " + std::to_string(m_Fuel) + " / " + std::to_string(m_MaxFuel),
		Vec3D(10, windowHeight - 140, 0),
		CalculateColour((float)m_Fuel / m_MaxFuel),
		0.5f);
	GameApplication::Game_DrawText("Bullets: " + std::to_string(m_BulletCount) + " / " + std::to_string(m_MaxBulletCount),
		Vec3D(10, windowHeight - 170, 0),
		CalculateColour((float)m_BulletCount / m_MaxBulletCount),
		0.5f);
	GameApplication::Game_DrawText("Missilles: " + std::to_string(m_MissilleCount) + " / " + std::to_string(m_MaxMissilleCount),
		Vec3D(10, windowHeight - 200, 0),
		CalculateColour((float)m_MissilleCount / m_MaxMissilleCount),
		0.5f);


}

void Player::FillReserves()
{
	m_Health = std::min(m_Health + m_FillRate / 2, m_MaxHealth);
	m_Fuel = std::min(m_Fuel + m_FillRate, m_MaxFuel);
	m_BulletCount = std::min(m_BulletCount + m_FillRate, m_MaxBulletCount);
	m_MissilleCount = std::min(m_MissilleCount + m_FillRate/5, m_MaxMissilleCount);
}

Vec3D Player::CalculateColour(float ratio)
{
//	float r = 0.5f < ratio ? 2 * (1 - ratio) : 1.0f;
//	float g = 0.5f < ratio ? 1.0f : 2 * ratio;
	float r = 0.5f < ratio ? 4 * (ratio - ratio* ratio) : 1.0f;
	float g = 0.5f < ratio ? 1.0f : 4 * (ratio - ratio * ratio);
	return Vec3D(r, g, 0.0f);
}