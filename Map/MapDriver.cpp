#include <iostream>;
#include <string>;
#include <set>;
#include "Map.h";
int main() {
    Map* test;

    MapLoader test2;
    //Note that validate is ran as part of the load map function. It is trivial to recall it but serves no purpose, the output of validate will be printed out during the load map call.
    test = test2.loadMap("bigeurope.map"); //Proper map example.
    test = test2.loadMap("fletzmap.map"); //Not all continents are fully connected sub-graphs
    test = test2.loadMap("austria.txt"); //Added a country to the file to make it not a fully connected graph.
    test = test2.loadMap("test.txt"); //invalid map file, returns a null pointer prior to calling validate.





    return 0;
}