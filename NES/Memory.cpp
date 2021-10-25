#include "Memory.h"

void Memory::init()
{
	addressSpace = new uint16_t(0xFFFF);
	memset(addressSpace, 0, sizeof(addressSpace));
	SP = 0xFD;
}

// $0100-$01FF
// Note that stack works in reverse on MOS6502
void Memory::pushStack(uint8_t val)
{
	if ((0x1 + SP) > 0x0100)
	{
		addressSpace[SP] = val;
		SP -= 1;
	}
	else
	{
		throw("Stack Overflow");
		return;
	}
}

uint8_t Memory::pullStack()
{
	if ((0x1 + SP) < 0x1FF)
	{
		uint8_t val = std::move(addressSpace[(0x1 + SP)]);
		SP += 1;
		return val;
	}
	else
	{
		// Return empty stack
		throw("Stack Underflow");
		return -1;
	}
}


void Memory::WriteRam(uint8_t loc, uint8_t val)
{
	if (loc < 0x000 || loc > 0x07FF)
	{
		throw("Outside range of ram!");
	}
	else
	{
		addressSpace[loc] = val;
	}
}

void Memory::WritePpu(uint8_t loc, uint8_t val)
{
	if (loc < 0x2000 || loc < 0x2007)
	{
		throw("Outside range of PPU!");
	}
	else
	{
		addressSpace[loc] = val;
	}
}

void Memory::WriteApu(uint8_t loc, uint8_t val)
{
	if (loc < 0x4000 || loc > 0x4017)
	{
		throw("Outside range of APU!");
	}
	else
	{
		addressSpace[loc] = val;
	}
}

void Memory::WriteCartSpace(uint16_t loc, uint8_t val)
{
	if (loc < 0x4020 || loc > 0xFFFF)
	{
		throw("Outside range of Cart Space!");
	}
	else
	{
		addressSpace[loc] = val;
	}
}

uint8_t Memory::ReadRam(uint8_t loc)
{
	if (loc < 0x000 || loc > 0x07FF)
	{
		throw("Outside range of ram!");
		return -1;
	}
	else
	{
		return addressSpace[loc];
	}
}

uint8_t Memory::ReadPpu(uint8_t loc)
{
	if (loc < 0x2000 || loc < 0x2007)
	{
		throw("Outside range of PPU!");
		return -1;
	}
	else
	{
		return addressSpace[loc];
	}
}

uint8_t Memory::ReadApu(uint8_t loc)
{
	if (loc < 0x4000 || loc > 0x4017)
	{
		throw("Outside range of APU!");
		return -1;
	}
	else
	{
		return addressSpace[loc];
	}
}

uint8_t Memory::ReadCartSpace(uint16_t loc)
{
	if (loc < 0x4020 || loc > 0xFFFF)
	{
		throw("Outside range of Cart Space!");	
		return -1;
	}
	else
	{
		return addressSpace[loc];
	}
}