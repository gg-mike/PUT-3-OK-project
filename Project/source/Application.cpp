#include "pch.h"
#include "ProcessGenerator.h"
#include "genetic/Generation.h"

namespace Data {
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
	std::vector<std::string> bestDestFilepaths = {
		"results/best_m10n21.csv",
		"results/best_m20n41.csv",
		"results/best_m25n198.csv",
		"results/best_m50n101.csv",
		"results/best_m10n200.csv",
		"results/best_m50n200.csv",
		"results/best_m50n200lpt.csv",
		"results/best_m50n1000.csv"
	};
	std::vector<std::string> avgDestFilepaths = {
		"results/avg_m10n21.csv",
		"results/avg_m20n41.csv",
		"results/avg_m25n198.csv",
		"results/avg_m50n101.csv",
		"results/avg_m10n200.csv",
		"results/avg_m50n200.csv",
		"results/avg_m50n200lpt.csv",
		"results/avg_m50n1000.csv"
	};
	std::vector<size_t> coresNums = std::vector<size_t>(Data::size, 0);
	std::vector<std::vector<size_t>> processesLens = std::vector<std::vector<size_t>>(Data::size, std::vector<size_t>());
	std::vector<GenerationParams> geneticParams = std::vector<GenerationParams>(Data::size, GenerationParams());
}

void initData(const std::string& filepath, size_t& coresNum, std::vector<size_t>& processesLen, GenerationParams& genParams)
{
	std::ifstream ifs(filepath);
	std::string line;
	processesLen.clear();

	if (ifs.is_open()) {
		// Number of cores
		std::getline(ifs, line);
		coresNum = std::stoull(line);
		// Number of processes [IGNORED]
		std::getline(ifs, line);

		while (std::getline(ifs, line))
			processesLen.push_back(std::stoull(line));
	}
	ifs.close();

	genParams = GenerationParams(processesLen.size() * 10, clamp(processesLen.size() / 10, 2, SIZE_MAX), .75, .01);
}

void initAll() {
	for (size_t i = 0; i < Data::size; i++)
		initData(Data::srcFilepaths[i], Data::coresNums[i], Data::processesLens[i], Data::geneticParams[i]);
}

void testGreedy() {
	Processor processor;
	for (size_t i = 0; i < Data::size; i++) {
		processor.init(Data::coresNums[i], Data::processesLens[i]);
		std::cout << "m=" << std::setw(2) << Data::coresNums[i] << ", n=" << std::setw(4) << Data::processesLens[i].size() << ": "
			<< std::setw(6) << processor.getCore(processor.findCMax()).getTotalLength() << "\n";
	}

}

void testGenetic() {
	Generation gen;
	for (size_t i = 0; i < Data::size; i++) {
		gen.init(Data::coresNums[i], Data::processesLens[i], Data::geneticParams[i]);
		std::cout << "m=" << std::setw(2) << Data::coresNums[i] << ", n=" << std::setw(4) << Data::processesLens[i].size() << ":\n" << gen;
		size_t iter = Data::coresNums[i] * 10;
		for (size_t j = 0; j < iter; j++) {
			std::cout << " " << static_cast<size_t>((j * 100.0) / iter) << "%";
			gen.newGen();
			std::cout << '\r';
		}
		std::cout << gen << std::endl;
	}
}

void testGenetic(size_t first, size_t last, size_t trailsNum, const std::string& title) {
	Generation gen;
	for (size_t i = first; i <= last; i++) {
		std::ofstream bestOfs(Data::bestDestFilepaths[i], std::ios::app);
		std::ofstream avgOfs(Data::avgDestFilepaths[i], std::ios::app);
		bestOfs << title << std::endl;
		avgOfs << title << std::endl;
		size_t iter = Data::coresNums[i] * 10;
		for (size_t t = 0; t < trailsNum; t++) {
			gen.init(Data::coresNums[i], Data::processesLens[i], Data::geneticParams[i]);
			std::cout << "(" << t << "/" << trailsNum << ")\n";
			for (size_t j = 0; j < iter; j++) {
				std::cout << " " << static_cast<size_t>((j * 100.0) / iter) << "%";
				bestOfs << gen.getBestCmax() << ';';
				avgOfs << gen.getAvgCmax() << ';';
				gen.newGen();
				std::cout << '\r';
			}
			bestOfs << gen.getBestCmax() << ";\n";
			avgOfs << gen.getAvgCmax() << ";\n";
		}
		bestOfs << std::endl;
		avgOfs << std::endl;
		bestOfs.close();
		avgOfs.close();
		std::cout << "done\n";
	}
	std::cout << "end\n";

}

int main() {
	initAll();
	testGreedy();
	std::cout << std::endl;
	testGenetic(0, 2, 10, "genetic (prLen*10, coresN*10, size/100, .75, .01)");
	std::cout << "end";
	std::cin.get();
}
