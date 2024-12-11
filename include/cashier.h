#ifndef CASHIER_H
#define CASHIER_H

#include "user.h"
using namespace std;

class Cashier : public User {
public:
    Cashier();

    void searchBookByTitle(const string& title) const;
    void searchBookByAuthor(const string& author) const;
    
    int checkExistingCustomer(const string& name, const string& contact) const;
    void addNewCustomer(const string& name, const string& contact) const;

    void processSales(const string& customerName, const string& contact, const vector<tuple<string, string, string, int>>& bookDetails);
};

#endif 
