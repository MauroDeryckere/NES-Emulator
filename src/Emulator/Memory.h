#ifndef NES_EMULATOR_MEMORY
#define NES_EMULATOR_MEMORY

#include <cstdint>
#include <array>

namespace NesEm
{
	class Memory final
	{
	public:
		Memory() = default;
		~Memory() = default;

		Memory(Memory const&) = delete;
		Memory(Memory&&) = delete;
		Memory& operator=(Memory const&) = delete;
		Memory& operator=(Memory&&) = delete;

		uint8_t Read(uint16_t address) const noexcept;
		void Write(uint16_t address, uint8_t value) noexcept;

	private:
		std::array<uint8_t, static_cast<size_t>(64 * 1024)> m_RAM;

		// No mapper support for now
		// std::shared_ptr<Mapper> mapper; // Cartridge mapper

	};
}

#endif