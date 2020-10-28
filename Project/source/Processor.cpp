#include "pch.h"
#include "Processor.h"

static size_t counter = 1;

inline void PrintBox0() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cout << '|';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

inline void PrintBox1() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	std::cout << '|';
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

// ////////////////////
// Core
// ////////////////////

void Core::Init(size_t totalLength, const std::vector<Process>& processes) {
	this->totalLength = 0;
	this->processes.clear(); 
	this->processes = processes;
	CalculateTotalLength();
}

void Core::AppendNewProcess(Process process) {
	totalLength += process.length;
	processes.push_back(process);
}

void Core::EraseProcess(size_t ID) {
	auto it = std::find(processes.begin(), processes.end(), Process(ID, 0));
	if (it != processes.end())
		processes.erase(it);
}

Process Core::GetProcess(size_t ID) {
	for (const auto& process : processes)
		if (process.ID == ID)
			return process;
	return Process();
}

void Core::CalculateTotalLength() {
	for (const auto& process : processes)
		totalLength += process.length;
}

std::ostream& operator<<(std::ostream& os, const Core& core)
{
	for (const auto& process : core.processes) {
		for (size_t i = 0; i < process.length; i++) { 
			if (counter % 2)
				PrintBox1();
			else
				PrintBox0();
		}
		counter++;
	}
	counter = 1;
	return os;
}

// ////////////////////
// Processor
// ////////////////////

void Processor::Init(size_t N, const std::vector<size_t>& lengths) {
	this->N = N;
	cores.clear();
	for (size_t i = 0; i < N; i++)
		cores.push_back(Core());

	for (size_t i = 0; i < lengths.size(); i++)
		cores.at(FindLeastUsedCore()).AppendNewProcess(Process(i, lengths.at(i)));

}

void Processor::Init(const std::string& filepath, char sep) {
	std::ifstream ifs(filepath);
	std::string line;

	if (ifs.is_open()) {
		// Number of cores
		std::getline(ifs, line, sep);
		this->N = std::stoull(line);
		for (size_t i = 0; i < N; i++)
			cores.push_back(Core());

		// Number of processes [IGNORED]
		std::getline(ifs, line, sep);

		size_t i = 0;
		while (std::getline(ifs, line, sep)) {
			size_t len = std::stoull(line);
			cores.at(FindLeastUsedCore()).AppendNewProcess(Process(i, len));
		}
	}
	ifs.close();
}

const Core& Processor::GetCore(size_t ID) const {
	if (ID < cores.size())
		return cores[ID];
	return Core();
}

size_t Processor::FindCMax() {
	size_t maxLength = 0, maxLengthID = SIZE_MAX, i;

	for (i = 0; i < N; i++)
		if (maxLength < cores.at(i).GetTotalLength()) {
			maxLength = cores.at(i).GetTotalLength();
			maxLengthID = i;
		}

	return maxLengthID;
}

size_t Processor::FindLeastUsedCore() {
	size_t minLength = SIZE_MAX, minLengthID = SIZE_MAX, i;

	for (i = 0; i < N; i++)
		if (minLength > cores.at(i).GetTotalLength()) {
			minLength = cores.at(i).GetTotalLength();
			minLengthID = i;
		}
	return minLengthID;
}

std::ostream& operator<<(std::ostream& os, const Processor& processor) {
	for (size_t i = 0; i < processor.N; i++)
		os << std::setw(3) << i << " : " << processor.cores.at(i) << std::endl;
	return os;
}
