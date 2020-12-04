#include "pch.h"
#include "Generation.h"

std::random_device devG;
std::mt19937 rngG(devG());

double clamp(double val, double mn, double mx) {
	if (val < mn)
		return mn;
	else if (val > mx)
		return mx;
	return val;
}

void Generation::init(size_t coresNum, const std::vector<size_t>& processesLen, const GenerationParams& genParams) {
	this->genParams = genParams;
	this->iter = 0;
	this->coresNum = coresNum;
	this->processesLen = processesLen;
	instances.clear();
	for (size_t i = 0; i < genParams.size; i++)
		instances.push_back(Instance(coresNum, processesLen));
	calcCmaxs();
}

void Generation::newGen() {
	std::vector<Instance> newInstances;
	
	while (newInstances.size() < genParams.size) {
		Instance child1 = instances[tournament(genParams.tournamentSize)];
		Instance child2 = instances[tournament(genParams.tournamentSize)];

		crossover(child1, child2, genParams.crossoverRate);

		mutation(child1, genParams.mutationRate);
		mutation(child2, genParams.mutationRate);

		child1.calcCmax(processesLen);
		child2.calcCmax(processesLen);

		newInstances.push_back(child1);
		newInstances.push_back(child2);
	}

	instances.clear();
	instances = newInstances;
	iter++;
	calcCmaxs();
}

size_t Generation::tournament(size_t k) {
	size_t winner = SIZE_MAX;
	size_t winnerCmax = SIZE_MAX;
	std::uniform_int_distribution<std::mt19937::result_type> participantChoosing(0, genParams.size - 1);
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
	for (size_t p = static_cast<size_t>(crossoverRate * 100); p < instances.front().processes.size(); p++)
		std::swap(child1.processes[p], child2.processes[p]);
}

void Generation::mutation(Instance& child, double mutationRate) {
	std::uniform_int_distribution<std::mt19937::result_type> mutationChoosing(0, 100);
	std::uniform_int_distribution<std::mt19937::result_type> coreAssign(0, instances.front().coresNum - 1);
	for (auto& process : child.processes)
		if (mutationChoosing(rngG) <= mutationRate * 100)
			process = coreAssign(rngG);
}

void Generation::calcCmaxs() {
	double sumCmax = 0;
	bestCmax = SIZE_MAX;
	for (const auto& instance : instances) {
		sumCmax += instance.Cmax;
		if (instance.Cmax < bestCmax)
			bestCmax = instance.Cmax;
	}
	avgCmax = sumCmax / instances.size();
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
