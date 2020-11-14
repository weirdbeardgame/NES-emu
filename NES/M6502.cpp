#include "M6502.h"


void M6502::Reset()
{

	activeRegs = new registers();

    activeRegs->P = 0x34;
	activeRegs->A = 0x00;
	activeRegs->X = 0x00;
	activeRegs->Y = 0x00;
	activeRegs->SP = 0xFD;

	std::ifstream in("C:/Users/final/source/repos/NES/DK.nes", std::ios::binary);

	ram.insert(ram.begin(),
		std::istream_iterator<uint8_t>(in),
		std::istream_iterator<uint8_t>());


}
bool M6502::AND()
{
	return ram[activeRegs->PC] && activeRegs->A;
}

void M6502::ADC()
{

}

bool M6502::ORA()
{
	return activeRegs->A || ram[activeRegs->PC];
}

void M6502::LSR()
{
	activeRegs->A >> 1;
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

void M6502::SEI()
{
	activeRegs->P |= 1 << 2;
}

void M6502::TAY()
{
	activeRegs->Y = activeRegs->A;
}

void M6502::execute(uint8_t OP) // What Addressing Mode are we in?
{

	std::cout << "OP: 0x" << std::hex << unsigned(OP) << std::endl;
	std::cout << "A: " << unsigned(activeRegs->A) << std::endl;
	std::cout << "X: " << unsigned(activeRegs->X) << std::endl;
	std::cout << "Y: " << unsigned(activeRegs->Y) << std::endl;


	switch (OP)
	{
	case 0x01:
		// ORA! Logical Inclusive OR
		ORA();
		break;
	case 0x40:
		// Return from interrupt
		RTI();
		break;
	case 0x45:
		// EOR - Zero Page. I'm assuming clears the register shit out?
		break;
	case 0x4E:
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
	case 0xA8:
		TAY();
		break;
	}
}