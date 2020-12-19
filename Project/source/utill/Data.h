#pragma once
#include <string>
#include "genetic/Generation.h"

class Data
{
public:
	Data(const std::string& filepath);

	void algorithmInit(GenerationParams generationParams);

	size_t getTasksVectorSize() {
		return tasksVector.size();
	}

	std::vector<size_t>& getTasksVector() {
		return tasksVector;
	}

	size_t getCoreCount() {
		return coreCount;
	}

	std::string getInstance() {
		return instance;
	}

	std::string getAlgorithm() {
		return algorithm;
	}

private:
	std::vector<size_t> tasksVector;
	size_t coreCount;
	std::string instance, algorithm = "";
};

