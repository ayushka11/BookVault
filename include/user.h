#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
using namespace std;

class User {
protected:
    string role;

public:
    User(const string& role);

    string getRole() const;
};

#endif
