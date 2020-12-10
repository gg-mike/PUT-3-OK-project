#include "pch.h"
#include "Data.h"

Data::Data(const std::string& filepath)
{
	std::ifstream ifs(filepath);
	std::string line;

	if (ifs.is_open()) {
		// Number of cores
		std::getline(ifs, line);
		coreCount = std::stoull(line);
		// Number of processes [IGNORED]
		std::getline(ifs, line);

		while (std::getline(ifs, line))
			tasksVector.push_back(std::stoull(line));
	}
	else {
		std::cout << "Data openning error!\n";
	}
	ifs.close();

	title = "Cores: " + std::to_string(coreCount) + ", Tasks: " + std::to_string(getTasksVectorSize());
}
