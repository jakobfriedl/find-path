#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
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
        std::map<std::string, Station*> stations;

        void setColor(int color);

    public:
        Graph();
        ~Graph();

        std::map<std::string, Station*> getStations() const;
        Station* createNewStation(std::string name, std::string line, int cost);

        void createGraph(std::string filename);

        void dijkstra(std::string start, std::string dest);
};

#endif // GRAPH_H
