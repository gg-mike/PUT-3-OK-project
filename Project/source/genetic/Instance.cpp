#include "pch.h"
#include "Instance.h"

std::random_device devI;
std::mt19937 rngI(devI());

void Instance::init(size_t coresNum, const std::vector<size_t>& processesLen) {
	this->coresNum = coresNum;
	Cmax = SIZE_MAX;
	std::uniform_int_distribution<std::mt19937::result_type> coreAssign(0, coresNum - 1ull);
	for (size_t i = 0; i < processesLen.size(); i++)
		processes.push_back(coreAssign(rngI));

	calcCmax(processesLen);
}

size_t Instance::calcCmax(const std::vector<size_t>& processesLen) {
	std::vector<size_t> coresC = std::vector<size_t>(coresNum, 0);
	
	for (size_t i = 0; i < processes.size(); i++)
		coresC[processes[i]] += processesLen[i];

	Cmax = *std::max_element(coresC.begin(), coresC.end());
	return Cmax;
}
