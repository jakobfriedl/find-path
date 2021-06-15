#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

#include "Graph.h"

void setColor(int color);

int main(int argc, char *argv[])
{
    Graph graph;

    if(argc != 4 && argc != 2){
        setColor(12); std::cout << "Invalid input format, please use: ";
        setColor(4); std::cout << "* find_path filename start destination *";
        setColor(12); std::cout << " OR ";
        setColor(4); std::cout << "* find_path filename *" << std::endl; setColor(7);
        return 0;
    }
    std::string file = argv[1], start, dest;
    if(argc == 4){
        start = argv[2];
        dest = argv[3];
    }else{
        //Get Start and Destination if station name consists of multiple words
        setColor(8); std::cout << "Please define start and destination: " << std::endl;
        setColor(3); std::cout << "  Start: "; setColor(7);
        getline(std::cin, start);
        setColor(3); std::cout << "  Destination: "; setColor(7);
        getline(std::cin, dest);
    }

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
