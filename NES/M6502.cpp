#include "M6502.h"
#include <bitset>

void M6502::Reset()
{

	activeRegs = new registers();

	activeRegs->P = 0x34;
	activeRegs->A = 0x00;
	activeRegs->X = 0x00;
	activeRegs->Y = 0x00;
	activeRegs->SP = 0xFD;

	cpuAddressSpace.init();

	std::ifstream in("C:/Users/final/source/repos/NES-emu/NES/nestest.nes", std::ios::binary);

	// Do note that the size of in here is intended to be the actual file buffer length
	// Though I have a sneaking suspicion that this will end up just being the size of pointer value or some retarded thing like that
	uint8_t* temp = new uint8_t(sizeof(in));

	for (uint8_t i = 0; i < sizeof(in); i++)
	{
		in >> temp[i];
	}
	// cpuspace[0x8000] = temp[0x10]; // Reading into the beginning of mappable cpu memory. Skip past 16 byte header
}
void M6502::AND()
{
	cpuAddressSpace.WriteRam (activeRegs->PC, activeRegs->A);
	//ram[activeRegs->PC] &= activeRegs->A;
	if (activeRegs->A == 0)
	{
		activeRegs->P | (1 << 2); // 1 is what's being written. 2 is postion. | is to set. Ie, if num is 0 OR 1 return 1. If num is 0 | 0 return 1
	}

	if (activeRegs->A & (1 >> 7)) // If Overflow. Carry muthafucker!
	{
		activeRegs->P |= 1 << 7;
	}


}

void M6502::ADC()
{
	//activeRegs->A = activeRegs->A + ram[activeRegs->PC] + (activeRegs->P & 1);

	if (activeRegs->A & (1 >> 7)) // If Overflow. Carry muthafucker!
	{
		activeRegs->P |= 1;
	}

	if (activeRegs->A == 0)
	{
		activeRegs->P | (1 << 2); // 1 is what's being written. 2 is postion. | is to set. Ie, if num is 0 OR 1 return 1. If num is 0 | 0 return 1
	}

	// Set overflow if sign bit is incorrect?

} 

void M6502::EOR()
{
	//activeRegs->A ^ ram[activeRegs->PC];
}

void M6502::INC()
{
	//ram[activeRegs->PC] += 1;
}

void M6502::INX()
{
	activeRegs->X += 1;
}

void M6502::INY()
{
	activeRegs->Y += 1;
}

void M6502::ORA()
{
	 //activeRegs->A | ram[activeRegs->PC];
}

void M6502::LSR()
{
	activeRegs->A >> 1;
}

void M6502::ROL() // ROL copies the initial Carry flag to the lowmost bit of the byte; Either Ram or Accumulator. Rn, just Accumulator. 
{
	if (activeRegs->A == 0)
	{
		activeRegs->P | 1 << 7;
	}
	else
	{
		activeRegs->A << (activeRegs->P << 0 & 1);
	}
}

void M6502::RTI()
{
	// Return from Interrupt
	// Need to factor Stack and add handling in here
	// the stack always uses some part of the $0100-$01FF page
}


void M6502::update()
{
	while (true)
	{
		//uint8_t opcode = (ram[activeRegs->PC++]);
		//execute(opcode);
	}
}

void M6502::SED()
{
	activeRegs->P |= 1 << 2;
}

void M6502::SEI()
{
	activeRegs->P |= 1 << 1;
}

// An assembler will automatically select zero page addressing mode
// if the operand evaluates to a zero page address and the instruction supports the mode (not all do).
void M6502::STA()
{
	//ram[activeRegs->PC] = activeRegs->A;
}

void M6502::TAY()
{
	activeRegs->Y = activeRegs->A;
}

void M6502::execute(uint8_t OP) // What Addressing Mode are we in?
{

	std::cout << "PC: 0x" << std::hex << unsigned(OP) << std::endl;
	std::cout << "A: " << unsigned(activeRegs->A) << std::endl;
	std::cout << "X: " << unsigned(activeRegs->X) << std::endl;
	std::cout << "Y: " << unsigned(activeRegs->Y) << std::endl;
	std::cout << "Status: " << std::bitset<8>(activeRegs->P) << std::endl;

	switch (OP)
	{
	case 0x01:
		// ORA! Logical Inclusive OR
		ORA();
		break;
	case 0x1A:
		// NOP
		return;
		break;
	case 0x2A:
		ROL();
		break;
	case 0x40:
		// Return from interrupt
		RTI();
		break;
	case 0x45:
		// EOR - Zero Page. I'm assuming clears the register shit out?
		break;
	case 0x4e:
		// Logical Shift Right
		LSR();
		break;
	case 0x69:
		ADC();
		break;
	case 0x78:
		SEI();
		break;
	case 0x83:
		// Rotate one bit to Left
		break;
	case 0x81:
		// STA Indirect,X
		STA();
		break;
	case 0x85:
		// STA Zero Page
		STA();
		break;
	case 0x8D:
		// STA Absolute
		STA();
		break;
	case 0x91:
		// STA Indirect,Y
		STA();
		break;
	case 0x95:
		// STA Zero Page,X
		STA();
		break;
	case 0x99:
		//Absolute,Y
		STA();
		break;
	case 0x9D:
		// STA Absolute,X
		STA();
		break;
	case 0xA8:
		TAY();
		break;
	case 0xC8:
		INY();
		break;
	case 0xE6:
		INC();
		break;
	case 0xE8:
		INX();
		break;
	case 0xF6:
		INC();
		break;
	case 0xF8:
		break;
	}
}