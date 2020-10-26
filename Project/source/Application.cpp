#include "pch.h"
#include "Processor.h"

int main() {
	std::vector<size_t> lens = { 4, 3, 2, 4, 1, 3, 2, 4, 5 };
	Processor processor(3, lens);

	std::cout << processor << std::endl;
	std::cout << "Cmax: " << processor.GetCore(processor.FindCMax()).GetTotalLength();
	std::cin.get();
}