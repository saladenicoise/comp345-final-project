#include "Player.h"
#include <iostream>
#include <ostream> 
#include <string>

using namespace std;
Player::Player()
{
      this->h = new Hand();
      this->olst = new OrderList();
} //default constructor 

Player::Player(string playerName,int pid, Territory * t, Hand* h, OrderList* o) //constructor for Player class
{
    this->playerName = playerName;
    this->pid = pid;
    this->t = t;
    this->h = h;
    this->olst = o;
}

Player::Player(const Player& p) //copy constructor
{
    this->playerName = p.playerName;
    this->pid = p.pid;
    this->t = p.t;
    this->h = p.h;
    this->olst = p.olst;
}

Player::~Player() //destructor 
{
    playerName.clear();
    delete this->t;
    delete this->h;
    delete this->olst;
    this->h = nullptr;
    this->olst = nullptr;
}

Player &Player::operator=(const Player &p) //assignment operator overloader to create deep copy 
{
    if(this == &p)
    {
        // Self-assignment guard
        return *this;
    }
    this->playerName = p.playerName;
    this->pid = p.pid;
    this->t = p.t;
    this->h = p.h;
    this->olst = p.olst;
    return *this;
}

std::ostream &operator<<(std::ostream& os,const Player& p) //overloads stream insertion
{
    return os << "Initializing Player" << p.pid << ": "  << p.playerName<< "\n" <<std::endl;
}

string Player::getPlayerName() //gets player's name 
{
    return playerName;
}

int Player::getPID()    //gets player's id
{
    return pid;
}

Hand *Player::getHand() //returns hand object
{
    return h;
}

OrderList *Player::getOrderList() //returns order list object
{
	return olst;
}

void Player::toAttack(vector<Territory*> toAttackTerritory, Player& p) 
{
    std::string str ("Attack: ");

	cout << "The list of territories that are be attacked" << endl;
	for (int i = 0; i < toAttackTerritory.size(); i++)
	{
		cout << *toAttackTerritory[i] << endl;
        p.issueOrder(str.append(toAttackTerritory[i]->countryName));
	}

	cout << endl;
}

void Player::toDefend(vector<Territory*> toDefendTerritory, Player& p) 
{
    std::string str ("Defend: ");
    
	cout << "The list of territories that are be defended" << endl;
	for (int i = 0; i < toDefendTerritory.size(); i++)
	{
		cout << *toDefendTerritory[i] << endl;
        p.issueOrder(str.append(toDefendTerritory[i]->countryName));
	}
	cout << endl;
}

Territory* Player::getTerritories(){
	return t;
}

void Player::issueOrder(const string& order) //creates an order object and adds it to the list of orders. 
{
    Order *newOrder = new Order(order);
    this->olst->addOrder(newOrder);
}

