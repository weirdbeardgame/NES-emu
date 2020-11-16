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

	std::ifstream in("C:/Users/final/source/repos/NES/nestest.nes", std::ios::binary);

	for (uint8_t i = 0; i < sizeof(in); i++)
	{
		in >> ram[i];
	}
}
bool M6502::AND()
{
	return ram[activeRegs->PC] && activeRegs->A;
}

void M6502::ADC()
{
	uint8_t carry = (ram[activeRegs->PC] >> 7);

	if (carry & 1)
	{
		activeRegs->P |= 0 << 1;
	}

	activeRegs->A + ram[activeRegs->PC] + activeRegs->P >> 0;

	activeRegs->P |= 0 << activeRegs->A << 7; // Setting overflow in carry

} 

bool M6502::EOR()
{
	return activeRegs->A | ram[activeRegs->PC];
}

bool M6502::ORA()
{
	return activeRegs->A || ram[activeRegs->PC];
}

void M6502::LSR()
{
	activeRegs->A >> 1;
}

void M6502::ROL() // ROL copies the initial Carry flag to the lowmost bit of the byte; Either Ram or Accumulator. Rn, just Accumulator. 
{
	if (activeRegs->A == 0)
	{
		activeRegs->P |= 1 << 7;
	}
	else
	{
		activeRegs->A << (activeRegs->P >> 0 & 1);
	}
}

void M6502::RTI()
{
	// Return from Interrupt
}

void M6502::update()
{
	while (true)
	{
		uint8_t opcode = (ram[activeRegs->PC++]);
		execute(opcode);
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

void M6502::STA()
{
	ram[activeRegs->PC] = activeRegs->A;
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
	case 0xF8:
		break;
	}
}