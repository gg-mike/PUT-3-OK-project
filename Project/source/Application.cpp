#include "pch.h"
#include "genetic/Genetic.h"
#include "utill/ProcessGenerator.h"
#include "genetic/Generation.h"
#include "greedy/Greedy.h"


namespace AllData {
	size_t size = 8;
	std::vector<std::string> srcFilepaths = {
		"assets/m10n21.txt",
		"assets/m20n41.txt",
		"assets/m25n198.txt",
		"assets/m50n101.txt",
		"assets/m10n200.txt",
		"assets/m50n200.txt",
		"assets/m50n200lpt.txt",
		"assets/m50n1000.txt"
	};
	std::vector<std::string> resDestFilepaths = {
		"results/res_m10n21.csv",
		"results/res_m20n41.csv",
		"results/res_m25n198.csv",
		"results/res_m50n101.csv",
		"results/res_m10n200.csv",
		"results/res_m50n200.csv",
		"results/res_m50n200lpt.csv",
		"results/res_m50n1000.csv"
	};
}

void testGreedy(size_t first, size_t last)
{
	for (size_t index = first; index < clamp(last, first, AllData::size); index++)
	{
		Data data(AllData::srcFilepaths[index]);
		std::cout << "VectorSize: " << data.getTasksVectorSize() << std::endl;
		Greedy::test(data);
	}
}

void testGenetic(size_t first, size_t last)
{
	for (size_t index = first; index < clamp(last,first, AllData::size); index++)
	{
		Data data(AllData::srcFilepaths[index]);
		GenerationParams genParams(clamp(10 * data.getTasksVectorSize(), 100ull, 500ull), clamp(data.getTasksVectorSize() / 10, 10ull, 50ull), .75, .01, 10);
		std::cout << "VectorSize: " << data.getTasksVectorSize() << std::endl;
		Genetic::test(data, genParams, AllData::resDestFilepaths[index]);
	}
}


int main() 
{

	testGenetic(0, 10);

	

	std::cout << "end";
	std::cin.get();
}
