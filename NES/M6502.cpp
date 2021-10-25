#include "M6502.h"
#include <bitset>

// In the future this logic needs to be broken into the other loadRom function...
void M6502::Reset()
{
	activeRegs = new registers();

	activeRegs->P = 0x34;
	activeRegs->A = 0x00;
	activeRegs->X = 0x00;
	activeRegs->Y = 0x00;

	cpuAddressSpace.init();

	std::ifstream in("C:/Users/final/source/repos/NES-emu/NES/nestest.nes", std::ios::binary);

	// Do note that the size of in here is intended to be the actual file buffer length
	// Though I have a sneaking suspicion that this will end up just being the size of pointer value or some retarded thing like that
	uint8_t* temp = new uint8_t(sizeof(in));
	uint8_t* header = new uint8_t(0x10);
	for (uint8_t i = 0; i < sizeof(in); i++)
	{
		in >> temp[i];
	}
	memcpy(header, temp, 0x10);
	std::string title = { &header[0], &header[3] };
	if (title.find("NES<EOF>") != std::string::npos)
	{
		throw("Not a valid nes file!");
		return;
	}
	cartridge.romStandard = FileStandard::INES;
	// This is a NES 2.0 standard file
	if ((header[7] & 0x0C) == 0x08)
	{
		cartridge.romStandard = FileStandard::NES2;
	}

	switch (cartridge.romStandard)
	{
	case FileStandard::INES:
		printf("INES \n");
		cartridge.prgSize = header[4];
		cartridge.chrSize = header[5];
		cartridge.mapperMirroring = header[6];
		cartridge.mapper = header[7];
		break;

	case FileStandard::NES2:
		printf("NES 2.0 \n");
		// Mapper number is 12 bit
		break;
	}
	for (int i = 0; i < cartridge.prgSize; i++)
	{
		for (int j = 0x10; j < sizeof(temp); j++)
		{
			// Prg rom. 16kb banks. The rom isn't written to 0x8000 it's mapped to it big difference!
			//cpuAddressSpace.WriteCartSpace(i, temp[j]);
		}
	}
	//activeRegs->PC = cpuAddressSpace.ReadCartSpace(0x8000);
}
void M6502::AND()
{
	activeRegs->A &= cpuAddressSpace.ReadCartSpace(activeRegs->PC);
	if (activeRegs->A == 0)
	{
		activeRegs->P | (1 << Flags::Z);
	}

	if (activeRegs->A & (1 << 7)) // If Overflow. Carry muthafucker!
	{
		activeRegs->P |= 1 << Flags::N;
	}
}

void M6502::ADC()
{
	// A + M + C
	activeRegs->A += cpuAddressSpace.ReadCartSpace(activeRegs->PC) + (activeRegs->P & (1 << Flags::C));

	// Check if bit 7 is active
	if (activeRegs->A & (1 << 7))
	{
		activeRegs->P |= 1 << Flags::N;
	}

	if (activeRegs->A == 0)
	{
		activeRegs->P | (1 << Flags::Z);
	}

	// Check for overflow in bit 7?

} 

void M6502::ASL(uint8_t& shift)
{
	shift = shift << 1;
	activeRegs->P = 0;
}

// To increment the PC by an offset
void M6502::BCC()
{
	// In relative add the value to the PC following the OP Code
	if (~(activeRegs->P & Flags::C)) // This is wrong. I probably need to mask the bit out. 
	{
		activeRegs->PC += 1;
		uint8_t immediate = cpuAddressSpace.ReadCartSpace(activeRegs->PC);
		activeRegs->PC += immediate;
	}

}

void M6502::EOR()
{
	activeRegs->A ^ cpuAddressSpace.ReadCartSpace(activeRegs->PC);
	if (activeRegs->A == 0)
	{
		// Set Zero Flag
		activeRegs->P |= 1 << Flags::Z;
	}
	// Mask off all but bit 7. If bit 7 active set negative flag
	if (activeRegs->A & 0x80 >> 1)
	{
		activeRegs->P |= 1 << Flags::N;
	}
}

