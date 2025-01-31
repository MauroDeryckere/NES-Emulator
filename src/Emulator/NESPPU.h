#ifndef NES_EMULATOR_PPU
#define NES_EMULATOR_PPU

namespace NesEm
{
	class PPU final
	{
	public:
		PPU() = default;
		~PPU() = default;

		PPU(PPU const&) = delete;
		PPU(PPU&&) = delete;
		PPU& operator=(PPU const&) = delete;
		PPU& operator=(PPU&&) = delete;
	private:
	};
}

#endif