
#ifndef TIMESTEP_H
#define TIMESTEP_H

class Timestep
{
public:
	Timestep(float time = 0.0f)
		: m_Time(time)
	{
	}

	operator float() const { return m_Time; } // this is like a static cast operator that lets us to cast this object implicitely

	float GetSeconds() const { return m_Time; }
	float GetMiliSeconds() const { return m_Time * 1000.0f; }

private:
	float m_Time = 0.0f; // stores in seconds

};


#endif // TIMESTEP_H
