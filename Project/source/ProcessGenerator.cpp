#include "pch.h"
#include "ProcessGenerator.h"
#include <random>

thread_local std::mt19937 gen{ std::random_device{}() };

template<typename T>
T random(T min, T max) {
	return std::uniform_int_distribution<T>{min, max}(gen);
}

ProcessGenerator::ProcessGenerator(int coreCount, int coreTime, int maxTaskTime) throw(int)
	: coreCount(coreCount), coreTime(coreTime), maxTaskTime(maxTaskTime) {
	if (coreCount < 0 || coreTime < 0 || maxTaskTime < 0)
		throw - 1;

	generateCoresVector();
}

ProcessGenerator::~ProcessGenerator() {
}

void ProcessGenerator::generateCoresVector() {
	coresVector.clear();
	coresVector.resize(coreCount);
	int ID = 0;
	for (int i = 0; i < coreCount; i++) {
		while (coresVector[i].GetTotalLength() + maxTaskTime < coreTime) {
			coresVector[i].AppendNewProcess(Process(ID++, random(1, maxTaskTime)));
			tasksCount++;
		}
		if (coresVector[i].GetTotalLength() < coreTime) {
			coresVector[i].AppendNewProcess(Process(ID++, coreTime - coresVector[i].GetTotalLength()));
			tasksCount++;
		}
	}
	generateTasksVector();
}

void ProcessGenerator::generateTasksVector() {
	tasksVector.clear();
	tasksVector.reserve(tasksCount);
	for (Core& core : coresVector) {
		for (const Process& process : core.GetProcesses()) {
			tasksVector.push_back(process.length);
		}
	}
}


