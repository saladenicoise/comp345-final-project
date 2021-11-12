#include "Player.h"
#include <iostream>
#include <ostream> 
#include <string>

using namespace std;
Player::Player()
{
    //this->h = new Hand();
    this->olst = new OrderList();
} //default constructor 

Player::Player(string playerName)
{
    this->playerName = playerName;
    //this->h = new Hand();
    this->olst = new OrderList();
}

Player::Player(string playerName,int pid, vector<Territory*> t, Hand* h, OrderList* o, vector<Player*> noAttack) // parameterized constructor for Player class
{
    this->playerName = playerName;
    this->pid = pid;
    this->t = t;
    this->h = h;
    this->olst = o;
    this->cannotAttack = noAttack;
}

Player::Player(const Player& p) //copy constructor
{
    this->playerName = p.playerName;
    this->pid = p.pid;
    for (auto c : p.t)
        this->t.push_back(new Territory(*c));
    this->h = p.h;
    this->olst = p.olst;
    for (auto a : p.cannotAttack)
        this->cannotAttack.push_back(new Player(*a));
}

Player::~Player() //destructor 
{
    playerName.clear();
    if (!t.empty()) {
    for (auto c : t) {
        delete c;
        c = nullptr;
        }
    }
    t.clear();
    delete this->h;
    delete this->olst;
    this->h = nullptr;
    this->olst = nullptr;
    if(!cannotAttack.empty()) {
        for(auto a : cannotAttack) {
            delete a;
            a = nullptr;
        }
    }
    cannotAttack.clear();
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
    this->cannotAttack = p.cannotAttack;
    return *this;
}

std::ostream &operator<<(std::ostream& os,const Player& p) //overloads stream insertion
{
    return os << "Initializing Player" << ": "  << p.playerName<< "\n" << std::endl;
}

string Player::getPlayerName() //gets player's name 
{
    return playerName;
}

int Player::getPID() //gets player's id
{
    return pid;
}

void Player::setpID(int pid){
    this->pid = pid;
}

Hand *Player::getHand() //returns hand object
{
    return h;
}

OrderList *Player::getOrderList() //returns order list object
{
	return olst;
}

vector<Player*> Player::getNotAttackablePlayers() { // get not attackable players
    return cannotAttack;
}

void Player::setNotAttackablePlayers(vector<Player*> cannotAttack) { //set not attackable players
    this->cannotAttack = cannotAttack;
}

void Player::toAttack(vector<Territory*> toAttackTerritory, Player& p) // Territories to attack
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

void Player::toDefend(vector<Territory*> toDefendTerritory, Player& p) // Territories to defend
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

vector<Territory*> Player::getTerritories(){ // get players territories
	return t;
}

void Player::setTerritories(vector<Territory*> newTerritoryList) {
    t = newTerritoryList; //Update the territory list
}

void Player::issueOrder(const string& order) //creates an order object and adds it to the list of orders. 
{
    Order *newOrder = new Order(order);
    this->olst->addOrder(newOrder);
}

int Player::getReinforcementPool() { // gets the number of armies in popl
    return reinforcementPool;
}

void Player::setReinforcementPool(int pool) { // set pool size
    this->reinforcementPool = pool;
}