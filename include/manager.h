#ifndef MANAGER_H
#define MANAGER_H

#include "user.h"
using namespace std;

class Manager : public User {
public:
    Manager();

    void addStock(string title, string author, string type, double price, double cost, int quantity);
    void removeStock(string title, string author, int quantity);
    void deleteStock(string title, string author);
    void viewStockDetails();
};

#endif 
