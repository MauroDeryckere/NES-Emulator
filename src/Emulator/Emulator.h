#ifndef NES_EMULATOR
#define NES_EMULATOR

#include "NESCPU.h"
#include "NESPPU.h"
#include "NESCartridge.h"

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
		CPU m_Cpu{  };
		PPU m_Ppu{  };
		Cartridge m_Cartridge{  };
	};
}

#endif