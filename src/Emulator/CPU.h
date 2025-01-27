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
		OpcodeHandler m_OpcodeHandler{  };
		NESMemory m_Memory{  };

		uint8_t m_Accumulator{ 0 };
		uint8_t m_XRegister{ 0 };
		uint8_t m_YRegister{ 0 };

		uint16_t m_ProgramCounter{ 0 };
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
		// Functions to adjust status flags TODO

		// Memory
		[[nodiscard]] inline uint8_t Fetch() noexcept;

		// Stack
		void Push(uint8_t value) noexcept;
		[[nodiscard]] inline uint8_t Pop() noexcept;

		// Runs "Async" and can interupt the CPU at any point in time (will finish the current instruction 1st)
		void Reset() noexcept;
		void IRQ() noexcept;
		void NMI() noexcept;
	};
}

#endif