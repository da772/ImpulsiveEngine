#pragma once

namespace GEngine {

	class GE_API Timestep {
	public:
		Timestep(double time = 0.0)
			: m_Time(time) {

		}

		double GetSeconds() const { return m_Time; }
		double GetMilliseconds() const { return m_Time * 1000.0; }

		operator double() const { return m_Time; }

	private:
		double m_Time;
	};

}