#include "pch.h"
#include "ProcessGenerator.h"

int main() {
	int coresCount = 15, coreTime = 100, maxTaskLen = 30;

	ProcessGenerator pg(coresCount, coreTime, maxTaskLen);
	pg.toFile("assets/tasks1");

	for (Core& core : pg.getCoresVector()) {
		std::cout << core << core.getTotalLength() << std::endl;
	}
	std::cout << std::endl << std::endl;
	Processor processor(coresCount, pg.getTasksVector());

	std::cout << processor << std::endl;
	std::cout << "Cmax: " << processor.getCore(processor.findCMax()).getTotalLength();
	std::cout << std::endl << std::endl;
	
	processor.init("assets/tasks1");
	std::cout << processor << std::endl;
	std::cout << "Cmax: " << processor.getCore(processor.findCMax()).getTotalLength();
	std::cin.get();
}