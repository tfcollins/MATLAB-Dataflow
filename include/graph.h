
// #include "flowMP.h"
// #include "utils.h"

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

#include "flowMP.h"

// Graph Class
class Graph
{
public:
        // Constructor
        Graph(std::string Name);

        // Deconstructor
        ~Graph();

        //Parameters
        std::string m_Name;
        std::vector<Worker*> Blocks;
        std::vector<std::string> Sources;
        std::vector<std::string> Sinks;
        bool m_BenchMarking;

        // Destructor
        //~Graph();

        // Run graph
        void run(double duration);

        // Monitor running graph
        void SimDuration(double duration);

        // Stop graph
        void shutdown();
};

void connect(Worker &aBlock, int outport, Worker &bBlock, int inport);

void SimDuration(double duration);

#endif
