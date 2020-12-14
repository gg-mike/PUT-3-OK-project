#pragma once
#include "./greedy/Processor.h"


class ProcessGenerator {
public:
	ProcessGenerator(int coreCount, int coreTime, int maxTaskTime);
	~ProcessGenerator();

	void generateCoresVector();

	std::vector<size_t>& getTasksVector() { return tasksVector; }
	std::vector<Core>& getCoresVector() { return coresVector; }
	void toFile(const std::string& filepath);

private:
	void generateTasksVector();

private:
	int coreCount, coreTime, maxTaskTime;
	int tasksCount = 0;
	std::vector<Core> coresVector;
	std::vector<size_t> tasksVector;
};


