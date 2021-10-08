#include <iostream>;
#include <string>;
#include <set>;
#include "Map.h";
int main() {
    Map* test;

    MapLoader test2;
    test = test2.loadMap("bigeurope.map"); //Proper map example.
    test = test2.loadMap("fletzmap.map"); //Not all continents are fully connected sub-graphs
    test = test2.loadMap("austria.txt"); //Added a country to the file to make it not a fully connected graph.






    return 0;
}