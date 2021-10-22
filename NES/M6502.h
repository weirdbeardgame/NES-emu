#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "Memory.h"

struct registers
{
	uint8_t A;
	uint8_t X, Y; //used for holding things such as iteration counts or as offsets to addressing modes. The X register in addition can be used to set or get the SP.
	uint8_t P; // Flags. C, Z, V, S, I
	
	// Both of these are supposed to point to memory addresses
	uint16_t SP; // 0x100 to 0x1FF
	uint16_t PC;

};

enum FileStandard{INES, NES2};

enum Flags 
{
	C = 0,
	Z = 1,
	I = 2,
	D = 3,
	B = 4,
	V = 6,
	N = 7
};

struct cartData
{
	FileStandard romStandard;
	int prgSize;
	int chrSize;

	/***************************************************************************************************
	* 76543210
	* ||||||||
	* |||||||+- Mirroring: 0: horizontal (vertical arrangement) (CIRAM A10 = PPU A11)
	* |||||||			1: vertical (horizontal arrangement) (CIRAM A10 = PPU A10)
	* ||||||+-- 1: Cartridge contains battery-backed PRG RAM ($6000-7FFF) or other persistent memory
	* |||||+--- 1: 512-byte trainer at $7000-$71FF (stored before PRG data)
	* ||||+---- 1: Ignore mirroring control or above mirroring bit; instead provide four-screen VRAM
	* ++++----- Lower nybble of mapper number
	***************************************************************************************************/
	uint8_t mapperMirroring;

	// Bit 7 is the actual mapper data
	uint8_t mapper;
};


class M6502
{
private:
	registers* activeRegs;
	cartData cartridge;
	// The CPU has a 16-bit address space (up to 64kb can be refrenced) but only a 2 kb size of ram. Address space is not ram!
	Memory cpuAddressSpace;
	int remainingCycleCount;
	int i = 0;
public:
	void init();
	void Reset();
	void ADC();
	void AND();
	void ASL(uint8_t& shift);
	void EOR();
	void INC();
	void INX();
	void INY();
	void LSR();
	void ORA();
	void RTI();
	void ROL();
	void SED();
	void SEI();
	void STA();
	void TAY();
	void execute(uint8_t OP);
	void update();
};

