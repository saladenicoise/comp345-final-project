//
// Created by Ryan on 2021-09-24.
//
#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <set>
#include "Map.h"

using namespace std;

Player::Player(){ //Simply a dummy constructor for player.
  playerName = "defaultName";
};

Player::Player(Player &player) { //Nothing to copy since player is currently a dummy class.
  this->playerName = player.playerName;
}

Player& Player::operator=(const Player &player) { //dummy player assignment operator.
    this->playerName = player.playerName;
    return *this;
}

ostream& operator<<(std::ostream& os, const Player& player){ //dummy stream insertion overload.
    return os << player.playerName;
}

Territory::Territory(string countryName, int continentNumber, int countryNumber){ //Territory constructor for default player/armycount
    this->countryName = countryName;
    this->continentNumber = continentNumber;
    this->countryNumber = countryNumber;
    playerOccupying = new Player();
    armyCount = 0;
    edges = vector<Territory*>(0);
}
Territory::Territory(string countryName, int continentNumber, int countryNumber, int armyCount, Player* playerOccupying){ //Territory constructor with player/army count
    this->countryName = countryName;
    this->continentNumber = continentNumber;
    this->countryNumber = countryNumber;
    this->armyCount = armyCount;
    edges = vector<Territory*>(0);
    this->playerOccupying = playerOccupying;

}

Territory::Territory(Territory &territory) { //Territory copy constructor.
    this->countryNumber = territory.countryNumber;
    this->continentNumber = territory.continentNumber;
    this->countryName = territory.countryName;
    this->armyCount = territory.armyCount;
    this->playerOccupying = new Player(*territory.playerOccupying);
    this->edges = vector<Territory *>(territory.edges.size());

        for(Territory* t : territory.edges){
            this->edges.push_back(new Territory(*t));
        }

}
ostream& operator<<(std::ostream& os, const Territory& territory){ //Territory ostream insert overload
    return os << territory.countryName;
}

Territory& Territory::operator=(const Territory &territory) {//Territory = operator.
    this->countryNumber = territory.countryNumber;
    this->continentNumber = territory.continentNumber;
    this->countryName = territory.countryName;
    this->armyCount = territory.armyCount;
    this->playerOccupying = new Player(*territory.playerOccupying);
    this->edges = vector<Territory *>(territory.edges.size());

    for(Territory* t : territory.edges){
        this->edges.push_back(new Territory(*t));
    }
    return *this;

}

Territory::~Territory(){ //Territory destructor

}

Map::Map(){ //Map constructor
    string mapName;
    territories = vector<Territory*>(0);
    continentList = vector<vector<int>>(0);
    adjacencyList = vector<vector<int>>(0);
};

Map::Map(Map &map){ //Map copy constructor
    this->mapName = map.mapName;
    this->continentList = map.continentList;
    this->adjacencyList = map.adjacencyList;
    this->territories = vector<Territory*>(map.territories.size());
    for(Territory* t : map.territories){
        this->territories.push_back(new Territory(*t));
    }
}

Map& Map::operator=(const Map &map) { //Map = operator
    this->mapName = map.mapName;
    this->continentList = map.continentList;
    this->adjacencyList = map.adjacencyList;
    this->territories = vector<Territory*>(map.territories.size());
    for(Territory* t : map.territories){
        this->territories.push_back(new Territory(*t));
    }
    return *this;
}

ostream& operator<<(std::ostream& os, const Map& map){ //Map ostream insert overload
    return os << map.mapName;
}

Map::~Map(){ //Map destructor
    for(Territory* t : territories){
        delete t;
    }
}

bool Map::validate(){ //Map validater.

    //First part of validate, to check for if it contains a fully connected graph.
        vector<int> validater(territories.size(), 0);
            validater[0] = 1;
            stack<int> stack;
            for(int j : adjacencyList[0]) {
                validater[j] = 1;
                stack.push(j);
            }
            while(!stack.empty()) {

                int currentNode = stack.top();
                stack.pop();
                for(int j : adjacencyList[currentNode]){

                    if(validater[j] == 1) continue;
                    else {

                        (validater[j] = 1);
                        stack.push(j);
                    }
                }
            }
            for(int i = 0; i < validater.size(); i++) {
                if (validater[i] == 0) {
                    cout << "this is not a fully connected graph" << endl;
                  return false;
                }
            }
                cout << "this is a fully connected graph" << endl;

        //Second part of validate, to check if all continents are fully connected sub-graphs.
            for(vector<int> vec : continentList) {
                vector<int> continentValidaterHelper(territories.size(), 1);
                for (int i : vec) {

                    continentValidaterHelper[i] = 0;
                }
                stack.push(vec[0]);
                continentValidaterHelper[vec[0]] = 1;
                while (!stack.empty()) {
                    int currentNode = stack.top();
                    stack.pop();
                    for (int j : adjacencyList[currentNode]) {
                        if (continentValidaterHelper[j] == 1) continue;
                        else {
                            (continentValidaterHelper[j] = 1);
                            stack.push(j);
                        }
                    }
                }
                for (int i : continentValidaterHelper) {

                    if (i == 0) {

                        cout << "not every continent is fully connected sub graph" << endl;
                        return false;
                    }
                }
            }
        cout << "Every continent is a fully connected sub graph" << endl;

            //Last part of validate, to make sure all countries are only included once.
            set<int> continentChecker;
            for(vector<int> i : continentList){
                for(int j : i){
                    if(continentChecker.count(j) > 0){
                        "There is duplicate countries in the continents";
                        return false;
                    }else{
                        continentChecker.insert(j);
                    }
                }
            }
            cout << "There are no duplicate countries in the continents" << endl;

    return true;
}


