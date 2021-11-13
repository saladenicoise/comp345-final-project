#include "Orders.h"
#include "../Map/Map.h"
using namespace std;
#include <iostream>
#include <cmath> //Necessary to round
#include <algorithm>

class Player;
class Map;
class Territory;


//Order Class
Order::Order() : ILoggable(), Subject() {
    this->orderType = "order";
}

Order::Order(string orderType) {
    this->orderType = orderType;
}

//Copy Constructor
Order::Order(const Order &order) {
    this->orderType = order.orderType;
}

//Destructor
Order::~Order() {
}

std::string Order::stringToLog() { //shouldn't occur but necessary since order can't be abstract
    return this->getOrderType();
}

string Order::getOrderType() {
    return orderType;
}

void Order::execute() {
    cout << "Order is valid, now executing" << endl;
}

ostream &operator<<(ostream &stream, Order &order) {
    return stream << "Order Type: " << order.orderType;
}

//Assignment Operator, we have to return a pointer to the newly copied over object
Order &Order::operator=(const Order &order) {
    this->orderType = order.orderType;
    return *this;
}

bool Order::validate(){
    return true;
}

//OrderList Class, nothing speciall is needed here and we dont have a default value
OrderList::OrderList() {}

OrderList::OrderList(vector<Order *> orderList) {
    this->orderList = orderList;
}

//Copy constructor
OrderList::OrderList(const OrderList &orderList) {
    //Since we have a vector of different pointers, we have to copy over each pointer
    for (int i = 0; i < orderList.orderList.size(); i++) {
        //We have to check for each individual order type so that we dont mess up which order is being pointed at
        if (orderList.orderList[i]->getOrderType() == "deploy") {
            this->orderList.push_back(new Deploy());
        }else if (orderList.orderList[i]->getOrderType() == "advance") {
            this->orderList.push_back(new Advance());
        }else if (orderList.orderList[i]->getOrderType() == "bomb") {
            this->orderList.push_back(new Bomb());
        }else if (orderList.orderList[i]->getOrderType() == "blockade") {
            this->orderList.push_back(new Blockade());
        }else if (orderList.orderList[i]->getOrderType() == "airlift") {
            this->orderList.push_back(new Airlift());
        }else if (orderList.orderList[i]->getOrderType() == "negotiate") {
            this->orderList.push_back(new Negotiate());
        }
    }
}

//Destructor
OrderList::~OrderList() {
    //Since we have a vector of pointers, we cannot simply delete the vectors, otherwise it would cause a memory leak. Thus we have to iterate through each and delete it whilst setting its pointer to null
    for (Order *order : orderList) {
        delete order;
        order = nullptr;
    }
}

//Getter
vector<Order *> OrderList::getOrderList() {
    return orderList;
}

//Setter
void OrderList::setOrderList(vector<Order *> orderList) {
    this->orderList = orderList;
}

//Add Order
void OrderList::addOrder(Order *order) {
    orderList.push_back(order);
    notify(this);
}

std::string OrderList::stringToLog() {
    return orderList.back()->getOrderType() + " order added to order list.";
}



//Remove Order
void OrderList::deleteOrder(int index) {
    int i = 0;
    for (Order *order : orderList)
    {
        if (i == index)
        {
            delete order;
            order = nullptr;
        }
        i++;
    }
    orderList.erase(orderList.begin() + index);
}

//Do we have this order present
bool OrderList::hasOrder(string orderType) {
    for (Order *order : orderList) {
        if(order->getOrderType() == orderType) {
            return true;
        }
    }
    return false;
}

void OrderList::moveOrder(string orderType, int newIndex) {
    if (newIndex > orderList.size()) {
        cout << "Error: Cannot use an index that is greater than the size of our orders" << endl;
        return;
    }
    if (!hasOrder(orderType)) {
        cout << "Error: Order has been deleted, cannot move!" << endl;
        return;
    }
    Order *temp;
    int i = 0, tempIndex = 0;
    //Find the index of the order which has orderType == orderType
    for (Order *order : orderList) {
        if (order->getOrderType() == orderType) {
            //Set our temp node to the value of the node that is trying to be moved
            temp = order;
            //Lock in our temporary index, will represent the position to switch
            tempIndex = i;
        }
        i++;
    }
    //Swap the elements in the list
    //Set the old index to be the order at newIndex
    orderList[tempIndex] = orderList[newIndex];
    //Set the value of the order which was copied to tempIndex to the temp order that we saved prior
    orderList[newIndex] = temp;
}

