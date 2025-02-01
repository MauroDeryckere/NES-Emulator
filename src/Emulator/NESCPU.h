#ifndef NES_EMULATOR_CPU
#define NES_EMULATOR_CPU

#include <cstdint>

#include "NESMemory.h"
#include "NESCartridge.h"
#include "NESPPU.h"
#include "OpcodeHandler.h"

/* Various sources used during development of the CPU of our emulator: 
 * https://medium.com/@guilospanck/the-journey-of-writing-a-nes-emulator-part-i-the-cpu-6e83b50baa37
 * https://www.youtube.com/watch?v=8XmxKPJDGU0
 * https://www.nesdev.org/obelisk-6502-guide/
 * https://skilldrick.github.io/easy6502/
 */

namespace NesEm
{
	class CPU final
	{
	public:
		CPU(PPU& ppu);
		~CPU() = default;

		void Clock() noexcept;

		// Runs "Async" and can interupt the CPU at any point in time (will finish the current instruction 1st)
		void Reset() noexcept
		{
			// When te CPU is "reset" it is set to a known state with the following values
			m_StackPointer -= 3;

			SetFlag(StatusFlags::U); // Unusued bit should always be set
			SetFlag(StatusFlags::I); // Interupt disable is set to on when Reset

			// Data at location 0x0FFC can be set by programmer, this is the "entry point" for the program
			// Program counter should be set to this data whenever we Reset
			// LL | HH
			m_ProgramCounter = static_cast<uint16_t>(Read(RESET_VECTOR) | (Read(RESET_VECTOR + 1) << 8));

			// Reset takes 8 cycles
			m_CurrCycles = 8;
		}

		CPU(CPU const&) = delete;
		CPU(CPU&&) = delete;
		CPU& operator=(CPU const&) = delete;
		CPU& operator=(CPU&&) = delete;

	private:
#pragma region constants

		static constexpr uint16_t STACK_PTR_INIT{ 0xFD };

		static constexpr uint16_t NON_MASK_INTERRUPT_VECTOR{ 0xFFFA };
		static constexpr uint16_t RESET_VECTOR{ 0x0FFC };
		static constexpr uint16_t INTERRUPT_VECTOR{ 0xFFFE };

		static constexpr uint16_t ADDRESSABLE_RAM_RANGE_END{ 0x1FFF };

		static constexpr uint16_t ADDRESSABLE_PPU_RANGE_START{ 0x2000 };
		static constexpr uint16_t ADDRESSABLE_PPU_RANGE_END{ 0x3FFF };

#pragma endregion

		PPU& m_PPU;

		// Opcode handler should be friended since we do need access to some private variables
		// Like editing registers, ...
		friend class OpcodeHandler;

		OpcodeHandler m_OpcodeHandler{ };
		//2KB RAM
		NESMemory<2048> m_Memory{ };

		uint8_t m_Accumulator{ 0 };
		uint8_t m_XRegister{ 0 };
		uint8_t m_YRegister{ 0 };

		mutable uint16_t m_ProgramCounter{ };

		// Whenever we reset the stack pointer should be set to 0xFD
		uint8_t m_StackPointer{ };
		uint8_t m_StatusRegister{ 0 };

		//Counter of cycles to be executed before next instruction may be executed
		uint8_t m_CurrCycles{ 0 };

		enum class StatusFlags : uint8_t
		{
			C = (1 << 0), // Carry
			Z = (1 << 1), // Zero
			I = (1 << 2), // Interupt
			D = (1 << 3), // Decimal
			B = (1 << 4), // Break
			U = (1 << 5), // Unused
			V = (1 << 6), // Overflow
			N = (1 << 7)  // Negative
		};

		// Convenience functions to access status register

		// Param StatusFlags status flag we check
		// Returns wether or not a specific status flag is set
		[[nodiscard]] FORCE_INLINE bool IsFlagSet(StatusFlags flag) const noexcept
		{
			static_assert(std::is_unsigned_v<std::underlying_type_t<StatusFlags>>);
			static_assert(std::is_enum_v<StatusFlags>);
			static_assert(std::is_same_v<std::underlying_type_t<StatusFlags>, decltype(m_StatusRegister)>);

			return (static_cast<std::underlying_type_t<StatusFlags>>(flag) & m_StatusRegister) != 0;
		}

		// Param StatusFlags status flag to set
		// Param bool value, when true set the bit when false clear the bit
		// Sets or clears (depending on the provided value) a specific status flag
		FORCE_INLINE void SetOrClearFlag(StatusFlags flag, bool value) noexcept
		{
			static_assert(std::is_unsigned_v<std::underlying_type_t<StatusFlags>>);
			static_assert(std::is_enum_v<StatusFlags>);
			static_assert(std::is_same_v<std::underlying_type_t<StatusFlags>, decltype(m_StatusRegister)>);

			if (value)
			{
				// 0 | 0 -> 0
				// 0 | 1 -> 1
				// 1 | 0 -> 1
				// 1 | 1 -> 1
				// E.g 0000 0000 |= 1000 0000 -> 1000 0000
				m_StatusRegister |= static_cast<std::underlying_type_t<StatusFlags>>(flag);  // Set bit
			}
			else
			{
				// 0 & 0 -> 0 
				// 0 & 1 -> 0 
				// 1 & 0 -> 0 
				// 1 & 1 -> 1 
				// E.g 0000 0000 &= 1000 0000 -> 0000 0000	-->  0000 0000 &= (~1000 0000) -> 0000 0000 &= 0111 1111 -> 0000 0000
				// E.g 1000 0000 &= 1000 0000 -> 1000 0000	-->  1000 0000 &= (~1000 0000) -> 1000 0000 &= 0111 1111 -> 0000 0000
				m_StatusRegister &= ~static_cast<std::underlying_type_t<StatusFlags>>(flag); // Clear bit
			}
		}

