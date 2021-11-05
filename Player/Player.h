//This is the header file Player.h. 
//It contains all the class and operator declarations.
//Also provides dummy classes for Territory class, Card class and Order class.
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <iostream>
#include <ostream> 
#include "../Cards/Cards.h"
#include "../Orders/Orders.h"
#include "../Map/Map.h"

class Player;
class Territory;
class Hand;
class OrderList;
class Order;

class Player //this is the Player class
{
    
    public:
        Player(); //default constructor
        Player(string playerName, int pid, Territory * t, Hand* h, OrderList* olst); //constructor  
        Player(const Player& p); //copy constructor
        ~Player(); //deconstructor 
        string getPlayerName();
        int getPID(); 
        void toAttack(vector<Territory*> toAttackTerritory, Player& p);
        void toDefend(vector<Territory*> toDefendTerritory, Player& p);
        void issueOrder(const string& order);
        void printOrder();
        Player& operator=(const Player& p); //overloaded assignment operator
        friend std::ostream& operator<<(std::ostream &os,const Player& p); //stream insertion operator
        Hand* getHand();
        OrderList* getOrderList();
        Territory* getTerritories();
    private:
        string playerName;
        int pid;
        Territory * t;
        Hand* h; 
        OrderList* olst;
};

#endif //PLAYER_H
