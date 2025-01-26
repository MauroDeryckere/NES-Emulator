#ifndef NES_EMULATOR_OPCODES_6502
#define NES_EMULATOR_OPCODES_6502

// Initialize the string implementation array for our instructions
#define NES_EM_INIT_STRING_REPRESENTATION 1;

namespace NesEm
{
#pragma region AddressingModes
	enum class AddressingMode : uint8_t
	{
		Accumulator, // Acts directly on accumulator
		Absolute, // Loads from absolute address
		AbsoluteX, // Absolute, X -> offset absolute address by X reg
		AbsoluteY, // Absolute, Y -> offset absolute address by Y reg
		Immediate, // Loads content itself instead of content from an address
		Implied,
		Indirect,  // 16 bit address is read to	get the actual 16 bit address (== pointer)
		IndirectX, // (Indirect, X) -> First uses ZeroPageX, so offset the zero page access by X, then takes the value found in memory on that location and uses this as an absolute access mode
		IndirectY, // (Indirect), Y -> Takes the "Indirect" zeropage value, then takes the value located there and uses it as an address, after that it adds Y to this absolute address, and then loads the value found at this address
		Relative, // Branching instructions	- branches to place relative on program counter based on current place
		ZeroPage, // First 256 bytes in memory for faster access
		ZeroPageX, // Works as ZeroPage but before getting contents, add value from X reg to the address (offset address by X reg)
		ZeroPageY, // Works as ZeroPage but before getting contents, add value from Y reg to the address (offset address by Y reg)
		Other // Other opcodes (only used for specific invalid opcodes)
	};
#pragma endregion

#pragma region OpCodesData
	//Struct to represent a single instruction with all necessary data
	struct Instruction final
	{
		uint8_t instructionId; // Id in the function table & string representation table
		AddressingMode addrMode; // What the address mode of the instruction is
		uint8_t cycles; // How many cycles the instruction takes
	};

	// because typing sucks
	using _i = Instruction;
	using _a = AddressingMode;

	constexpr auto AD_ACC{ _a::Accumulator };
	constexpr auto AD_ABS{ _a::Absolute };
	constexpr auto AD_ABX{ _a::AbsoluteX };
	constexpr auto AD_ABY{ _a::AbsoluteY };
	constexpr auto AD_IMM{ _a::Immediate };
	constexpr auto AD_IMP{ _a::Implied };
	constexpr auto AD_IND{ _a::Indirect };
	constexpr auto AD_INX{ _a::IndirectX };
	constexpr auto AD_INY{ _a::IndirectY };
	constexpr auto AD_REL{ _a::Relative };
	constexpr auto AD_ZPG{ _a::ZeroPage };
	constexpr auto AD_ZPX{ _a::ZeroPageX };
	constexpr auto AD_ZPY{ _a::ZeroPageY };
	constexpr auto AD_OTH{ _a::Other };

	// Updating any of these constants means we will have to update some tables too
	// But these should not have to be updated since the 6502 does not change anymore (unless we try a custom "mod", ...)
	constexpr uint8_t ID_ADC{ 0 };
	constexpr uint8_t ID_AND{ 1 };
	constexpr uint8_t ID_ASL{ 2 };
	constexpr uint8_t ID_BCC{ 3 };
	constexpr uint8_t ID_BCS{ 4 };
	constexpr uint8_t ID_BEQ{ 5 };
	constexpr uint8_t ID_BIT{ 6 };
	constexpr uint8_t ID_BMI{ 7 };
	constexpr uint8_t ID_BNE{ 8 };
	constexpr uint8_t ID_BPL{ 9 };
	constexpr uint8_t ID_BRK{ 10 };
	constexpr uint8_t ID_BVC{ 11 };
	constexpr uint8_t ID_BVS{ 12 };
	constexpr uint8_t ID_CLC{ 13 };
	constexpr uint8_t ID_CLD{ 14 };
	constexpr uint8_t ID_CLI{ 15 };
	constexpr uint8_t ID_CLV{ 16 };
	constexpr uint8_t ID_CMP{ 17 };
	constexpr uint8_t ID_CPX{ 18 };
	constexpr uint8_t ID_CPY{ 19 };
	constexpr uint8_t ID_DEC{ 20 };
	constexpr uint8_t ID_DEX{ 21 };
	constexpr uint8_t ID_DEY{ 22 };
	constexpr uint8_t ID_EOR{ 23 };
	constexpr uint8_t ID_INC{ 24 };
	constexpr uint8_t ID_INX{ 25 };
	constexpr uint8_t ID_INY{ 26 };
	constexpr uint8_t ID_JMP{ 27 };
	constexpr uint8_t ID_JSR{ 28 };
	constexpr uint8_t ID_LDA{ 29 };
	constexpr uint8_t ID_LDX{ 30 };
	constexpr uint8_t ID_LDY{ 31 };
	constexpr uint8_t ID_LSR{ 32 };
	constexpr uint8_t ID_NOP{ 33 };
	constexpr uint8_t ID_ORA{ 34 };
	constexpr uint8_t ID_PHA{ 35 };
	constexpr uint8_t ID_PHP{ 36 };
	constexpr uint8_t ID_PLA{ 37 };
	constexpr uint8_t ID_PLP{ 38 };
	constexpr uint8_t ID_ROL{ 39 };
	constexpr uint8_t ID_ROR{ 40 };
	constexpr uint8_t ID_RTI{ 41 };
	constexpr uint8_t ID_RTS{ 42 };
	constexpr uint8_t ID_SBC{ 43 };
	constexpr uint8_t ID_SEC{ 44 };
	constexpr uint8_t ID_SED{ 45 };
	constexpr uint8_t ID_SEI{ 46 };
	constexpr uint8_t ID_STA{ 47 };
	constexpr uint8_t ID_STX{ 48 };
	constexpr uint8_t ID_STY{ 49 };
	constexpr uint8_t ID_TAX{ 50 };
	constexpr uint8_t ID_TAY{ 51 };
	constexpr uint8_t ID_TSX{ 52 };
	constexpr uint8_t ID_TXA{ 53 };
	constexpr uint8_t ID_TXS{ 54 };
	constexpr uint8_t ID_TYA{ 55 };
	constexpr uint8_t ID_INV{ 56 };
	constexpr uint8_t ID_INVALID_OPCODE{ 56 };

