//This is the header file Player.h. 
//It contains all the class and operator declarations.
//Also provides dummy classes for Territory class, Card class and Order class.
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include<iostream>

using namespace std;

class Player;
class Territory;
class Cards;
class Order;

class Player //this is the Player class
{
    
    public:
        Player(); //default constructor
        Player(string playerName, int pid,Territory* t,Cards* c, vector<Order*>); //constructor  
        Player(const Player& p); //copy constructor
        ~Player(); //deconstructor 
        string getPlayerName();
        int getPID(); 
        void toAttack();
        void toDefend();
        void issueOrder(string);
        void printOrder();
        vector<Order*> getOrderList();

    private:
        string playerName;
        int pid;
        Territory* t;
        Cards* c; 
        vector<Order*> orderList;
};

class Territory //dummy class for Territory 
{
    public:
        Territory(); //default constructor 
        void setTerritory(string name);     //mutator method
        void getTerritory();    //receives territories 

    private:
        string name; 
        vector<string*> t;
};

class Cards //dummy class for Cards
{
    public:
        Cards(); //default constructor
        void setCard(string card); //mutator method for cards
        void hands();   //receives card hands

    private:
        string card;
        vector<string*> c;
};

class Order //dummy class for Order
{
    public:
        Order(); //default constructor
        Order(string order); //constructor
        string returnOrder();  //returns order

    private:
        string order;
};
#endif //PLAYER_H
