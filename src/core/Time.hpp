#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace DazaiEngine
{
	static class Time
	{
	public:
		auto static updateDeltaTime() -> void
		{
			mCurrentTime = glfwGetTime();
			deltaTime = mCurrentTime - mPrevTime;
			mPrevTime = mCurrentTime;
		}
		auto static delayTime()->void
		{
			double targetFrameTime = double (1 / (double)frameCap);
			if (deltaTime < targetFrameTime)
			{
				long sleepTime = targetFrameTime - deltaTime;
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			}
		}
		inline double static deltaTime{ 0.0f };
		inline int static frameCap{ 60 };

	private:
		inline static double mCurrentTime{ 0 };
		inline static double mPrevTime{ 0 };
	};
}