	// 16x16 Array to represent all possible opcode bytes read from the ROM
	// Table with all opcodes for reference: 
	// https://www.masswerk.at/6502/6502_instruction_set.html
	static constexpr std::array<Instruction, 256> OPCODES_6502
	{
		_i{ ID_BRK, AD_IMP, 7 }, { ID_ORA, AD_INX, 6 }, { ID_INV, AD_OTH, 2 }, { ID_INV,AD_INX, 8 }, { ID_INV, AD_ZPG, 3 }, { ID_ORA, AD_ZPG, 3 }, { ID_ASL, AD_ZPG, 5 }, { ID_INV, AD_ZPG, 5 }, { ID_PHP, AD_IMP, 3 }, { ID_ORA, AD_IMM, 2 }, { ID_ASL, AD_ACC, 2 }, { ID_INV, AD_IMM, 2 }, { ID_INV, AD_ABS, 4 }, { ID_ORA, AD_ABS, 4 }, { ID_ASL, AD_ABS, 6 }, { ID_INV, AD_ABS, 6 },
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
		_i{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {},
	};

	//One lone coders table just as an extra comparison to make sure all our cycles are correct
	//{
	//	{ "BRK", & a::BRK, & a::IMM, 7 }, { "ORA", &a::ORA, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 3 }, { "ORA", &a::ORA, &a::ZP0, 3 }, { "ASL", &a::ASL, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PHP", &a::PHP, &a::IMP, 3 }, { "ORA", &a::ORA, &a::IMM, 2 }, { "ASL", &a::ASL, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ABS, 4 }, { "ASL", &a::ASL, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BPL", &a::BPL, &a::REL, 2 }, { "ORA", &a::ORA, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ZPX, 4 }, { "ASL", &a::ASL, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "CLC", &a::CLC, &a::IMP, 2 }, { "ORA", &a::ORA, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "ORA", &a::ORA, &a::ABX, 4 }, { "ASL", &a::ASL, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "JSR", &a::JSR, &a::ABS, 6 }, { "AND", &a::AND, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "BIT", &a::BIT, &a::ZP0, 3 }, { "AND", &a::AND, &a::ZP0, 3 }, { "ROL", &a::ROL, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PLP", &a::PLP, &a::IMP, 4 }, { "AND", &a::AND, &a::IMM, 2 }, { "ROL", &a::ROL, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "BIT", &a::BIT, &a::ABS, 4 }, { "AND", &a::AND, &a::ABS, 4 }, { "ROL", &a::ROL, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BMI", &a::BMI, &a::REL, 2 }, { "AND", &a::AND, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "AND", &a::AND, &a::ZPX, 4 }, { "ROL", &a::ROL, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "SEC", &a::SEC, &a::IMP, 2 }, { "AND", &a::AND, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "AND", &a::AND, &a::ABX, 4 }, { "ROL", &a::ROL, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "RTI", &a::RTI, &a::IMP, 6 }, { "EOR", &a::EOR, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 3 }, { "EOR", &a::EOR, &a::ZP0, 3 }, { "LSR", &a::LSR, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PHA", &a::PHA, &a::IMP, 3 }, { "EOR", &a::EOR, &a::IMM, 2 }, { "LSR", &a::LSR, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "JMP", &a::JMP, &a::ABS, 3 }, { "EOR", &a::EOR, &a::ABS, 4 }, { "LSR", &a::LSR, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BVC", &a::BVC, &a::REL, 2 }, { "EOR", &a::EOR, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "EOR", &a::EOR, &a::ZPX, 4 }, { "LSR", &a::LSR, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "CLI", &a::CLI, &a::IMP, 2 }, { "EOR", &a::EOR, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "EOR", &a::EOR, &a::ABX, 4 }, { "LSR", &a::LSR, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "RTS", &a::RTS, &a::IMP, 6 }, { "ADC", &a::ADC, &a::IZX, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 3 }, { "ADC", &a::ADC, &a::ZP0, 3 }, { "ROR", &a::ROR, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "PLA", &a::PLA, &a::IMP, 4 }, { "ADC", &a::ADC, &a::IMM, 2 }, { "ROR", &a::ROR, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "JMP", &a::JMP, &a::IND, 5 }, { "ADC", &a::ADC, &a::ABS, 4 }, { "ROR", &a::ROR, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BVS", &a::BVS, &a::REL, 2 }, { "ADC", &a::ADC, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "ADC", &a::ADC, &a::ZPX, 4 }, { "ROR", &a::ROR, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "SEI", &a::SEI, &a::IMP, 2 }, { "ADC", &a::ADC, &a::ABY, 4 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "ADC", &a::ADC, &a::ABX, 4 }, { "ROR", &a::ROR, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "???", &a::NOP, &a::IMP, 2 }, { "STA", &a::STA, &a::IZX, 6 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 6 }, { "STY", &a::STY, &a::ZP0, 3 }, { "STA", &a::STA, &a::ZP0, 3 }, { "STX", &a::STX, &a::ZP0, 3 }, { "???", &a::XXX, &a::IMP, 3 }, { "DEY", &a::DEY, &a::IMP, 2 }, { "???", &a::NOP, &a::IMP, 2 }, { "TXA", &a::TXA, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "STY", &a::STY, &a::ABS, 4 }, { "STA", &a::STA, &a::ABS, 4 }, { "STX", &a::STX, &a::ABS, 4 }, { "???", &a::XXX, &a::IMP, 4 },
	//	{ "BCC", &a::BCC, &a::REL, 2 }, { "STA", &a::STA, &a::IZY, 6 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 6 }, { "STY", &a::STY, &a::ZPX, 4 }, { "STA", &a::STA, &a::ZPX, 4 }, { "STX", &a::STX, &a::ZPY, 4 }, { "???", &a::XXX, &a::IMP, 4 }, { "TYA", &a::TYA, &a::IMP, 2 }, { "STA", &a::STA, &a::ABY, 5 }, { "TXS", &a::TXS, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 5 }, { "???", &a::NOP, &a::IMP, 5 }, { "STA", &a::STA, &a::ABX, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "???", &a::XXX, &a::IMP, 5 },
	//	{ "LDY", &a::LDY, &a::IMM, 2 }, { "LDA", &a::LDA, &a::IZX, 6 }, { "LDX", &a::LDX, &a::IMM, 2 }, { "???", &a::XXX, &a::IMP, 6 }, { "LDY", &a::LDY, &a::ZP0, 3 }, { "LDA", &a::LDA, &a::ZP0, 3 }, { "LDX", &a::LDX, &a::ZP0, 3 }, { "???", &a::XXX, &a::IMP, 3 }, { "TAY", &a::TAY, &a::IMP, 2 }, { "LDA", &a::LDA, &a::IMM, 2 }, { "TAX", &a::TAX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "LDY", &a::LDY, &a::ABS, 4 }, { "LDA", &a::LDA, &a::ABS, 4 }, { "LDX", &a::LDX, &a::ABS, 4 }, { "???", &a::XXX, &a::IMP, 4 },
	//	{ "BCS", &a::BCS, &a::REL, 2 }, { "LDA", &a::LDA, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 5 }, { "LDY", &a::LDY, &a::ZPX, 4 }, { "LDA", &a::LDA, &a::ZPX, 4 }, { "LDX", &a::LDX, &a::ZPY, 4 }, { "???", &a::XXX, &a::IMP, 4 }, { "CLV", &a::CLV, &a::IMP, 2 }, { "LDA", &a::LDA, &a::ABY, 4 }, { "TSX", &a::TSX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 4 }, { "LDY", &a::LDY, &a::ABX, 4 }, { "LDA", &a::LDA, &a::ABX, 4 }, { "LDX", &a::LDX, &a::ABY, 4 }, { "???", &a::XXX, &a::IMP, 4 },
	//	{ "CPY", &a::CPY, &a::IMM, 2 }, { "CMP", &a::CMP, &a::IZX, 6 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "CPY", &a::CPY, &a::ZP0, 3 }, { "CMP", &a::CMP, &a::ZP0, 3 }, { "DEC", &a::DEC, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "INY", &a::INY, &a::IMP, 2 }, { "CMP", &a::CMP, &a::IMM, 2 }, { "DEX", &a::DEX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 2 }, { "CPY", &a::CPY, &a::ABS, 4 }, { "CMP", &a::CMP, &a::ABS, 4 }, { "DEC", &a::DEC, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BNE", &a::BNE, &a::REL, 2 }, { "CMP", &a::CMP, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "CMP", &a::CMP, &a::ZPX, 4 }, { "DEC", &a::DEC, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "CLD", &a::CLD, &a::IMP, 2 }, { "CMP", &a::CMP, &a::ABY, 4 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "CMP", &a::CMP, &a::ABX, 4 }, { "DEC", &a::DEC, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//	{ "CPX", &a::CPX, &a::IMM, 2 }, { "SBC", &a::SBC, &a::IZX, 6 }, { "???", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "CPX", &a::CPX, &a::ZP0, 3 }, { "SBC", &a::SBC, &a::ZP0, 3 }, { "INC", &a::INC, &a::ZP0, 5 }, { "???", &a::XXX, &a::IMP, 5 }, { "INX", &a::INX, &a::IMP, 2 }, { "SBC", &a::SBC, &a::IMM, 2 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::SBC, &a::IMP, 2 }, { "CPX", &a::CPX, &a::ABS, 4 }, { "SBC", &a::SBC, &a::ABS, 4 }, { "INC", &a::INC, &a::ABS, 6 }, { "???", &a::XXX, &a::IMP, 6 },
	//	{ "BEQ", &a::BEQ, &a::REL, 2 }, { "SBC", &a::SBC, &a::IZY, 5 }, { "???", &a::XXX, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 8 }, { "???", &a::NOP, &a::IMP, 4 }, { "SBC", &a::SBC, &a::ZPX, 4 }, { "INC", &a::INC, &a::ZPX, 6 }, { "???", &a::XXX, &a::IMP, 6 }, { "SED", &a::SED, &a::IMP, 2 }, { "SBC", &a::SBC, &a::ABY, 4 }, { "NOP", &a::NOP, &a::IMP, 2 }, { "???", &a::XXX, &a::IMP, 7 }, { "???", &a::NOP, &a::IMP, 4 }, { "SBC", &a::SBC, &a::ABX, 4 }, { "INC", &a::INC, &a::ABX, 7 }, { "???", &a::XXX, &a::IMP, 7 },
	//};

	#if NES_EM_INIT_STRING_REPRESENTATION
	// String representation of all the opcodes, useful for debugging, disassembly,...
	// Put in alphabetical for easy of use, index in array also commented next to each item  
	// https://www.masswerk.at/6502/6502_instruction_set.html -> Instructions by Name
	static constexpr std::array<const char*, 57> OPCODES_6502_NAMES
	{
		"ADC", // 0
		"AND", // 1
		"ASL", // 2
		"BCC", // 3
		"BCS", // 4
		"BEQ", // 5
		"BIT", // 6
		"BMI", // 7
		"BNE", // 8
		"BPL", // 9
		"BRK", // 10
		"BVC", // 11
		"BVS", // 12
		"CLC", // 13
		"CLD", // 14
		"CLI", // 15
		"CLV", // 16
		"CMP", // 17
		"CPX", // 18
		"CPY", // 19
		"DEC", // 20
		"DEX", // 21
		"DEY", // 22
		"EOR", // 23
		"INC", // 24
		"INX", // 25
		"INY", // 26
		"JMP", // 27
		"JSR", // 28
		"LDA", // 29
		"LDX", // 30
		"LDY", // 31
		"LSR", // 32
		"NOP", // 33
		"ORA", // 34
		"PHA", // 35
		"PHP", // 36
		"PLA", // 37
		"PLP", // 38
		"ROL", // 39
		"ROR", // 40
		"RTI", // 41
		"RTS", // 42
		"SBC", // 43
		"SEC", // 44
		"SED", // 45
		"SEI", // 46
		"STA", // 47
		"STX", // 48
		"STY", // 49
		"TAX", // 50
		"TAY", // 51
		"TSX", // 52
		"TXA", // 53
		"TXS", // 54
		"TYA", // 55
		// Invalid Opcode
		"???"  // 56
	};
	#endif
#pragma endregion

#pragma region OpCodeFunctions
	//TODO - provide functions for all opcodes
	inline void ADC() noexcept
	{
		
	}
	// . . .
#pragma endregion
}

#endif