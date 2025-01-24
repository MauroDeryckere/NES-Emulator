#ifndef NES_EMULATOR_CPU
#define NES_EMULATOR_CPU

#include <cstdint>
#include "Memory.h"

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


		CPU(CPU const&) = delete;
		CPU(CPU&&) = delete;
		CPU& operator=(CPU const&) = delete;
		CPU& operator=(CPU&&) = delete;
	private:
		Memory m_Memory{  };

		uint8_t m_Accumulator{ 0 };
		uint8_t m_XRegister{ 0 };
		uint8_t m_YRegister{ 0 };

		uint16_t m_ProgramCounter{ 0 };
		uint8_t m_StackPointer{ 0 };
		uint8_t m_StatusRegister{ 0 };

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

		//Address modes
		enum class AddressingMode : uint8_t
		{
			Accumulator, // Acts directly on accumulator
			Relative, // Branching instructions	- branches to place relative on program counter based on current place
			Immediate, // Loads content itself instead of content from an address
			ZeroPage, // First 256 bytes in memory for faster access
			ZeroPageX, // Works as ZeroPage but before getting contents, add value from X reg to the address (offset address by X reg)
			ZeroPageY, // Works as ZeroPage but before getting contents, add value from Y reg to the address (offset address by Y reg)
			Absolute, // Loads from absolute address
			AbsoluteX, // Absolute, X -> offset absolute address by X reg
			AbsoluteY, // Absolute, Y -> offset absolute address by Y reg
			IndirectX, // (Indirect, X) -> First uses ZeroPageX, so offset the zero page access by X, then takes the value found in memory on that location and uses this as an absolute access mode
			IndirectY, // (Indirect), Y -> Takes the "Indirect" zeropage value, then takes the value located there and uses it as an address, after that it adds Y to this absolute address, and then loads the value found at this address
			NoneAddressing // Instructions that don't require any special opcodes like BRK, ...
		};

		[[nodiscard]] inline uint16_t GetAddress() noexcept;

		//Opcodes

		//Opcode handler table
		//TODO

		// In case an illegal opcode is executed
		inline void IllegalOpcode() noexcept {  };

		[[nodiscard]] inline uint8_t Fetch() noexcept;
		void Push(uint8_t value) noexcept;
		[[nodiscard]] inline uint8_t Pop() noexcept;

		void Clock() noexcept;

		// Runs "Async" and can interupt the CPU at any point in time (will finish the current instruction 1st)
		void Reset() noexcept;
		void IRQ() noexcept;
		void NMI() noexcept;


	};
}

#endif