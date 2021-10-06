#include "Orders.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    //Create orders of each type
    Deploy *deployOrder = new Deploy;
    Advance *advanceOrder = new Advance;
    Bomb *bombOrder = new Bomb;
    Blockade *blockadeOrder = new Blockade;
    Airlift *airliftOrder = new Airlift;
    Negotiate *negotiateOrder = new Negotiate;

    //We will validate and then execute, we already check for validation in execution, but this is just to test the different methods
    string isValid = deployOrder->validate() ? "true" : "false";
    cout << "Deploy order is valid: " + isValid  << endl; 
    deployOrder->execute();
    cout << "-------" << endl;
    isValid = advanceOrder->validate() ? "true" : "false";
    cout << "Advance order is valid: " + isValid  << endl;
    advanceOrder->execute();
    cout << "-------" << endl;
    isValid = bombOrder->validate() ? "true" : "false";
    cout << "Bomb order is valid: " +  isValid << endl;
    bombOrder->execute();
    cout << "-------" << endl;
    isValid = blockadeOrder->validate() ? "true" : "false";
    cout << "Blockade order is valid: " + isValid << endl;
    blockadeOrder->execute();
    cout << "-------" << endl;
    isValid = airliftOrder->validate() ? "true" : "false";
    cout << "Airlift order is valid: " + isValid << endl;
    airliftOrder->execute();
    cout << "-------" << endl;
    isValid = negotiateOrder->validate() ? "true" : "false";
    cout << "Negotiate order is valid: " + isValid << endl;
    negotiateOrder->execute();
    cout << "-------" << endl;

    //Create our OrderList
    OrderList orderList;

    //Populate the list
    orderList.addOrder(deployOrder);
    orderList.addOrder(advanceOrder);
    orderList.addOrder(bombOrder);
    orderList.addOrder(blockadeOrder);
    orderList.addOrder(airliftOrder);
    orderList.addOrder(negotiateOrder);

    //Show stream insertion operator
    cout << "Order list contains: \n" << orderList << endl;

    //Delete element at index 1
    orderList.deleteOrder(1);

    //Show delete function worked
    cout << "Order list contains after deletion: \n" << orderList << endl;

    //Move order at position 2 to position 5
    orderList.moveOrder("bomb", 4);

    //Show move order function worked
    cout << "Order list contains after move: \n" << orderList << endl;

    return 0;
}