#include "Graph.h"
#include <iostream>
#include <set>

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

std::unordered_map<std::string, Graph::Station*> Graph::getStations() const{
    return this->stations;
}

Graph::Station* Graph::createNewStation(std::string name, std::string line, int cost){
    Station* station = new Station{name, line, cost};
    return station;
}

void Graph::processInputFile(std::string filename){
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

        this->createAdjacencyList(lineData);
    }
    //Close file
    readFile.close();
}

void Graph::createAdjacencyList(std::vector<std::string> lineData){
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

        //Adjacency list is empty
        if((int)this->stations.size() == 0){
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

void Graph::printAdjacencyList(){
    //Print adjacency-list
    for(const auto& s  : this->stations){
        std::cout << std::endl;
        std::cout << s.second->name << std::endl;
        for(const auto& t : s.second->adjacentStations){
            std::cout << "   " << t->name << " via " << t->line << " -- Costs: " << t->cost << std::endl;
        }
    }
    std::cout << std::endl << "Number of stations: " << this->stations.size() << std::endl;
}

void Graph::dijkstra(std::string start, std::string dest){
    //Checks if both input-variables are valid stations
    if(this->stations[start] == nullptr || this->stations[dest] == nullptr){
        throw std::invalid_argument("[Error] Please enter a valid start and destination station!");
    }

    //Start Dijkstra-Search
    setColor(9); std::cout << "Starting Dijkstra search from " << start << " to " << dest << std::endl; setColor(7);

    std::unordered_map<std::string, int> costs;
    std::unordered_map<std::string, bool> visited;
    std::unordered_map<std::string, Station*> path;
    std::set<std::pair<int, std::string>> checkNext;

    //Insert start-node to
    costs[start] = 0;
    checkNext.insert(std::make_pair(0, start));
    path[start] = this->stations[start];

    auto counter = 0;

    //Search until destination is found
    while(checkNext.begin()->second != dest){
        auto distance = checkNext.begin()->first;
        auto stationName = checkNext.begin()->second;

        setColor(3); std::cout << stationName << " via " << this->stations[stationName]->line << std::endl; setColor(7);

        for(auto& adj : this->stations[stationName]->adjacentStations){
            //Check if Station has already been visited
            if(visited[adj->name]) continue;

            //Cost has not been initialised yet
            if(costs[adj->name] == 0 && adj->name != start){
                costs[adj->name] = distance + adj->cost;
                checkNext.insert(std::make_pair(costs[adj->name], adj->name));
            }

            //Update Distance if necessary
            if(distance + adj->cost < costs[adj->name]){
                setColor(5); std::cout << "   [UPDATE] "; setColor(7); std::cout << adj->name << " from " << costs[adj->name] << " to " << distance+adj->cost << std::endl;
                auto toUpdate = checkNext.find(std::make_pair(costs[adj->name], adj->name));

                //Delete old entry from set
                if(toUpdate != checkNext.end()){
                    checkNext.erase(toUpdate);
                }

                //Insert new entry with updated Cost
                costs[adj->name] = distance + adj->cost;
                checkNext.insert(std::make_pair(costs[adj->name], adj->name));
            }
            path[adj->name] = this->stations[stationName];

            std::cout << "   checking " << adj->name << " via " << adj->line << ": " << costs[adj->name] << std::endl;
            counter++;
        }
        //Remove pair with lowest distance from set and add it to visited
        checkNext.erase(checkNext.begin());
        visited[stationName] = true;
    }

    std::cout << "=========================" << std::endl;
    std::cout << "Start: " << start << std::endl;
    std::cout << "Destination: " << dest << std::endl;
    std::cout << "Total Cost: " << costs[dest] << std::endl;
    setColor(8); std::cout << this->getPath("", path, dest) << std::endl; setColor(7);

    //std::cout << "Checks: " << counter << std::endl;
}

std::string Graph::getPath(std::string pathString, std::unordered_map<std::string, Station*> path, std::string station){
    //Check if start station is reached, since path(start) == start
    if(station != path[station]->name){
        int cost = INT_MAX;
        //Concatenate Stations to output-string
        for(const auto& s : this->stations[station]->adjacentStations){
            if(s->name == path[station]->name){
                if(s->cost < cost)
                    cost = s->cost;
            }
        }

        pathString = getPath(pathString, path, path[station]->name) + /**/"=[ " + std::to_string(cost) + " ]=>[ " + station + " (" + path[station]->line + ") ]";
    }else{
        //Add first station
        pathString = "[ " + station + " ]" + pathString;
    }
    return pathString;
}


void Graph::setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

