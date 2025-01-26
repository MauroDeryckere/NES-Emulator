#ifndef NES_EMULATOR_MEMORY
#define NES_EMULATOR_MEMORY

#include <cstdint>
#include <array>

namespace NesEm
{
	class NESMemory final
	{
	public:
		NESMemory() = default;
		~NESMemory() = default;

		NESMemory(NESMemory const&) = delete;
		NESMemory(NESMemory&&) = delete;
		NESMemory& operator=(NESMemory const&) = delete;
		NESMemory& operator=(NESMemory&&) = delete;

		//TODO
		uint8_t Read(uint16_t address) const noexcept { return 0; };
		void Write(uint16_t address, uint8_t value) noexcept {};

	private:
		std::array<uint8_t, static_cast<size_t>(64 * 1024)> m_RAM;

		// No mapper support for now
		// std::shared_ptr<Mapper> mapper; // Cartridge mapper

	};
}

#endif