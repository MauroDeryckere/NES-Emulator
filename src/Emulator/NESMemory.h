#ifndef NES_EMULATOR_MEMORY
#define NES_EMULATOR_MEMORY

#include "emulator_pch.h"

#include <array>

namespace NesEm
{
	template <std::size_t RAM_SIZE>
	class NESMemory final
	{
	public:
		NESMemory() = default;
		~NESMemory() = default;

		NESMemory(NESMemory const&) = delete;
		NESMemory(NESMemory&&) = delete;
		NESMemory& operator=(NESMemory const&) = delete;
		NESMemory& operator=(NESMemory&&) = delete;

		[[nodiscard]] inline uint8_t Read(uint16_t address) const noexcept
		{
			return m_RAM[address];
		}

		inline void Write(uint16_t address, uint8_t value) noexcept
		{
			assert(address < m_RAM.size());

			m_RAM[address] = value;
		}

	private:
		std::array<uint8_t, RAM_SIZE> m_RAM{  };
	};
}

#endif