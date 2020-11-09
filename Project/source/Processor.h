#pragma once
#include <iomanip>
#include <ostream>
#include <vector>

struct Process {
	Process() = default;
	Process(size_t ID, size_t length) : ID(ID), length(length) {}

	bool operator==(const Process& other) {
		return this->ID == other.ID;
	}

	size_t ID = SIZE_MAX;
	size_t length = 0;
};

class Core {
public:
	Core() = default;
	Core(const std::vector<Process>& processes) : processes(processes) { calculateTotalLength(); }
	Core(size_t totalLength, const std::vector<Process>& processes) 
		: totalLength(totalLength), processes(processes) {}

	void init(size_t totalLength, const std::vector<Process>& processes);
	void appendNewProcess(Process process);
	void eraseProcess(size_t ID);
	size_t getTotalLength() const { return totalLength; }
	Process getProcess(size_t ID);
	const std::vector<Process>& getProcesses() const { return std::ref(processes); }
	
	friend std::ostream& operator<<(std::ostream& os, const Core& core);

private:
	void calculateTotalLength();

private:
	size_t totalLength = 0;
	std::vector<Process> processes = {};
};

class Processor
{
public:
	Processor(size_t N, const std::vector<size_t>& lengths) { init(N, lengths); }
	Processor(const std::string& filepath, char sep = '\n') { init(filepath); }
	void init(size_t N, const std::vector<size_t>& lengths);
	void init(const std::string& filepath, char sep = '\n');
	const Core& getCore(size_t ID) const;
	size_t findCMax();

	friend std::ostream& operator<<(std::ostream& os, const Processor& processor);

private:
	size_t findLeastUsedCore();

private:
	size_t N = 0;
	std::vector<Core> cores = {};
};
