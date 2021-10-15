#pragma once
#include <iostream>
#include <iomanip>

/***********************************************************************
* $0000 - $07FF: RAM
* $0800 - $1FFF: Mirror of RAM
* $2000 - $2007: PPU Registers
* $2008 - $3FFF: Mirror of PPU
* $4000 - $4017: NES APU and I/O Registers
* $4020 - $FFFF: Cartridge space. PRG ROM, PRG RAM, Mapper Resgisters
***********************************************************************/
class Memory
{
private:
	uint8_t* addressSpace;
public:
	uint8_t ReadRam(uint8_t loc);
	uint8_t ReadPpu(uint8_t loc);
	uint8_t ReadApu(uint8_t loc);
	uint8_t ReadCartSpace(uint8_t loc);

	void init();
	void WriteRam(uint8_t loc, uint8_t val);
	void WritePpu(uint8_t loc, uint8_t val);
	void WriteApu(uint8_t loc, uint8_t val);
	void WriteCartSpace(uint8_t loc, uint8_t val);
};