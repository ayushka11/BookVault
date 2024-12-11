#include "../include/user.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <iostream>
#include "../include/SQLiteHelper.h"
using namespace std;

User::User(const string& role)
    : role(role) {
        
    }

string User::getRole() const {
    return role;
}