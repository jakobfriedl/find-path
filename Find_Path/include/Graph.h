#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
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
            std::shared_ptr<std::vector<std::shared_ptr<struct station>>> adjacentStations;
        }Station;

        std::shared_ptr<std::vector<std::shared_ptr<Station>>> stations;

        void setColor(int color);

    public:
        Graph();
        ~Graph();

        std::shared_ptr<std::vector<std::shared_ptr<Station>>> getStations() const;
        std::shared_ptr<Station> createNewStation(std::string name, std::string line, int cost);

        void createGraph(std::string filename);
};

#endif // GRAPH_H