ostream &operator<<(ostream &stream, OrderList &orderList) {
    for(int i = 0; i < orderList.orderList.size(); i++) {
        stream << "Order found at index " + to_string(i) << " is of type: " + orderList.orderList[i]->getOrderType() << endl;
    }
    return stream;
}

//Assignment Operator
OrderList &OrderList::operator=(const OrderList &orderList) {
    for (int i = 0; i < orderList.orderList.size(); i++) {
        //We have to check for each individual order type so that we dont mess up which order is being pointed at
        if (orderList.orderList[i]->getOrderType() == "deploy") {
            this->orderList.push_back(new Deploy());
        }else if (orderList.orderList[i]->getOrderType() == "advance") {
            this->orderList.push_back(new Advance());
        }else if (orderList.orderList[i]->getOrderType() == "bomb") {
            this->orderList.push_back(new Bomb());
        }else if (orderList.orderList[i]->getOrderType() == "blockade") {
            this->orderList.push_back(new Blockade());
        }else if (orderList.orderList[i]->getOrderType() == "airlift") {
            this->orderList.push_back(new Airlift());
        }else if (orderList.orderList[i]->getOrderType() == "negotiate") {
            this->orderList.push_back(new Negotiate());
        }
    }

    return *this;
}

//Deploy Class
Deploy::Deploy() {
    this->orderType = "deploy";
    this->targetTerritory = nullptr;
    this->sourcePlayer = nullptr;
    this->numtoDeploy = 0;
}

Deploy::Deploy(Player* player, Territory* source, int numToDeploy) {
    this->orderType = "deploy";
    this->targetTerritory = source;
    this->sourcePlayer = player;
    this->numtoDeploy = numToDeploy;
}

//Copy constructor
Deploy::Deploy(const Deploy &deploy) {
    this->orderType = deploy.orderType;
    this->sourcePlayer = deploy.sourcePlayer;
    this->targetTerritory = deploy.targetTerritory;
    this->numtoDeploy = deploy.numtoDeploy;
}
//Destructor
Deploy::~Deploy() {

}

//Getter
string Deploy::getOrderType() {
    return orderType;
}

Player *Deploy::getPlayer() {
    return sourcePlayer;
}

Territory *Deploy::getTerritory() {
    return targetTerritory;
}

int Deploy::getToDeploy() {
    return numtoDeploy;
}

bool Deploy::validate() {
    //Iterate through all terittories owned by the player
    if (targetTerritory == nullptr) {
        return false;
    }
        //Does the current territory match that of the target territory?
        //Is the number to deploy > 0?
        //Is the number to deploy <= to the reinforcement pool?
     if (std::find(sourcePlayer->getTerritories().begin(), sourcePlayer->getTerritories().end(), targetTerritory) != sourcePlayer->getTerritories().end())
    {
        if ((numtoDeploy > 0) && (numtoDeploy <= sourcePlayer->getReinforcementPool()))
        {
        return true;
        }
    }
    return false;
}

std::string Deploy::stringToLog() {
    return to_string(numtoDeploy) + " troops moved to " + targetTerritory->countryName + " by player " + sourcePlayer->getPlayerName();
}

