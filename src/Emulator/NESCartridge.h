#ifndef NES_EMULATOR_CARTRIDGE
#define NES_EMULATOR_CARTRIDGE

namespace NesEm
{
	class Cartridge final
	{
	public:
		Cartridge() = default;
		~Cartridge() = default;

		Cartridge(Cartridge const&) = delete;
		Cartridge(Cartridge&&) = delete;
		Cartridge& operator=(Cartridge const&) = delete;
		Cartridge& operator=(Cartridge&&) = delete;
	private:
	};
}

#endif