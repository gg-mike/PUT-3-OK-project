#include "pch.h"
#include "ProcessGenerator.h"

int main() {
	int coresCount = 15, coreTime = 100, maxTaskLen = 30;

	ProcessGenerator pg(coresCount, coreTime, maxTaskLen);

	for (Core& core : pg.getCoresVector()) {
		std::cout << core << core.GetTotalLength()<< std::endl;
	}
	std::cout << std::endl << std::endl;

	Processor processor(coresCount, pg.getTasksVector());
	
	std::cout << processor << std::endl;
	std::cout << "Cmax: " << processor.GetCore(processor.FindCMax()).GetTotalLength();
	std::cin.get();
}