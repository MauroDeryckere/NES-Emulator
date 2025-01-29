#ifndef NES_EMULATOR_CPU
#define NES_EMULATOR_CPU

#include <cstdint>

#include "NESMemory.h"
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
		CPU() = default;
		~CPU() = default;

		void Clock() noexcept;

		CPU(CPU const&) = delete;
		CPU(CPU&&) = delete;
		CPU& operator=(CPU const&) = delete;
		CPU& operator=(CPU&&) = delete;

	private:
		// Opcode handler should be friended since we do need access to some private variables
		// Like editing registers, ...
		friend class OpcodeHandler;

		OpcodeHandler m_OpcodeHandler{ };
		NESMemory m_Memory{ };

		uint8_t m_Accumulator{ 0 };
		uint8_t m_XRegister{ 0 };
		uint8_t m_YRegister{ 0 };

		mutable uint16_t m_ProgramCounter{ 0 };
		uint8_t m_StackPointer{ 0 };
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
		FORCE_INLINE void SetFlag(StatusFlags flag, bool value) noexcept
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

#pragma region Memory
		// Read memory at program counter and increase the program counter
		[[nodiscard]] FORCE_INLINE uint8_t Read() const noexcept
		{
			return m_Memory.Read(m_ProgramCounter++);
		}
		// Read memory at a specific address
		[[nodiscard]] FORCE_INLINE uint8_t Read(uint16_t address) const noexcept
		{
			return m_Memory.Read(address);
		}

		// Write a value to a specific address
		FORCE_INLINE void Write(uint16_t address, uint8_t value) noexcept
		{
			m_Memory.Write(address, value);
		}

#pragma region Stack
		//TODO
		void Push(uint8_t value) noexcept
		{
			
		}
		[[nodiscard]] inline uint8_t Pop() noexcept
		{
			
		}
#pragma endregion

#pragma endregion

		// Runs "Async" and can interupt the CPU at any point in time (will finish the current instruction 1st)
		void Reset() noexcept;
		void IRQ() noexcept;
		void NMI() noexcept;
	};
}

#endif