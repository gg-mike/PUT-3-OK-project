#include "pch.h"
#include "Genetic.h"
#include <genetic\Generation.h>
#include <thread>


void runGenetic(size_t iterations, Data data, GenerationParams genParams, std::vector<size_t>& bestCmaxs, std::vector<double>& avgCmaxs)
{
	Generation gen(data.getCoreCount(), data.getTasksVector(), genParams);
	for (size_t j = 0; j < iterations; j++) {
		//std::cout << " " << static_cast<size_t>((j * 100.0) / iterations) << "%";
		bestCmaxs[j] += gen.getBestCmax();
		avgCmaxs[j] += gen.getAvgCmax();
		gen.newGen();
		//std::cout << '\r';
	}
	//std::cout << "thread done: " << std::this_thread::get_id() << std::endl;
}

void Genetic::test(Data& data, GenerationParams& generationParams, const std::string& outputFileName) 
{
	std::ofstream ofs(outputFileName);
	if (ofs.is_open())
	{
		ofs << "Genetic " + data.getTitle() << "\nN;Best;Avg\n";
		size_t iter = 500;

		std::vector<std::vector<size_t>> bestCmaxs(generationParams.trialsNum, std::vector<size_t>(iter, 0));
		std::vector<std::vector<double>> avgCmaxs(generationParams.trialsNum, std::vector<double>(iter, 0));

		std::vector<std::thread> threads;


		for (size_t t = 0; t < generationParams.trialsNum; t++)
		{
			threads.push_back(std::thread(runGenetic, iter, data, generationParams, std::ref(bestCmaxs[t]), std::ref(avgCmaxs[t])));
		}
		
		for (std::thread& t : threads)
		{
			std::cout << "thread: " << t.get_id();
			t.join();
			std::cout << " ended\n";
		}

		for (size_t i = 0; i < iter; i++)
		{
			size_t best = MAXUINT64;
			double avg = 0;
			for (size_t j = 0; j < generationParams.trialsNum; j++)
			{	
				avg += avgCmaxs[j][i];
				if (bestCmaxs[j][i] < best)
					best = bestCmaxs[j][i];
			}
			ofs << i << ';' << best << ';' << avg / generationParams.trialsNum << std::endl;
		}
		
		ofs << std::endl;
		ofs.close();
		std::cout << "done\n";
	} 
	else {
		std::cout << "genetic opening error" << std::endl;
	}
}
