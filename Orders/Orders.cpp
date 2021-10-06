#include "Orders.h"
using namespace std;
#include <iostream>

//Order Class
Order::Order() {
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
}

//Copy constructor
Deploy::Deploy(const Deploy &deploy) {
    this->orderType = deploy.orderType;
}
//Destructor
Deploy::~Deploy() {
    
}

//Getter
string Deploy::getOrderType() {
    return orderType;
}

//Always return true, since we havent implemented the order functionality yet
bool Deploy::validate() {
    return true;
}

void Deploy::execute() {
    if (Deploy::validate()) {
        cout << "Order is valid, now executing" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Deploy &deploy) {
    return strm << "Order Type: " << deploy.getOrderType();
}

//Assignment Operator
Deploy &Deploy::operator=(const Deploy &deploy) {
    this->orderType = deploy.orderType;
    return *this;
}

//Advance Class
Advance::Advance() {
    this->orderType = "advance";
}

//Copy constructor
Advance::Advance(const Advance &advance) {
    this->orderType = advance.orderType;
}
//Destructor
Advance::~Advance() {
    
}

//Getter
string Advance::getOrderType() {
    return orderType;
}

//Always return true, since we havent implemented the order functionality yet
bool Advance::validate() {
    return true;
}

void Advance::execute() {
    if (Advance::validate()) {
        cout << "Order is valid, now executing" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Advance &advance) {
    return strm << "Order Type: " << advance.getOrderType();
}

//Assignment Operator
Advance &Advance::operator=(const Advance &advance) {
    this->orderType = advance.orderType;
    return *this;
}

//Bomb Class
Bomb::Bomb() {
    this->orderType = "bomb";
}

//Copy constructor
Bomb::Bomb(const Bomb &bomb) {
    this->orderType = bomb.orderType;
}

//Destructor
Bomb::~Bomb() {
    
}

//Getter
string Bomb::getOrderType() {
    return orderType;
}

//Always return true, since we havent implemented the order functionality yet
bool Bomb::validate() {
    return true;
}

void Bomb::execute() {
    if (Bomb::validate()) {
        cout << "Order is valid, now executing" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Bomb &bomb) {
    return strm << "Order Type: " << bomb.getOrderType();
}

//Assignment Operator
Bomb &Bomb::operator=(const Bomb &bomb) {
    this->orderType = bomb.orderType;
    return *this;
}

//Blockade Class
Blockade::Blockade() {
    this->orderType = "blockade";
}

//Copy constructor
Blockade::Blockade(const Blockade &blockade) {
    this->orderType = blockade.orderType;
}

Blockade::~Blockade() {
    
}

//Getter
string Blockade::getOrderType() {
    return orderType;
}

//Always return true, since we havent implemented the order functionality yet
bool Blockade::validate() {
    return true;
}

void Blockade::execute() {
    if (Blockade::validate()) {
        cout << "Order is valid, now executing" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Blockade &blockade) {
    return strm << "Order Type: " << blockade.getOrderType();
}


//Assignment Operator
Blockade &Blockade::operator=(const Blockade &blockade) {
    this->orderType = blockade.orderType;
    return *this;
}

//Airlift Class
Airlift::Airlift() {
    this->orderType = "airlift";
}

//Copy constructor
Airlift::Airlift(const Airlift &airlift) {
    this->orderType = airlift.orderType;
}

Airlift::~Airlift() {
    
}

//Getter
string Airlift::getOrderType() {
    return orderType;
}

//Always return true, since we havent implemented the order functionality yet
bool Airlift::validate() {
    return true;
}

void Airlift::execute() {
    if (Airlift::validate()) {
        cout << "Order is valid, now executing" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Airlift &airlift) {
    return strm << "Order Type: " << airlift.getOrderType();
}


//Assignment Operator
Airlift &Airlift::operator=(const Airlift &airlift) {
    this->orderType = airlift.orderType;
    return *this;
}

//Negotiate Class
Negotiate::Negotiate() {
    this->orderType = "negotiate";
}

//Copy constructor
Negotiate::Negotiate(const Negotiate &negotiate) {
    this->orderType = negotiate.orderType;
}

Negotiate::~Negotiate() {
    
}

//Getter
string Negotiate::getOrderType() {
    return orderType;
}

//Always return true, since we havent implemented the order functionality yet
bool Negotiate::validate() {
    return true;
}

void Negotiate::execute() {
    if (Negotiate::validate()) {
        cout << "Order is valid, now executing" << endl;
    }
}

//Stream insertion operator overload
ostream &operator<<(ostream &strm, Negotiate &negotiate) {
    return strm << "Order Type: " << negotiate.getOrderType();
}


//Assignment Operator
Negotiate &Negotiate::operator=(const Negotiate &negotiate) {
    this->orderType = negotiate.orderType;
    return *this;
}



