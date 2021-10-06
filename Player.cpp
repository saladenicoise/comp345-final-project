#include "Player.h"
using namespace std;

Player::Player() {} //default constructor 

Player::Player(string playerName,int pid, Territory* t, Cards* c, vector<Order*> o) //constructor for Player class
{
    this->playerName = playerName;
    this->pid = pid;
    this->t = t;
    this->c = c;
    this->orderList = o;
}

Player::Player(const Player& p) //copy constructor
{
    this->playerName = p.playerName;
    this->pid = p.pid;
    this->t = p.t;
    this->c = p.c;
    this->orderList = p.orderList;
}

Player::~Player() //destructor 
{
    playerName.clear();
    delete t;
    delete c;
    for (auto order: orderList)
    {
        delete order; 
    }
	orderList.clear();
}

string Player::getPlayerName() //gets player's name 
{
    return playerName;
}

int Player::getPID()    //gets player's id
{
    return pid;
}

void Player::toAttack() //returns a list of territories that are to be attacked from the territory class
{
    std::cout << "\nTerritories to attack:\n";
    t -> getTerritory();
}

void Player::toDefend() //returns a list of territories that are to be defended from the territory class
{
    std::cout << "\nTerritories to defend:\n";
    t -> getTerritory();
}

void Player::issueOrder(string order) //creates an order object and adds it to the list of orders. 
{
	Order *newOrder = new Order(order);
	orderList.push_back(newOrder);
}

vector<Order*> Player::getOrderList() //returns order list object
{
	return orderList;
}

void Player::printOrder() //prints order
{
   std::cout << "\nOrderlist:\n" ;
   vector<Order*>::iterator it = orderList.begin();
	for (; it != orderList.end(); it++)
	{
		cout << (*it)->returnOrder() << "\n";
	}
}

Territory::Territory() {}//constructor for Territory class 

void Territory::setTerritory(string name) //mutator method for territories
{
    t.push_back(&name);
}

void Territory::getTerritory() //returns a collection of territories
{
    for (int i = 0; i < t.size(); i++)
    {
        std::cout << *t[i] << "\n";
    }
}

Cards::Cards() {} //constructor for Cards class 

void Cards::setCard(string name) //mutator method for Cards 
{
    c.push_back(&name);
}

void Cards::hands() //returns a player's hands
{
    for (int i = 0; i < c.size(); i++)
    {
        std::cout << *c[i] << "\n";
    }
}

Order::Order() {} //default constructor for Order class 

Order::Order(string o) //constructor for Order class 
{
    this->order = o;
}

string Order::returnOrder() //returns a player's orders
{
    return order;
}