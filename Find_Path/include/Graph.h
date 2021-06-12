#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <windows.h>

class Graph
{
    private:
        typedef struct station{
            std::string name;
            std::string line;
            int cost;
            std::vector<struct station*> adjacentStations;
        }Station;

        //Adjacency List
        std::unordered_map<std::string, Station*> stations;
        std::string getPath(std::string pathString, std::unordered_map<std::string, Station*> path, std::string station);

        void setColor(int color);

    public:
        Graph();
        ~Graph();

        std::unordered_map<std::string, Station*> getStations() const;
        Station* createNewStation(std::string name, std::string line, int cost);

        void processInputFile(std::string filename);
        void createAdjacencyList(std::vector<std::string> lineData);
        void printAdjacencyList();

        void dijkstra(std::string start, std::string dest);
};

#endif // GRAPH_H
