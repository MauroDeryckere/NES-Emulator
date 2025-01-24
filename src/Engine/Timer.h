#ifndef NES_EMULATOR_TIME
#define NES_EMULATOR_TIME

#include "Singleton.h"
#include <chrono>

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
			using namespace std::chrono;

			const auto currentTime = high_resolution_clock::now();
			m_ElapsedSec = duration<float>(currentTime - m_LastTime).count();

			m_MsLag += m_ElapsedSec * 1000.f;
			m_LastTime = std::chrono::high_resolution_clock::now();
		}

		[[nodiscard]] inline auto SleepTime() const noexcept
		{
			return (m_LastTime + std::chrono::milliseconds(static_cast<uint64_t>(m_MsPerFrame)) - std::chrono::high_resolution_clock::now());
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

		std::chrono::steady_clock::time_point m_LastTime{ std::chrono::high_resolution_clock::now() };

		float m_MsPerFrame{ 16.7f };
		float m_MsFixedTimeStep{ 20.f };

		float m_ElapsedSec{ 0.f };
		float m_MsLag{ 0.f };
	};
}

#endif