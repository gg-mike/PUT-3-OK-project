#pragma once
#include <ostream>
#include <fstream>
#include "Instance.h"

template<typename T>
T clamp(T val, T mn, T mx) {
	if (val < mn)
		return mn;
	else if (val > mx)
		return mx;
	return val;
}

struct GenerationParams {
	GenerationParams() = default;
	GenerationParams(size_t instancesCount, size_t tournamentSize, double crossoverRate, double mutationRate, size_t trialsNum)
	: instancesCount(instancesCount), tournamentSize(tournamentSize), trialsNum(trialsNum) {
		this->crossoverRate = clamp(crossoverRate, 0.0, 1.0);
		this->mutationRate = clamp(mutationRate, 0.0, 1.0);
	}

	size_t instancesCount = 10;
	size_t trialsNum = 1;
	size_t tournamentSize = 0;
	double crossoverRate = 0;
	double mutationRate = 0;
};

class Generation
{
public:
	Generation() = default;
	Generation(size_t coresNum, const std::vector<size_t>& processesLen, const GenerationParams& genParams) { init(coresNum, processesLen, genParams); }
	void init(size_t coresNum, const std::vector<size_t>& processesLen, const GenerationParams& genParams);
	~Generation();
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
	size_t coresNum = 0;
	std::vector<size_t> processesLen;
	size_t instancesCount = 0;
	Instance* instances = nullptr;
	Instance* instancesBuffor = nullptr;
};

