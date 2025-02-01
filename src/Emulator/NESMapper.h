#ifndef NES_EMULATOR_MAPPER
#define NES_EMULATOR_MAPPER

#include <cstdint>

namespace NesEm
{
	class Mapper
	{
	public:
		Mapper(uint8_t chrBanks, uint8_t prgBanks)
		{

		}
		virtual ~Mapper() = default; 

		Mapper(Mapper const&) = delete;
		Mapper(Mapper&&) = delete;
		Mapper& operator=(Mapper const&) = delete;
		Mapper& operator=(Mapper&&) = delete;
		
	
	private:
	};
}

#endif