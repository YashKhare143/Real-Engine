#pragma once

namespace RealEngine {
	class Timestep {
	public:
		Timestep(float time = 0.0f) : m_Time(time){}

		operator float() const { return m_Time; };

		inline float GetDeltaTime() const { return m_Time; };
		inline float GetMilliSeconds() const { return m_Time * 1000.0f; };
	private:
		float m_Time;
	};
}
