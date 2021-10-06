#ifndef Orders
#define Orders
#include <string>
#include <vector>
using namespace std;

class Order {
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
};

class OrderList {
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
        //Moves an order from one index to a new one
        void moveOrder(string orderType, int newIndex);

        bool hasOrder(string orderType);

        //Stream insertion operator
        friend ostream &operator<<(ostream &stream, OrderList &orderList);
        //Assignment operation
        OrderList &operator=(const OrderList &orderList);
};

// List of necessary orders: deploy, advance, bomb, blockade, airlift, and negotiate. Must contain execute and validate function, nothing else is needed at this point (10/4/2021)

//Deploy Order
class Deploy : public Order {

    private:
        string orderType;

    public:
        //Constructors
        Deploy();
        Deploy(const Deploy &deploy);
        //Destructor
        ~Deploy();

        //Getter
        string getOrderType();

        //Is our order valid
        bool validate();

        //Executes the order
        void execute();

        //Insertion Operator
        friend ostream &operator<<(ostream &stream, Deploy &deploy);

        //Assignment Operator
        Deploy &operator=(const Deploy &dep);
};

class Advance : public Order {

    private:
        string orderType;

    public:
        //Constructors
        Advance();
        Advance(const Advance &advance);
        //Destructor
        ~Advance();

        //Getter
        string getOrderType();

        //Is our order valid
        bool validate();

        //Executes the order
        void execute();

        //Insertion Operator
        friend ostream &operator<<(ostream &stream, Advance &advance);

        //Assignment Operator
        Advance &operator=(const Advance &advance);
};

class Bomb : public Order {

    private:
        string orderType;

    public:
        //Constructors
        Bomb();
        Bomb(const Bomb &bomb);
        //Destructor
        ~Bomb();

        //Getter
        string getOrderType();

        //Is our order valid
        bool validate();

        //Executes the order
        void execute();

        //Insertion Operator
        friend ostream &operator<<(ostream &stream, Bomb &bomb);

        //Assignment Operator
        Bomb &operator=(const Bomb &bomb);
};

class Blockade : public Order {

    private:
        string orderType;

    public:
        //Constructors
        Blockade();
        Blockade(const Blockade &blockade);
        //Destructor
        ~Blockade();

        //Getter
        string getOrderType();

        //Is our order valid
        bool validate();

        //Executes the order
        void execute();

        //Insertion Operator
        friend ostream &operator<<(ostream &stream, Blockade &blockade);

        //Assignment Operator
        Blockade &operator=(const Blockade &blockade);
};

class Airlift : public Order {

    private:
        string orderType;

    public:
        //Constructors
        Airlift();
        Airlift(const Airlift &airlift);
        //Destructor
        ~Airlift();

        //Getter
        string getOrderType();

        //Is our order valid
        bool validate();

        //Executes the order
        void execute();

        //Insertion Operator
        friend ostream &operator<<(ostream &stream, Airlift &airlift);

        //Assignment Operator
        Airlift &operator=(const Airlift &airLift);
};

class Negotiate : public Order {

    private:
        string orderType;

    public:
        //Constructors
        Negotiate();
        Negotiate(const Negotiate &negotiate);
        //Destructor
        ~Negotiate();

        //Getter
        string getOrderType();

        //Is our order valid
        bool validate();

        //Executes the order
        void execute();

        //Insertion Operator
        friend ostream &operator<<(ostream &stream, Negotiate &negotiate);

        //Assignment Operator
        Negotiate &operator=(const Negotiate &negotiate);
};
#endif