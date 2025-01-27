#ifndef NES_EMULATOR
#define NES_EMULATOR

#include "CPU.h"

namespace NesEm
{
	class Emulator final
	{
	public:
		Emulator() = default;
		~Emulator() = default;

		void Run() noexcept;

		Emulator(Emulator const&) = delete;
		Emulator(Emulator&&) = delete;
		Emulator& operator=(Emulator const&) = delete;
		Emulator& operator=(Emulator&&) = delete;
	private:
		CPU m_Cpu{};
	};
}

#endif