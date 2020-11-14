#include <iostream>
#include <fstream>
#include <vector>
#include "M6502.h"


int main()
{

	M6502 nes;

	nes.Reset();
	nes.update();

	return 0;
}