#include "../include/manager.h"
#include "../include/SQLiteHelper.h"
#include <iostream>
#include <iomanip>

using namespace std;

Manager::Manager()
    : User("manager") {}

void Manager::addStock(string title, string author, string type, double price, double cost, int quantity) {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string checkQuery = "SELECT COUNT(*) FROM books WHERE title = ? AND author = ? AND type = ?";
    vector<string> checkParams = { title, author, type };
    int count = dbHelper.executeScalarQueryWithParams(checkQuery, checkParams);

    if (count > 0) {
        string getCostQuery = "SELECT cost, quantity FROM books WHERE title = ? AND author = ? AND type = ?";
        double existingCost = dbHelper.executeScalarQueryWithParams(getCostQuery, checkParams);
        int existingQuantity = dbHelper.executeScalarQueryWithParams(getCostQuery, checkParams);
        double newCost = (existingCost * existingQuantity + cost * quantity) / (existingQuantity + quantity);
        string updateQuery = "UPDATE books SET quantity = quantity + ?, price = ?, cost = ? WHERE title = ? AND author = ? AND type = ?";
        vector<string> updateParams = { to_string(quantity), to_string(price), to_string(newCost), title, author, type };
        if (dbHelper.executeQueryWithParams(updateQuery, updateParams)) {
            cout << "Stock updated successfully.\n";
        } else {
            cerr << "Error updating stock in the database.\n";
        }
    } else {
        string insertQuery = "INSERT INTO books (title, author, type, price, cost, quantity) VALUES (?, ?, ?, ?, ?, ?)";
        vector<string> insertParams = { title, author, type, to_string(price), to_string(cost), to_string(quantity) };
        if (dbHelper.executeQueryWithParams(insertQuery, insertParams)) {
            cout << "New stock added successfully.\n";
        } else {
            cerr << "Error adding new stock to the database.\n";
        }
    }
}

void Manager:: removeStock(string title, string author, int quantity) {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string checkQuery = "SELECT COUNT(*) FROM books WHERE title = ? AND author = ?";
    vector<string> checkParams = { title, author };
    int count = dbHelper.executeScalarQueryWithParams(checkQuery, checkParams);

    if (count > 0) {
        string checkQuantityQuery = "SELECT quantity FROM books WHERE title = ? AND author = ?";
        int existingQuantity = dbHelper.executeScalarQueryWithParams(checkQuantityQuery, checkParams);
        if (existingQuantity < quantity) {
            cerr << "Error: Quantity to be removed is more than existing quantity.\n";
            return;
        }
        string updateQuery = "UPDATE books SET quantity = quantity - ? WHERE title = ? AND author = ?";
        vector<string> updateParams = { to_string(quantity), title, author };
        if (dbHelper.executeQueryWithParams(updateQuery, updateParams)) {
            cout << "Stock updated successfully.\n";
        } else {
            cerr << "Error updating stock in the database.\n";
        }
    } else {
        cerr << "Error: Book not found in the database.\n";
    }
}

void Manager::deleteStock(string title, string author) {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string checkQuery = "SELECT COUNT(*) FROM books WHERE title = ? AND author = ?";
    vector<string> checkParams = { title, author };
    int count = dbHelper.executeScalarQueryWithParams(checkQuery, checkParams);

    if (count > 0) {
        string deleteQuery = "DELETE FROM books WHERE title = ? AND author = ?";
        vector<string> deleteParams = { title, author };
        if (dbHelper.executeQueryWithParams(deleteQuery, deleteParams)) {
            cout << "Stock removed successfully.\n";
        } else {
            cerr << "Error removing stock from the database.\n";
        }
    } else {
        cerr << "Error: Book not found in the database.\n";
    }
}

void Manager::viewStockDetails() {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string query = "SELECT id, title, author, type, price, quantity, cost, added_date FROM books";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbHelper.getDb(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        cout << setw(5) << "ID" 
                  << setw(30) << "Title" 
                  << setw(20) << "Author" 
                  << setw(10) << "Type" 
                  << setw(10) << "Price" 
                  << setw(10) << "Quantity" 
                  << setw(10) << "Cost" 
                  << setw(20) << "Added Date" << "\n";

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            double price = sqlite3_column_double(stmt, 4);
            int quantity = sqlite3_column_int(stmt, 5);
            double cost = sqlite3_column_double(stmt, 6);
            string addedDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

            cout << setw(5) << id 
                      << setw(30) << title 
                      << setw(20) << author 
                      << setw(10) << type 
                      << setw(10) << price 
                      << setw(10) << quantity 
                      << setw(10) << cost 
                      << setw(20) << addedDate << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Error retrieving stock details from the database.\n";
    }
}