void Deploy::execute() {
    if(validate()) {
        //Set the number of troops in the territory to the current plus the new number to deploy
        targetTerritory->armyCount += numtoDeploy;
        sourcePlayer->setReinforcementPool(sourcePlayer->getReinforcementPool()-numtoDeploy);
        notify(this);
        cout << "Deploy Order Executed" << endl;
    }else{
        cout << "Invalid Deploy Order" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Deploy &deploy) {
    return strm << "Order Type: " << deploy.getOrderType();
}

//Assignment Operator
Deploy &Deploy::operator=(const Deploy &deploy) {
    this->orderType = deploy.orderType;
    this->sourcePlayer = deploy.sourcePlayer;
    this->targetTerritory = deploy.targetTerritory;
    this->numtoDeploy = deploy.numtoDeploy;
    return *this;
}

//Advance Class
Advance::Advance() {
    this->orderType = "advance";
    this->sourcePlayer = nullptr;
    this->targetPlayer = nullptr;
    this->attackSource = nullptr;
    this->attackTarget = nullptr;
    this->numOfUnits = 0;
}

Advance::Advance(Player *sourceP, Player *targetP, Territory *sourceT, Territory *targetT, int numOfUnits) {
    this->orderType = "advance";
    this->sourcePlayer = sourceP;
    this->targetPlayer = targetP;
    this->attackSource = sourceT;
    this->attackTarget = targetT;
    this->numOfUnits = numOfUnits;
}

//Copy constructor
Advance::Advance(const Advance &advance) {
    this->orderType = advance.orderType;
    this->sourcePlayer = advance.sourcePlayer;
    this->targetPlayer = advance.targetPlayer;
    this->attackSource = advance.attackSource;
    this->attackTarget = advance.attackTarget;
    this->numOfUnits = advance.numOfUnits;
}

//Destructor
Advance::~Advance() {

}

//Getter
string Advance::getOrderType() {
    return orderType;
}

Player* Advance::getAttacker() {
    return sourcePlayer;
}

Player* Advance::getDefender() {
    return targetPlayer;
}

Territory* Advance::getAttackSource() {
    return attackSource;
}

Territory* Advance::getAttackTarget() {
    return attackTarget;
}

int Advance::getNumOfUnits() {
    return numOfUnits;
}

//Always return true, since we havent implemented the order functionality yet
bool Advance::validate() {
    if(attackSource == nullptr || attackTarget == nullptr) {
        return false;
    }
    bool attackable = true;
    if(sourcePlayer->getNotAttackablePlayers().size() > 0) {
        for (int i = 0; i < sourcePlayer->getNotAttackablePlayers().size(); i++) {
            if (sourcePlayer->getNotAttackablePlayers()[i] == targetPlayer) {
                attackable = false;
                break;
            }
        }
    }
    Territory* sourceTerritory;
    Territory* destTerritory;
    bool adjacent = false;
    //Get attacking and defending territories
    for(int i = 0; i < sourcePlayer->getTerritories().size(); i++) {
        sourceTerritory = 0;
        if(sourcePlayer->getTerritories()[i] == attackSource) {
            sourceTerritory = attackSource;
            break;
        }
    }
    for(int i = 0; i < targetPlayer->getTerritories().size(); i++) {
        destTerritory = 0;
        if(targetPlayer->getTerritories()[i] == attackTarget) {
            destTerritory = attackTarget;
            break;
        }
    }
    //Not the same territory
    if(sourceTerritory == destTerritory) {
        return false;
    }
    //Make sure territories are adjacent
    for(int i = 0; i < sourceTerritory->edges.size(); i++) {
        if(sourceTerritory->edges[i] == destTerritory) {
            adjacent = true;
            break;
        }
    }
    //Are the source and destination not null AND is the number of units less than or = to armies in territory AND are the territories adjacent
    if((sourceTerritory != nullptr || destTerritory != nullptr) && (numOfUnits <= sourceTerritory->armyCount) && adjacent && attackable) {
        return true;
    }else{
        return false;
    }
}

std::string Advance::stringToLog() {
    return getAttackTarget()->countryName + " is now owned by Player " + to_string(getAttackTarget()->getPlayerIDOccupying()) + " and has an army count of " + to_string(getAttackTarget()->armyCount) + " \n" + attackSource->countryName + " now has an army count of " + to_string(attackSource->armyCount);
}
void Advance::execute() {
    if(validate()) {
        cout << "Advance Order Executed" << endl;
        //By default its an attack order
        bool attackOrder = true;
        for(int i = 0; i < sourcePlayer->getTerritories().size(); i++) {
            //Is the target of the advance in the source's territory list?
            if(sourcePlayer->getTerritories()[i] == attackTarget) {
                attackOrder = false; //This is not an attack order, but a march order
                break;
            }
        }
        if(attackOrder && sourcePlayer != targetPlayer) {
            cout << "Now commencing attack" << endl;
            //70% chance of an attacker dying
            int attackersKilled = round(static_cast<double>(attackTarget->armyCount * 0.7));
            //60% chance of a defender dying
            int defendersKilled = round(static_cast<double>(numOfUnits * 0.6));
            int survivedAttackers = numOfUnits - attackersKilled;
            int survivedDefenders = attackTarget->armyCount - defendersKilled;
            //All defending units are dead, and at least one attacking unit is left alive
            if(survivedAttackers > 0 && survivedDefenders == 0) {
                cout << "Attack Status: WON | Territory Conquered!" << endl;
                cout << "---------" << endl;
                cout << "Defending Casualties: " << defendersKilled << endl;
                cout << "Attacking Casualties: " << attackersKilled << endl;
                cout << "---------" << endl;

                //Move Units
                attackSource->armyCount = attackSource->armyCount - numOfUnits;
                attackTarget->armyCount = numOfUnits - attackersKilled;

                //Change Ownership
                attackTarget->setOwnerId(sourcePlayer->getPID());

                //Remove from loser's list
                vector<Territory *> loserTer = targetPlayer->getTerritories();
                vector<Territory *> newTer;
                for(int i = 0; i < loserTer.size(); i++) {
                    if(loserTer[i] != attackTarget) {
                        newTer.push_back(loserTer[i]);
                    }
                }
                targetPlayer->setTerritories(newTer);

                //Add to winner's list
                vector<Territory *> winnerTer = sourcePlayer->getTerritories();
                winnerTer.push_back(attackTarget);
                sourcePlayer->setTerritories(winnerTer);
                sourcePlayer->setGetCard(1);
            }else{ //Attacker lost
                cout << "Attack Status: LOST | Territory Defended!" << endl;
                cout << "---------" << endl;
                cout << "Defending Casualties: " << defendersKilled << endl;
                cout << "Attacking Casualties: " << attackersKilled << endl;
                cout << "---------" << endl;

                //Update army counts
                attackTarget->armyCount -= defendersKilled;
                attackSource->armyCount -= attackersKilled;

            }
        }
        if((sourcePlayer == targetPlayer) && !attackOrder) {
            cout << "Troops have been moved" << endl;
            attackSource->armyCount -= numOfUnits;
            attackTarget->armyCount += numOfUnits;
        }
    }else{
        cout << "Invalid Advance Order" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Advance &advance) {
    return strm << "Order Type: " << advance.getOrderType();
}

//Assignment Operator
Advance &Advance::operator=(const Advance &advance) {
    this->orderType = advance.orderType;
    this->sourcePlayer = advance.sourcePlayer;
    this->targetPlayer = advance.targetPlayer;
    this->attackSource = advance.attackSource;
    this->attackTarget = advance.attackTarget;
    this->numOfUnits = advance.numOfUnits;
    return *this;
}

//Bomb Class
Bomb::Bomb() {
    this->orderType = "bomb";
    this->sourcePlayer = nullptr;
    this->target = nullptr;
}

Bomb::Bomb(Player *bombingPlayer, Territory *bombingTarget) {
    this->orderType = "bomb";
    this->sourcePlayer = bombingPlayer;
    this->target = bombingTarget;
}

//Copy constructor
Bomb::Bomb(const Bomb &bomb) {
    this->orderType = bomb.orderType;
    this->sourcePlayer = bomb.sourcePlayer;
    this->target = bomb.target;
}

//Destructor
Bomb::~Bomb() {

}

//Getter
string Bomb::getOrderType() {
    return orderType;
}

Player* Bomb::getPlayer() {
    return sourcePlayer;
}

Territory* Bomb::getTarget() {
    return target;
}

bool Bomb::validate() {
    Territory* targAddress = 0;
    for(int i = 0; i < sourcePlayer->getTerritories().size(); i++){
        targAddress = 0;
        if((sourcePlayer->getTerritories()[i] == target) && (target != nullptr)){
            targAddress = sourcePlayer->getTerritories()[i];
            break;
        }
    }

    //check that the player issuing the order is not targeting their own territory
    if (targAddress != target) {
        return true;
    }
    else {
        return false;
    }
}


std::string Bomb::stringToLog() {
    return getTarget()->countryName + " has been bombed lowering its army count to " + to_string(getTarget()->armyCount);
}
void Bomb::execute() {
    if(validate()) {
        target->armyCount /= 2;
        cout << "Bomb Order Executed" << endl;
        notify(this);
    }else{
        cout << "Invalid Bomb Order" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Bomb &bomb) {
    return strm << "Order Type: " << bomb.getOrderType();
}

//Assignment Operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    this->orderType = bomb.orderType;
    this->sourcePlayer = bomb.sourcePlayer;
    this->target = bomb.target;
    return *this;
}

//Blockade Class
Blockade::Blockade() {
    this->orderType = "blockade";
    this->sourcePlayer = nullptr;
    this->target = nullptr;
}

Blockade::Blockade(Player *sourcePlayer, Territory *target, Player* neutralPlayer) {
    this->orderType = "blockade";
    this->sourcePlayer = sourcePlayer;
    this->target = target;
    this->neutralPlayer = neutralPlayer;
}

//Copy constructor
Blockade::Blockade(const Blockade &blockade) {
    this->orderType = blockade.orderType;
    this->sourcePlayer = blockade.sourcePlayer;
    this->target = blockade.target;
}

Blockade::~Blockade() {

}

//Getter
string Blockade::getOrderType() {
    return orderType;
}

Player* Blockade::getPlayer() {
    return sourcePlayer;
}

Territory* Blockade::getTarget() {
    return target;
}

bool Blockade::validate() {
    for(int i = 0; i < sourcePlayer->getTerritories().size(); i++) {
        if(sourcePlayer->getTerritories()[i] == target) {//Is it part of our territories?
            return true;
        }
    }
    return false; //Not part of our territories
}
std::string Blockade::stringToLog() {
    return "Player " + to_string(target->getPlayerIDOccupying()) + " has lost ownership of "  + target->countryName + " to the neutral player, its army count has doubled to " + to_string(target->armyCount);
}

void Blockade::execute() {
    if (validate()) {
        cout << "Blockade Order Executed" << endl;
        target->armyCount *= 2;

        //Change ownership to neutral player
        target->setOwnerId(0);

        //Remove from player's list
        vector<Territory*> sourceOwnedTer = sourcePlayer->getTerritories();
        for(int i = 0; sourceOwnedTer.size(); i++) {
            if (sourceOwnedTer[i] == target) {
                sourceOwnedTer.erase(sourceOwnedTer.begin()+i);//Iterator that starts at the beginning and goes to the proper index to delete territory
                break;
            }
        }
        sourcePlayer->setTerritories(sourceOwnedTer);
        vector<Territory*> neutralOwnerTer = neutralPlayer->getTerritories();
        neutralOwnerTer.push_back(target); //Add to neutral player
        neutralPlayer->setTerritories(neutralOwnerTer);
        notify(this);
    }else{
        cout << "Invalid Blockade Order" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Blockade &blockade) {
    return strm << "Order Type: " << blockade.getOrderType();
}


//Assignment Operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    this->orderType = blockade.orderType;
    this->sourcePlayer = blockade.sourcePlayer;
    this->target = blockade.target;
    return *this;
}

//Airlift Class
Airlift::Airlift() {
    this->orderType = "airlift";
    this->sourcePlayer = nullptr;
    this->source = nullptr;
    this->dest = nullptr;
    this->numOfArmies = 0;
}

Airlift::Airlift(Player* sourcePlayer, Territory* source, Territory* destination, int numOfArmies) {
    this->orderType = "airlift";
    this->sourcePlayer = sourcePlayer;
    this->source = source;
    this->dest = destination;
    this->numOfArmies = numOfArmies;
}

//Copy constructor
Airlift::Airlift(const Airlift &airlift) {
    this->orderType = airlift.orderType;
    this->sourcePlayer = airlift.sourcePlayer;
    this->source = airlift.source;
    this->dest = airlift.dest;
    this->numOfArmies = airlift.numOfArmies;
}

Airlift::~Airlift() {

}

//Getter
string Airlift::getOrderType() {
    return orderType;
}

Player* Airlift::getPlayer() {
    return sourcePlayer;
}

Territory* Airlift::getSource() {
    return source;
}

Territory* Airlift::getDest() {
    return dest;
}

int Airlift::getNumOfArmies() {
    return numOfArmies;
}

//Always return true, since we havent implemented the order functionality yet
bool Airlift::validate() {
    if(!(source || dest)) {
        return false;
    }
    bool hasSource = false, hasDest = false;
    for(int i = 0; i < sourcePlayer->getTerritories().size(); i++) {
        if(sourcePlayer->getTerritories()[i] == source) {
            hasSource = true;
        }
        if(sourcePlayer->getTerritories()[i] == dest) {
            hasDest = true;
        }
    }
    if(hasSource && hasDest && (numOfArmies <= source->armyCount)) {//Does the source and dest both belong to the player, and does the number of troops moving <= to the number of troops in the territory
        return true;
    }else{
        return false;
    }
}

std::string Airlift::stringToLog() {
    return to_string(numOfArmies) + " moved from " + source->countryName + " to " + dest->countryName;
}

void Airlift::execute() {
    if (validate()) {
        cout << "Airlift Order Executed" << endl;
        //Move
        source->armyCount -= numOfArmies;
        dest->armyCount += numOfArmies;
        notify(this);
    }else{
        cout << "Invalid Airlift Order" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Airlift &airlift) {
    return strm << "Order Type: " << airlift.getOrderType();
}


//Assignment Operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    this->orderType = airlift.orderType;
    this->sourcePlayer = airlift.sourcePlayer;
    this->source = airlift.source;
    this->dest = airlift.dest;
    this->numOfArmies = airlift.numOfArmies;
    return *this;
}

//Negotiate Class
Negotiate::Negotiate() {
    this->orderType = "negotiate";
    this->sourcePlayer = nullptr;
    this->targetPlayer = nullptr;
}

Negotiate::Negotiate(Player* sourcePlayer, Player* targetPlayer) {
    this->orderType = "negotiate";
    this->sourcePlayer = sourcePlayer;
    this->targetPlayer = targetPlayer;
}

//Copy constructor
Negotiate::Negotiate(const Negotiate &negotiate) {
    this->orderType = negotiate.orderType;
    this->sourcePlayer = negotiate.sourcePlayer;
    this->targetPlayer = negotiate.targetPlayer;
}

Negotiate::~Negotiate() {

}

//Getter
string Negotiate::getOrderType() {
    return orderType;
}

Player* Negotiate::getSourcePlayer() {
    return sourcePlayer;
}

Player* Negotiate::getTargetPlayer() {
    return targetPlayer;
}

//Cannot negotiate with yourself
bool Negotiate::validate() {
    if(sourcePlayer != targetPlayer)
        return true;
    return false;
}

void Negotiate::execute() {
    if (validate()) {
        notify(this);
        cout << "Negotiate Order Executed" << endl;
        //Players get added to each other's do not attack list
        vector<Player*> noAttackP;
        for(Player* player : sourcePlayer->getNotAttackablePlayers()) {
            noAttackP.push_back(player);
        }
        noAttackP.push_back(targetPlayer);
        sourcePlayer->setNotAttackablePlayers(noAttackP);

        vector<Player*> noAttackP2;
        for(Player* player : targetPlayer->getNotAttackablePlayers()) {
            noAttackP2.push_back(player);
        }
        noAttackP2.push_back(sourcePlayer);
        targetPlayer->setNotAttackablePlayers(noAttackP2);
    }else{
        cout << "Invalid Negotiate Order" << endl;
    }
}

std::string Negotiate::stringToLog() {
    return this->getSourcePlayer()->getPlayerName() + " has negotiated with " + this->getTargetPlayer()->getPlayerName();
}
//Stream insertion operator overload
ostream &operator<<(ostream &strm, Negotiate &negotiate) {
    return strm << "Order Type: " << negotiate.getOrderType();
}


//Assignment Operator
Negotiate &Negotiate::operator=(const Negotiate &negotiate) {
    this->orderType = negotiate.orderType;
    this->sourcePlayer = negotiate.sourcePlayer;
    this->targetPlayer = negotiate.targetPlayer;
    return *this;
}

Order* OrderList::getIndex(int index)
{
    return orderList[index];
}

int OrderList::getSize()
{
    return orderList.size();
}
