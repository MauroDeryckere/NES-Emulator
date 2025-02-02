#ifndef NES_EMULATOR
#define NES_EMULATOR

#include "emulator_pch.h"

#include "NESCPU.h"
#include "NESPPU.h"
#include "NESCartridge.h"


/* Various sources used during development of the Emulator class of our emulator:
 * https://www.nesdev.org/wiki/Cycle_reference_chart
 *
 */

namespace NesEm
{
	class Emulator final
	{
	public:
		Emulator();
		~Emulator() = default;

		void Run() noexcept;

		void Reset() noexcept;

		Emulator(Emulator const&) = delete;
		Emulator(Emulator&&) = delete;
		Emulator& operator=(Emulator const&) = delete;
		Emulator& operator=(Emulator&&) = delete;

	private:
		PPU m_PPU;
		CPU m_CPU;
		Cartridge m_Cartridge;

		uint64_t m_MasterClock{ 0 };
	};
}

#endif