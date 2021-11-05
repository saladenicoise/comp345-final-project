//
// Created by Ryan on 2021-09-24.
//

#ifndef C345_MAP_H
#define C345_MAP_H
using namespace std;
#include <vector>
#include <ostream>
#include "../Player/Player.h"
class Player;
class Territory;
class Map;
class MapLoader;

class Territory{
public:
    ~Territory();
    Territory(Territory& territory);
    Territory& operator= (const Territory& territory);
    Territory(string countryName, int continentNumber, int countryNumber);
    Territory(string countryName, int continentNumber, int countryNumber, int armyCount, Player* playerOccupying);
    friend ostream& operator<< (ostream& os, const Territory& Territory);
    string countryName; //Name of the territory
    vector<Territory*> edges; //Connected territorys to the territory.
    int continentNumber; //Continent number of the territory
    int countryNumber; //Country number of the territory
    int armyCount; //army count of the territory
    Player* playerOccupying; //Player occupying the territory.
};
class Map{
public:

    Map();
    ~Map();
    Map(Map& map);
    Map& operator= (const Map& map);
    string mapName; //Stores the file name of the map.
    std::vector<Territory*> territories; //List of territories in the map.
    std::vector<vector<int>> adjacencyList; //Adjacency List of the map.
    std::vector<vector<int>> continentList; //Continent list of the map.
    friend ostream& operator<< (ostream& os, const Map& map);
    bool validate();

};
class MapLoader{
public:
    ~MapLoader();
    MapLoader(MapLoader& mapLoader);
    MapLoader& operator= (const MapLoader& mapLoader);
    friend ostream& operator<< (ostream& os, const MapLoader& mapLoader);
    std::vector<Map*> loadedMaps;
    MapLoader();
    Map* loadMap(string fileName);
};


#endif //C345_MAP_H
