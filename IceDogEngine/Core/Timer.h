#pragma once

namespace IceDogCore
{
	class Timer
	{
	public:
		Timer();
		// tick and update the prevTime and delta time
		// tick should called once per frame
		void Tick();

		/* Get DeltaTime */
		double GetDeltaTime() { return r_deltaTime; }

		/* Get FPS */
		int GetFPS() { return r_currentFPS; }

	private:
		int r_currentFPS;
		double r_deltaTime;

	private:
		// save for fps calculation
		double r_fpsTemp;
		int r_fpsTempCalled;
		// save the second count relationship
		double r_secondPerCount;
		// save the preview time for delta time calculation
		__int64 r_prevTime;
	};
}

