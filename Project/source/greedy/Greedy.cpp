#include "pch.h"
#include "Greedy.h"
#include "Processor.h"

void Greedy::test(Data &data)
{
	Processor processor;
	processor.init(data.getCoreCount(), data.getTasksVector());
	std::cout << "m=" << std::setw(2) << data.getCoreCount() << ", n=" << std::setw(4) << data.getTasksVectorSize() << ": "
		<< std::setw(6) << processor.getCore(processor.findCMax()).getTotalLength() << "\n";

}
