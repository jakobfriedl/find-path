#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

#include "Graph.h"

void setColor(int color);

int main(int argc, char *argv[])
{
    Graph graph;

    if(argc != 4){
        setColor(12); std::cout << "Invalid input format, please use: ";
        setColor(4); std::cout << "find_path filename start destination" << std::endl; setColor(7);
        return 0;
    }
    std::string file = argv[1];
    std::string start = argv[2];
    std::string dest = argv[3];

    graph.processInputFile(file);
    //graph.printAdjacencyList();

    try{
        graph.dijkstra(start, dest);
    }catch(std::invalid_argument& e){
        setColor(12); std::cout << e.what() << std::endl; setColor(7);
    }

    return 0;
}

void setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
