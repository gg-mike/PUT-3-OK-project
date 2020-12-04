#pragma once
#include <ostream>
#include <fstream>
#include "Instance.h"

double clamp(double val, double mn, double mx);

struct GenerationParams {
	GenerationParams() {}
	GenerationParams(size_t size, size_t tournamentSize, double crossoverRate, double mutationRate)
	: size(size), tournamentSize(tournamentSize) {
		this->crossoverRate = clamp(crossoverRate, 0, 1);
		this->mutationRate = clamp(mutationRate, 0, 1);
	}

	size_t size = 0;
	size_t tournamentSize = 0;
	double crossoverRate = 0;
	double mutationRate = 0;
};

class Generation
{
public:
	Generation() {}
	Generation(size_t coresNum, const std::vector<size_t>& processesLen, const GenerationParams& genParams) { init(coresNum, processesLen, genParams); }
	void init(size_t coresNum, const std::vector<size_t>& processesLen, const GenerationParams& genParams);
	void newGen();
	size_t getBestCmax() { return bestCmax; }
	double getAvgCmax() { return avgCmax; }
	friend std::ostream& operator<<(std::ostream& os, const Generation& generation);

private:
	size_t tournament(size_t k);
	void crossover(Instance& child1, Instance& child2, double crossoverRate);
	void mutation(Instance& child, double mutationRate);
	void calcCmaxs();

private:
	size_t bestCmax = SIZE_MAX;
	double avgCmax = DBL_MAX;
	GenerationParams genParams;
	size_t iter = 0;
	size_t coresNum;
	std::vector<size_t> processesLen = {};
	std::vector<Instance> instances = {};
};

