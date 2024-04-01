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
			mCounter++;
			if (mCurrentTime>mLastFpsUpdateTime>=1.0f)
			{
				fpsCount = mCounter;
				mCounter = 0;
				mLastFpsUpdateTime += 1.0f;
			}
		}
		auto static delayTime()->void
		{
			double targetFrameTime = double (1 / (double)frameCap);
			if (deltaTime < targetFrameTime)
			{
				long sleepTime = (targetFrameTime - deltaTime) * 1000.0;
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			}

		}
		auto static fps()->int { return fpsCount; }
		inline double static deltaTime{ 0.0f };
		inline int static frameCap{ 150 };

	private:
		inline static double mCurrentTime{ 0 };
		inline static double mPrevTime{ 0 };
		inline static int mCounter{ 0 };
		inline static int fpsCount = 0.0;
		inline static double mLastFpsUpdateTime = 0.0;
	};
}