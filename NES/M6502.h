#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
struct registers
{
	uint8_t A;
	uint8_t X, Y; //used for holding things such as iteration counts or as offsets to addressing modes. The X register in addition can be used to set or get the SP.
	uint8_t P; // Flags. C, Z, V, S, I
	
	uint16_t SP; // 0x100 to 0x1FF
	uint16_t PC;

};


class M6502
{
private:
	registers* activeRegs;
	uint8_t ram[0x800];
	uint8_t stack[0x100];
	int remainingCycleCount;
	int i = 0;

public:
	void init();
	void Reset();
	void ADC();
	void AND();
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

