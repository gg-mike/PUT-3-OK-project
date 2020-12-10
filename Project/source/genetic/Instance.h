#pragma once
#include <vector>

struct Instance
{
	Instance() = default;
	Instance(size_t coresNum, const std::vector<size_t>& processesLen);
	void init(size_t coresNum, const std::vector<size_t>& processesLen);

	size_t calcCmax(const std::vector<size_t>& processesLen);

	size_t Cmax;
	size_t coresNum;
	std::vector<size_t> processes;
};
