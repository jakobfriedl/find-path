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

std::shared_ptr<std::vector<std::shared_ptr<Graph::Station>>> Graph::getStations() const{
    return this->stations;
}

std::shared_ptr<Graph::Station> Graph::createNewStation(std::string name, std::string line, int cost){
    return std::make_shared<Station>(Station{name, line, cost, nullptr});
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


        for(const auto& s: lineData){
            std::cout << s << std::endl;
        }
        std::cout << std::endl;
    }
    //Close file
    readFile.close();
}

void Graph::setColor(int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

