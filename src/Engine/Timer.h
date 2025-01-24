#ifndef NES_EMULATOR_TIME
#define NES_EMULATOR_TIME

#include "Singleton.h"
#pragma warning (push)
#pragma warning (disable: 4820)
#pragma warning (disable: 4514)
#pragma warning (disable: 4548)
	#include <SDL3/SDL.h>
#pragma warning (pop)

namespace NesEm
{
	class GameTime final : public Singleton<GameTime>
	{
	public:
		[[nodiscard]] inline float ElapsedSec() const noexcept { return m_ElapsedSec; }
		[[nodiscard]] inline float FixedTimeStep() const { return (m_MsFixedTimeStep / 1000.f); }

		[[nodiscard]] inline bool IsLag() const noexcept { return m_MsLag >= m_MsFixedTimeStep; }
		inline void ProcessLag() noexcept { m_MsLag -= m_MsFixedTimeStep; }

		inline void Update() noexcept
		{
			const auto currentTime = SDL_GetPerformanceCounter();
			m_ElapsedSec = static_cast<float>((currentTime - m_LastTime) * m_SecondsPerCount);
			if (m_ElapsedSec < 0.0f)
				m_ElapsedSec = 0.0f;

			m_MsLag += m_ElapsedSec * 1000.f;
			m_LastTime = currentTime;
		}

		[[nodiscard]] inline auto SleepTime() const noexcept
		{
			const auto sleepTime = m_LastTime * m_SecondsPerCount + m_MsPerFrame - SDL_GetPerformanceCounter() * m_SecondsPerCount;
			return (sleepTime > 0.0f) ? static_cast<uint32_t>(sleepTime) : 0u;
		}

		void SetFPS(float fps) noexcept
		{
			m_MsPerFrame = 1000.f / fps;
			m_MsFixedTimeStep = m_MsPerFrame + 3.3f; // hard coded value, based on 60 FPS may need to be adjusted in future
		}

		GameTime(const GameTime&) = delete;
		GameTime(GameTime&&) = delete;
		GameTime& operator=(const GameTime&) = delete;
		GameTime& operator=(const GameTime&&) = delete;

	private:
		friend class NesEm::Singleton<GameTime>;
		GameTime() = default;
		~GameTime() = default;

		uint64_t m_LastTime{ SDL_GetPerformanceCounter() };

		const double m_SecondsPerCount{ 1.0 / static_cast<double>(SDL_GetPerformanceFrequency()) };

		float m_MsPerFrame{ 16.7f };
		float m_MsFixedTimeStep{ 20.f };

		float m_ElapsedSec{ 0.f };
		float m_MsLag{ 0.f };
	};
}

#endif