MapLoader::MapLoader(){ //MapLoader constructor
    loadedMaps = vector<Map*>(0);
}

MapLoader::MapLoader(MapLoader &mapLoader){ //MapLoader copy constructor
    this->loadedMaps = vector<Map*>(mapLoader.loadedMaps.size());
    for(Map* m : mapLoader.loadedMaps){
        this->loadedMaps.push_back(new Map(*m));
    }
}

MapLoader& MapLoader::operator=(const MapLoader &mapLoader) {//MapLoader assignment operator
    this->loadedMaps = vector<Map*>(mapLoader.loadedMaps.size());
    for(Map* m : mapLoader.loadedMaps){
        this->loadedMaps.push_back(new Map(*m));
    }
    return *this;
}

ostream& operator<<(std::ostream& os, const MapLoader& mapLoader){ //MapLoader ostream insert overload
    return os << "MapLoader with " << mapLoader.loadedMaps.size() << " maps";
}

MapLoader::~MapLoader() { //MapLoader destructor
    for(Map* m : loadedMaps){
        delete m;
    }
}

Map* MapLoader::loadMap(string fileName){ //Load Map method, will return all maps even if not valid, if valid will be added to a list of created maps.

    Map* newMap = new Map();
    newMap->mapName = fileName;

    ifstream readMap(fileName);
    if(!readMap){
        cout << "File " << fileName << " not successfully opened" << endl;
        return nullptr;
    }
    string inputString;
    cout << "starting read for " << fileName << endl;
    while(getline(readMap, inputString)){

        if(inputString == "[countries]"){
            while(getline(readMap, inputString) && inputString != "[borders]"){

                if (inputString[0] == ';' || inputString.empty()) continue;
                int spaceOne = inputString.find(' ');
                int spaceTwo = inputString.substr(spaceOne+1).find(' ');
                int spaceThree = inputString.substr(spaceTwo+1).find(' ');


                Territory* tempTerritory = new Territory(inputString.substr(spaceOne+1,spaceTwo+1), stoi(inputString.substr(spaceOne+spaceTwo+2,spaceTwo+spaceThree+2)), stoi(inputString.substr(0, spaceOne))-1);
                newMap->territories.push_back(tempTerritory);


            }
            if(inputString == "[borders]"){

                while(getline(readMap, inputString)){

                    if (inputString[0] == ';' || inputString.empty()) continue;
                    vector<int> tempVector;
                    int spaceOne = inputString.find(' ');

                    inputString = inputString.substr(spaceOne+1);
                    while (!inputString.empty()){
                        int spaceOne = inputString.find(' ');
                        if(spaceOne == -1){
                            tempVector.push_back(stoi(inputString)-1);
                            break;
                        }

                        tempVector.push_back(stoi(inputString.substr(0,spaceOne+1))-1);
                        inputString = inputString.substr(spaceOne+1);

                    }
                    newMap->adjacencyList.push_back(tempVector);
                }
            }
        }
    }
    //Stores the collection of continents in the map data structure.
    int numberOfContinents = 0;
    for(Territory* t : newMap->territories){
        if(t->continentNumber > numberOfContinents) numberOfContinents = t->continentNumber;

    }
    vector<vector<int>> continentList(numberOfContinents, vector<int>(0));
    for(Territory* t : newMap->territories){

        continentList[t->continentNumber-1].push_back(t->countryNumber);
    }

    newMap->continentList = continentList;

    //Creates the edges for the territories from the adjacency list.
    for(Territory* t: newMap->territories){
        for(int i : newMap->adjacencyList[t->countryNumber]) {
            t->edges.push_back(newMap->territories[i]);
        }
    }
    //Adds map to list of loaded maps if it successfully validates.
    if(newMap->validate()){
        loadedMaps.push_back(newMap);
    }
    return newMap;
}


