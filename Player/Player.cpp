#include "Player.h"
#include <iostream>
#include <ostream> 
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
Player::Player()
{
    this->h = new Hand();
    this->olst = new OrderList();
    this->isNeutral = 1;
} //default constructor 

Player::Player(string playerName, int isNeutral) {
    this->playerName = playerName;
    this->h = new Hand();
    this->olst = new OrderList();
    this->isNeutral = isNeutral;
}

Player::Player(string playerName)
{
    this->playerName = playerName;
    this->h = new Hand();
    this->olst = new OrderList();
    this->isNeutral = 1;
    
}

Player::Player(string playerName,int pid, vector<Territory*> t, Hand* h, OrderList* o, vector<Player*> noAttack, int isNeutral) // parameterized constructor for Player class
{
    this->playerName = playerName;
    this->pid = pid;
    this->t = t;
    this->h = h;
    this->olst = o;
    this->cannotAttack = noAttack;
    this->isNeutral = isNeutral;
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
    this->strategyString = p.strategyString;
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
    this->strategyString = p.strategyString;
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

vector<Territory*> Player::getTerritories(){ // get players territories
	return t;
}

string Player::getPlayerStrategyString() {
    return this->strategyString;
}

void Player::setTerritories(vector<Territory*> newTerritoryList) {
    t = newTerritoryList; //Update the territory list
}

vector<Territory*> Player::getDefendList(){
	return defendList;
}
void Player::setDefendList(vector<Territory*> newDefendList){
	defendList = newDefendList;
}
void Player::setAttackList(vector<Territory*> newAttackList){
	attackList = newAttackList;
}


vector<Territory*> Player::getAttackList(){
	return attackList;
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

void Player::setpID(int pID) { //sets players id
    this->pid = pID;
}

int Player::getNeutral() {
    return this->isNeutral;
}

int Player::getGetCard() {
    return this->getCard;
}

void Player::setGetCard(int getCard) {
    this->getCard = getCard;
}

vector<Territory*> Player::getNeighbour(vector<Territory*> Map) //finds the neighbouring teritories of a player
{  
    vector<Territory*> neighbouring_terrritories; //for each territory, find the neighbours of the territory
    for (int i = 0; i<t.size(); i++)
    {   
        for(int j = 0; j < t[i]->edges.size(); j++){
            bool contains = false;
            for(int k = 0; k< neighbouring_terrritories.size();k++){
                if(std::find(neighbouring_terrritories.begin(), neighbouring_terrritories.end(), t[i]->edges[j]) != neighbouring_terrritories.end()){ // if in neighbour vector
                    contains = true;
                } else {
                    neighbouring_terrritories.push_back(t[i]->edges[j]);
                    contains = true;
                }
            }
            if(!contains){
                neighbouring_terrritories.push_back(t[i]->edges[j]);
            }
        }
    }

    for(int i = 0; i < getDefendList().size(); i++) // remove everything from defend list in neighbour list
    {
        auto iter = std::find(neighbouring_terrritories.begin(),neighbouring_terrritories.end(),getDefendList()[i]);
        if(iter != neighbouring_terrritories.end())
        {
            neighbouring_terrritories.erase(iter);
        }
    }
    return neighbouring_terrritories;   //returns a player's list of territories that can be attacked
}

vector<Territory*> Player::toAttack(vector<Territory*> Map, Player &p)
{
    return this->strategy->toAttack(Map,p);
}

vector<Territory*> Player::toDefend(vector<Territory*> Map, Player& p)
{
    return this->strategy->toDefend(Map,p);
}

void Player::issueOrderObject(Order* order) //creates an order object and adds it to the list of orders. 
{
    // Order *newOrder = new Order();
    this->olst->addOrder(order);
}

void Player::issuingOrder(Player* p, GameEngine *game, Deck* deck)
{
    //cout << "Issuing Order for " << p->getPID() << endl;
    return this->strategy->issueOrder(p,game, deck);
}

void Player::setStrategy(PlayerStrategy *newStrategy) {
    this->strategy = newStrategy;
}

void Player::setStrategyString(string strategyString) {
    this->strategyString = strategyString;
}

PlayerStrategy* Player::getPlayerStrategy() {
    return strategy;
}