		// Param StatusFlags status flag to set
		// Set a specific status flag
		FORCE_INLINE void SetFlag(StatusFlags flag) noexcept
		{
			static_assert(std::is_unsigned_v<std::underlying_type_t<StatusFlags>>);
			static_assert(std::is_enum_v<StatusFlags>);
			static_assert(std::is_same_v<std::underlying_type_t<StatusFlags>, decltype(m_StatusRegister)>);

			// 0 | 0 -> 0
			// 0 | 1 -> 1
			// 1 | 0 -> 1
			// 1 | 1 -> 1
			// E.g 0000 0000 |= 1000 0000 -> 1000 0000
			m_StatusRegister |= static_cast<std::underlying_type_t<StatusFlags>>(flag);  // Set bit
		}

		// Param StatusFlags status flag to set
		// Clear a specific status flag
		FORCE_INLINE void ClearFlag(StatusFlags flag) noexcept
		{
			static_assert(std::is_unsigned_v<std::underlying_type_t<StatusFlags>>);
			static_assert(std::is_enum_v<StatusFlags>);
			static_assert(std::is_same_v<std::underlying_type_t<StatusFlags>, decltype(m_StatusRegister)>);

			// 0 & 0 -> 0 
			// 0 & 1 -> 0 
			// 1 & 0 -> 0 
			// 1 & 1 -> 1 
			// E.g 0000 0000 &= 1000 0000 -> 0000 0000	-->  0000 0000 &= (~1000 0000) -> 0000 0000 &= 0111 1111 -> 0000 0000
			// E.g 1000 0000 &= 1000 0000 -> 1000 0000	-->  1000 0000 &= (~1000 0000) -> 1000 0000 &= 0111 1111 -> 0000 0000
			m_StatusRegister &= ~static_cast<std::underlying_type_t<StatusFlags>>(flag); // Clear bit
		}

#pragma region Memory
		// Read memory at program counter and increase the program counter
		[[nodiscard]] FORCE_INLINE uint8_t Read() const noexcept
		{
			return m_Memory.Read(m_ProgramCounter++);
		}
		// Read memory at a specific address
		[[nodiscard]] FORCE_INLINE uint8_t Read(uint16_t address) const noexcept
		{
			if (address <= ADDRESSABLE_RAM_RANGE_END)
			{
				//Handle mirroring since there is an 8KB addressable range for our 2KB RAM
				return m_Memory.Read(address & 0x07FF);
			}

			if (address >= ADDRESSABLE_PPU_RANGE_START && address <= ADDRESSABLE_PPU_RANGE_END)
			{
				//Handle mirroring
				//return m_PPU.Read(address & 8);
				return 0;
			}

			//return m_Cartridge.Read(address);

			//assert(false);
			return 0;
		}

		// Write a value to a specific address
		FORCE_INLINE void Write(uint16_t address, uint8_t value) noexcept
		{
			if (address <= ADDRESSABLE_RAM_RANGE_END)
			{
				//Handle mirroring since there is an 8KB addressable range for our 2KB RAM
				m_Memory.Write(address & 0x07FF, value);
				return;
			}

			if (address >= ADDRESSABLE_PPU_RANGE_START && address <= ADDRESSABLE_PPU_RANGE_END)
			{
				//Handle mirroring
				//m_PPU.Write(address & 8, value);
				return;
			}

			// m_Cartridge.Write(address, value);

			assert(false);
		}

#pragma region Stack
		FORCE_INLINE void Push(uint8_t value) noexcept
		{
			Write(0x0100 + m_StackPointer--, value);
		}

		[[nodiscard]] FORCE_INLINE uint8_t Pop() noexcept
		{
			return Read(0x0100 + m_StackPointer++);
		}
#pragma endregion
#pragma endregion

		// Runs "Async" and can interupt the CPU at any point in time (will finish the current instruction 1st)
		void IRQ() noexcept
		{
			// https://www.nesdev.org/wiki/CPU_interrupts
			if (not IsFlagSet(StatusFlags::I))
				return;

			Push((m_ProgramCounter << 8) & 0x00FF);
			Push(m_ProgramCounter & 0x00FF);

			ClearFlag(StatusFlags::B);
			SetFlag(StatusFlags::U);
			SetFlag(StatusFlags::U);

			Push(m_StatusRegister);

			// LL | HH
			m_ProgramCounter = static_cast<uint16_t>(Read(INTERRUPT_VECTOR) | (Read(INTERRUPT_VECTOR + 1) << 8));

			m_CurrCycles = 7;
		}

		void NMI() noexcept
		{
			Push((m_ProgramCounter << 8) & 0x00FF);
			Push(m_ProgramCounter & 0x00FF);

			ClearFlag(StatusFlags::B);
			SetFlag(StatusFlags::U);
			SetFlag(StatusFlags::U);

			Push(m_StatusRegister);

			// LL | HH
			m_ProgramCounter = static_cast<uint16_t>(Read(NON_MASK_INTERRUPT_VECTOR) | (Read(NON_MASK_INTERRUPT_VECTOR + 1) << 8));

			m_CurrCycles = 8;
		}
	};
}

#endif