#include "Graph.h"
#include <iostream>

Graph::Graph()
{
    #if DEBUG
        std::cout << "Created Graph" << std::endl;
    #endif // DEBUG
}

Graph::~Graph()
{
    #if DEBUG
        std::cout << "Destroyed Graph" << std::endl;
    #endif // DEBUG
}

std::map<std::string, Graph::Station*> Graph::getStations() const{
    return this->stations;
}

Graph::Station* Graph::createNewStation(std::string name, std::string line, int cost){
    Station* station = new Station{name, line, cost};
    return station;
}

void Graph::createGraph(std::string filename){
    std::ifstream readFile(filename);

    //Open file
    if(!readFile.is_open()){
        setColor(12); std::cerr << "[Error] Could not open file!" << std::endl; setColor(7);
        return;
    }
    while(!readFile.eof()){
        std::string input;
        //Get line-information from input file as a single line (delimited by \n)
        getline(readFile, input);

        //Split string using different delimiters to get line data in vector
        std::vector<std::string> lineData;
        int startIndex = 0;
        int endIndex = 0;
        int counter = -1;

        std::string delimiter;
        do{
            //Check which delimiter to use based on current position in the string
            if(counter == 0) delimiter = ":";
            if(counter>0) delimiter = " \"";
            if(counter %2 == 0 && counter != 0) delimiter = "\" ";
            endIndex = input.find(delimiter, startIndex);

            //Add data to line-vector
            std::string data = input.substr(startIndex, endIndex - startIndex);
            lineData.push_back(data);

            startIndex = endIndex + delimiter.size();
            counter++;
        }while(endIndex < input.size());

        //Delete " from last entry, which is not removed by delimiters
        lineData.back().pop_back();

        //Delete empty vector entries
        lineData.erase(lineData.begin()+0);
        lineData.erase(lineData.begin()+1);

        auto lineName = lineData.at(0);

        //Add station-information to adjacency-list
        for(auto i = 1; i < (int)lineData.size(); i+=2){
            auto alreadyExists = false;

            auto current = this->createNewStation(lineData.at(i), lineName, 0);

            //Add adjacent stations to struct, if they exist
            if(i<=(int)lineData.size()-2){
                auto next = this->createNewStation(lineData.at(i+2), lineName, std::stoi(lineData.at(i+1)));
                current->adjacentStations.push_back(next);
            }
            if(i>=3){
                auto previous = this->createNewStation(lineData.at(i-2), lineName, std::stoi(lineData.at(i-1)));
                current->adjacentStations.push_back(previous);
            }

            if((int)this->stations.size() == 0){
                //Adjacency list is empty
                this->stations[current->name] = current;
            }else{
                //Check if current station has an entry in adjacency-list
                if(this->stations[current->name] != nullptr){
                    //Add adjacent stations from current station to entry in adjacency-list
                    for(const auto& station : current->adjacentStations){
                        this->stations[current->name]->adjacentStations.push_back(station);
                    }
                    alreadyExists = true;
                }
                if(!alreadyExists){
                    //Create new entry in adjacency-list for current station
                    this->stations[current->name] = current;
                }
            }
        }
    }

    //Print adjacency-list
    for(const auto& s  : this->stations){
        std::cout << std::endl;
        std::cout << s.second->name << std::endl;
        for(const auto& t : s.second->adjacentStations){
            std::cout << "   " << t->name << " via " << t->line << " -- Costs: " << t->cost << std::endl;
        }
    }
    std::cout << this->stations.size() << std::endl;

    //Close file
    readFile.close();
}

void Graph::dijkstra(std::string start, std::string dest){
    //Checks if both input-variables are valid stations
    if(this->stations[start] == nullptr || this->stations[dest] == nullptr){
        setColor(12); std::cout << "[Error] Please enter a valid start and destination station!" << std::endl; setColor(7);
        return;
    }

    //Start Dijkstra
    setColor(3); std::cout << "Starting Dijkstra search from " << start << " to " << dest << std::endl; setColor(7);

}

void Graph::setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

