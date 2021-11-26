#ifndef Orders
#define Orders
#include <string>
#include <vector>
#include "../Map/Map.h"
#include "../Player/Player.h"
#include "../LoggingObserver/LoggingObserver.h"
using namespace std;

class Player;
class Territory;

class Order : public ILoggable, public Subject {
private:
    string orderType;

public:
    //Constructors
    Order();
    Order(string orderType);
    Order(const Order &order);
    //Destructor, virtual so that it can be overriden later on
    virtual ~Order();

    //Get function (virtual so it can be overriden later)
    virtual string getOrderType();

    //Stream Insertion Operator
    friend ostream &operator<<(ostream &stream, Order &order);
    //Assignment Operator (virtual so it can be overriden)
    virtual Order &operator=(const Order &order);
    //Execute Order Function (virtual so it can be overriden)
    virtual void execute();

    virtual std::string stringToLog();
    virtual bool validate();



};

class OrderList : public ILoggable, public Subject{
    //Our list of orders
private:
    //The actual container for the list
    vector<Order *> orderList;

public:
    //Constructors and Destructors
    OrderList();
    OrderList(vector<Order *> orderList);
    OrderList(const OrderList &orderList);
    ~OrderList();

    //Getter to access the private orderlist container
    vector<Order *> getOrderList();

    //Setter to change the orderList to a new one if need be
    void setOrderList(vector<Order *> orderList);

    //Adds an order to our list
    void addOrder(Order* order);
    //Deletes an order from our list
    void deleteOrder(int index);
    void deleteAllOrder();
    //Moves an order from one index to a new one
    void moveOrder(string orderType, int newIndex);

    bool hasOrder(string orderType);

    //Stream insertion operator
    friend ostream &operator<<(ostream &stream, OrderList &orderList);
    //Assignment operation
    OrderList &operator=(const OrderList &orderList);

    std::string stringToLog();
    Order* getIndex(int index);
    int getSize();

};

// List of necessary orders: deploy, advance, bomb, blockade, airlift, and negotiate. Must contain execute and validate function, nothing else is needed at this point (10/4/2021)

//Deploy Order
class Deploy : public Order {

private:
    string orderType;
    Player* sourcePlayer;
    Territory* targetTerritory;
    int numtoDeploy;


public:
    //Constructors
    Deploy();
    Deploy(Player *playerIn, Territory *territoryIn, int toDeployNum);
    Deploy(const Deploy &deploy);
    //Destructor
    ~Deploy();

    //Getter
    string getOrderType();

    Player* getPlayer();

    Territory* getTerritory();

    int getToDeploy();

    //Is our order valid
    bool validate();

    //Executes the order
    void execute();

    //Insertion Operator
    friend ostream &operator<<(ostream &stream, Deploy &deploy);

    //Assignment Operator
    Deploy &operator=(const Deploy &dep);

    std::string stringToLog();

};

class Advance : public Order {

private:
    string orderType;
    Player* sourcePlayer;
    Player* targetPlayer;
    Territory* attackSource;
    Territory* attackTarget;
    int numOfUnits;

public:
    //Constructors
    Advance();
    Advance(Player *attackerIn, Player *defenderIn, Territory *attackSourceIn, Territory *attackTarget, int numOfUnits);
    Advance(const Advance &advance);
    //Destructor
    ~Advance();

    //Getter
    string getOrderType();

    Player* getAttacker();

    Player* getDefender();

    Territory* getAttackSource();

    Territory* getAttackTarget();

    int getNumOfUnits();

    //Is our order valid
    bool validate();

    //Executes the order
    void execute();

    //Insertion Operator
    friend ostream &operator<<(ostream &stream, Advance &advance);

    //Assignment Operator
    Advance &operator=(const Advance &advance);

    std::string stringToLog();

};

class Bomb : public Order {

private:
    string orderType;
    Player* sourcePlayer;
    Territory* target;

public:
    //Constructors
    Bomb();
    Bomb(Player *playerIn, Territory* target);
    Bomb(const Bomb &bomb);
    //Destructor
    ~Bomb();

    //Getter
    string getOrderType();

    Player* getPlayer();

    Territory* getTarget();

    //Is our order valid
    bool validate();

    //Executes the order
    void execute();

    //Insertion Operator
    friend ostream &operator<<(ostream &stream, Bomb &bomb);

    //Assignment Operator
    Bomb &operator=(const Bomb &bomb);

    std::string stringToLog();

};

class Blockade : public Order {

private:
    string orderType;
    Player* sourcePlayer;
    Territory* target;
    Player* neutralPlayer;

public:
    //Constructors
    Blockade();
    Blockade(Player *playerIn, Territory *targetIn, Player *neutralPlayer);
    Blockade(const Blockade &blockade);
    //Destructor
    ~Blockade();

    //Getter
    string getOrderType();

    Player* getPlayer();

    Territory* getTarget();

    //Is our order valid
    bool validate();

    //Executes the order
    void execute();

    //Insertion Operator
    friend ostream &operator<<(ostream &stream, Blockade &blockade);

    //Assignment Operator
    Blockade &operator=(const Blockade &blockade);

    std::string stringToLog();

};

class Airlift : public Order {

private:
    string orderType;
    Player* sourcePlayer;
    Territory* source;
    Territory* dest;
    int numOfArmies;

public:
    //Constructors
    Airlift();
    Airlift(Player *playerIn, Territory *sourceIn, Territory* destIn, int numOfArmies);
    Airlift(const Airlift &airlift);
    //Destructor
    ~Airlift();

    //Getter
    string getOrderType();

    Player* getPlayer();

    Territory* getSource();

    Territory* getDest();

    int getNumOfArmies();

    //Is our order valid
    bool validate();

    //Executes the order
    void execute();

    //Insertion Operator
    friend ostream &operator<<(ostream &stream, Airlift &airlift);

    //Assignment Operator
    Airlift &operator=(const Airlift &airLift);

    std::string stringToLog();

};

class Negotiate : public Order {

private:
    string orderType;
    Player* sourcePlayer;
    Player* targetPlayer;

public:
    //Constructors
    Negotiate();
    Negotiate(Player* sourcePlayer, Player* targetPlayer);
    Negotiate(const Negotiate &negotiate);
    //Destructor
    ~Negotiate();

    //Getter
    string getOrderType();

    Player* getSourcePlayer();

    Player* getTargetPlayer();

    //Is our order valid
    bool validate();

    //Executes the order
    void execute();

    //Insertion Operator
    friend ostream &operator<<(ostream &stream, Negotiate &negotiate);

    //Assignment Operator
    Negotiate &operator=(const Negotiate &negotiate);

    std::string stringToLog();

};
#endif