void M6502::INC()
{
	//ram[activeRegs->PC] += 1;
	cpuAddressSpace.WriteRam(activeRegs->PC, (cpuAddressSpace.ReadRam(activeRegs->PC) + 1));
}

void M6502::INX()
{
	activeRegs->X += 1;
}

void M6502::INY()
{
	activeRegs->Y += 1;
}

// Star Platinum noises
void M6502::ORA()
{
	 activeRegs->A | cpuAddressSpace.ReadCartSpace(activeRegs->PC);
	 if (activeRegs->A == 0)
	 {
		 // Set Zero Flag
		 activeRegs->P |= 1 << Flags::Z;
	 }
}

void M6502::LSR()
{
	activeRegs->A >> 1;
}

void M6502::ROL() 
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
	activeRegs->P = cpuAddressSpace.pullStack();
	activeRegs->PC = cpuAddressSpace.pullStack();
}


void M6502::update()
{
	while (true)
	{
		uint8_t opcode = (cpuAddressSpace.ReadCartSpace(activeRegs->PC) & 0b11110000 >> 4);
		execute(opcode); 
		activeRegs->PC += 1; // Though note the program is going to provide the offset!
	}
}

void M6502::SED()
{
	activeRegs->P |= 1 << Flags::D;
}

void M6502::SEI()
{
	activeRegs->P |= 1 << Flags::I;
}

// An assembler will automatically select zero page addressing mode
// if the operand evaluates to a zero page address and the instruction supports the mode (not all do).
void M6502::STA()
{
	cpuAddressSpace.WriteRam(activeRegs->PC, activeRegs->A);
}

void M6502::TAY()
{
	activeRegs->Y = std::move(activeRegs->A);
	if (activeRegs->Y == 0)
	{
		// Set Zero Flag
		activeRegs->P |= 1 << Flags::Z;
	}
}

// Right now just implied Addressing
void M6502::execute(uint8_t OP)
{

	std::cout << "PC: 0x" << std::hex << unsigned(OP) << std::endl;
	std::cout << "A: " << unsigned(activeRegs->A) << std::endl;
	std::cout << "X: " << unsigned(activeRegs->X) << std::endl;
	std::cout << "Y: " << unsigned(activeRegs->Y) << std::endl;
	std::cout << "Status: " << std::bitset<8>(activeRegs->P) << std::endl;

	switch (OP)
	{
	case 0x00:
		// BRK, Force interrupt
		break;
	case 0x01:
		// ORA! Logical Inclusive OR
		ORA();
		break;
	case 0x8:
		cpuAddressSpace.pushStack(activeRegs->P);
		break;
	case 0x1A:
		// NOP
		return;
		break;
	case 0x2A:
		ROL();
		break;
	case 0x28:
		activeRegs->P = cpuAddressSpace.pullStack();
		break;
	case 0x40:
		// Return from interrupt
		RTI();
		break;
	case 0x45:
		// EOR - Zero Page. I'm assuming clears the register shit out?
		break;
	case 0x4e:
		// Logical Shift Right or division
		LSR();
		break;
	case 0x48:
		cpuAddressSpace.pushStack(activeRegs->A);
		break;
	case 0x60:
		activeRegs->PC = (cpuAddressSpace.pullStack() - 1);
		break;
	case 0x68:
		activeRegs->A = cpuAddressSpace.pullStack();
		break;
	case 0x69:
		ADC();
		break;
	case 0x78:
		activeRegs->P |= 1 << 2;
		break;
	case 0x83:
		// Rotate one bit to Left
		break;
	case 0x9A:
		cpuAddressSpace.pushStack(std::move(activeRegs->X));
		break;
	case 0x9D:
		// STA Absolute,X
		STA();
		break;
	case 0xA8:
		TAY();
		break;
	case 0xAA:
		activeRegs->X = std::move(activeRegs->A);
		break;
	case 0xBA:
		activeRegs->X = cpuAddressSpace.pullStack();
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