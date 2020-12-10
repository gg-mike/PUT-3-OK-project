#include "pch.h"
#include "Generation.h"

std::random_device devG;
std::mt19937 rngG(devG());

void Generation::init(size_t coresNum, const std::vector<size_t>& processesLen, const GenerationParams& genParams) {
	this->genParams = genParams;
	this->iter = 0;
	this->coresNum = coresNum;
	this->processesLen = processesLen;
	this->instancesCount = genParams.instancesCount;
	this->instances = new Instance[instancesCount];
	this->instancesBuffor = new Instance[instancesCount];
	for (size_t i = 0; i < instancesCount; i++)
		instances[i].init(coresNum, processesLen);
	calcCmaxs();
}

void Generation::newGen() {
	int i = 0;
	while (i < instancesCount) {
		instancesBuffor[i] = instances[tournament(genParams.tournamentSize)];
		instancesBuffor[i + 1] = instances[tournament(genParams.tournamentSize)];

		crossover(instancesBuffor[i], instancesBuffor[i + 1], genParams.crossoverRate);

		mutation(instancesBuffor[i], genParams.mutationRate);
		//mutation(instancesBuffor[i + 1], genParams.mutationRate);

		instancesBuffor[i].calcCmax(processesLen);
		instancesBuffor[i + 1].calcCmax(processesLen);

		i += 2;
	}
	Instance* temp = instances;
	instances = instancesBuffor;
	instancesBuffor = temp;
	iter++;
	calcCmaxs();
}

size_t Generation::tournament(size_t k) {
	size_t winner = SIZE_MAX;
	size_t winnerCmax = SIZE_MAX;
	std::uniform_int_distribution<std::mt19937::result_type> participantChoosing(0, instancesCount - 1);
	for (size_t i = 0; i < k; i++) {
		size_t participant = participantChoosing(rngG);
		if (winner != participant && instances[participant].Cmax < winnerCmax) {
			winner = participant;
			winnerCmax = instances[participant].Cmax;
		}
	}
	return winner;
}

void Generation::crossover(Instance& child1, Instance& child2, double crossoverRate) {
	for (size_t p = static_cast<size_t>(crossoverRate * 100); p < instances[0].processes.size(); p++)
		std::swap(child1.processes[p], child2.processes[p]);
}

void Generation::mutation(Instance& child, double mutationRate) {
	std::uniform_int_distribution<std::mt19937::result_type> mutationChoosing(0, 100);
	std::uniform_int_distribution<std::mt19937::result_type> coreAssign(0, instances[0].coresNum - 1);
	for (auto& process : child.processes)
		if (mutationChoosing(rngG) <= mutationRate * 100)
			process = coreAssign(rngG);
}

void Generation::calcCmaxs() {
	double sumCmax = 0;
	bestCmax = SIZE_MAX;

	for (size_t i = 0; i < instancesCount; i++)
	{
		sumCmax += instances[i].Cmax;
		if (instances[i].Cmax < bestCmax)
			bestCmax = instances[i].Cmax;
	}
	avgCmax = sumCmax / instancesCount;
}


std::ostream& operator<<(std::ostream& os, const Generation& generation)
{
	os << std::setw(4) << generation.iter
		<< ": Best Cmax=" << std::setw(8) << generation.bestCmax
		<< ", Average Cmax=" << std::setw(8) << generation.avgCmax;
	/*
	os << ", process: {\n";
	for (auto instance : generation.instances) {
		for (auto process : instance.processes)
			os << process << " ";
		os << " == " << instance.Cmax << std::endl;
	}
	os << "}";
	*/
	os << std::endl;
	return os;
}
