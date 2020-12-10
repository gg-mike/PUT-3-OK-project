#pragma once
#include <string>

class Data
{
public:
	Data(const std::string& filepath);

	size_t getTasksVectorSize() {
		return tasksVector.size();
	}

	std::vector<size_t>& getTasksVector() {
		return tasksVector;
	}

	size_t getCoreCount() {
		return coreCount;
	}

	std::string getTitle() {
		return title;
	}

private:
	std::vector<size_t> tasksVector;
	size_t coreCount;
	std::string title;
};

