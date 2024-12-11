#include "../include/cashier.h"
#include "../include/SQLiteHelper.h"
#include <iostream>
#include <iomanip>
using namespace std;

Cashier::Cashier()
    : User("cashier") {}

void Cashier::searchBookByTitle(const string& title) const {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string query = "SELECT id, title, author, type, price, quantity FROM books WHERE title LIKE ?";
    vector<string> params = { "%" + title + "%" };

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dbHelper.getDb(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, params[0].c_str(), -1, SQLITE_TRANSIENT);

        cout << "Books matching title:\n";
        cout << setw(5) << "ID" 
             << setw(30) << "Title" 
             << setw(20) << "Author" 
             << setw(10) << "Type" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" << "\n";

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string bookTitle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            double price = sqlite3_column_double(stmt, 4);
            int quantity = sqlite3_column_int(stmt, 5);

            cout << setw(5) << id 
                 << setw(30) << bookTitle 
                 << setw(20) << author 
                 << setw(10) << type 
                 << setw(10) << price 
                 << setw(10) << quantity << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Error searching books by title." << endl;
    }
}

void Cashier::searchBookByAuthor(const string& author) const {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string query = "SELECT id, title, author, type, price, quantity FROM books WHERE author LIKE ?";
    vector<string> params = { "%" + author + "%" };

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(dbHelper.getDb(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, params[0].c_str(), -1, SQLITE_TRANSIENT);

        cout << "Books by author:\n";
        cout << setw(5) << "ID" 
             << setw(30) << "Title" 
             << setw(20) << "Author" 
             << setw(10) << "Type" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" << "\n";

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            string bookTitle = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            string bookAuthor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            double price = sqlite3_column_double(stmt, 4);
            int quantity = sqlite3_column_int(stmt, 5);

            cout << setw(5) << id 
                 << setw(30) << bookTitle 
                 << setw(20) << bookAuthor 
                 << setw(10) << type 
                 << setw(10) << price 
                 << setw(10) << quantity << "\n";
        }

        sqlite3_finalize(stmt);
    } else {
        cerr << "Error searching books by author." << endl;
    }
}

int Cashier::checkExistingCustomer(const string& name, const string& contact) const {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string query = "SELECT id FROM customers WHERE name = ? AND contact = ?";
    vector<string> params = { name, contact };

    return dbHelper.executeScalarQueryWithParams(query, params);
}

void Cashier::addNewCustomer(const string& name, const string& contact) const {
    SQLiteHelper dbHelper("../database/bookstore.db");

    string query = "INSERT INTO customers (name, contact) VALUES (?, ?)";
    vector<string> params = { name, contact };

    if (dbHelper.executeQueryWithParams(query, params)) {
        cout << "Customer added successfully.\n";
    } else {
        cerr << "Error adding customer.\n";
    }
}

void Cashier::processSales(const string& customerName, const string& contact, const vector<tuple<string, string, string, int>>& bookDetails) {
    SQLiteHelper dbHelper("../database/bookstore.db");

    int customerId = checkExistingCustomer(customerName, contact);
    if (customerId == -1) {
        addNewCustomer(customerName, contact);
        customerId = checkExistingCustomer(customerName, contact);
        if (customerId == -1) {
            cerr << "Error: Unable to create or fetch customer.\n";
            return;
        }
    }

    double totalTransactionPrice = 0.0;
    int transactionId = -1;

    string insertTransactionQuery = "INSERT INTO transactions (customer_id, total_price) VALUES (?, ?, ?)";
    vector<string> transactionInitParams = { to_string(customerId), "0.0" };
    if (dbHelper.executeQueryWithParams(insertTransactionQuery, transactionInitParams)) {
        transactionId = sqlite3_last_insert_rowid(dbHelper.getDb());
        cout << "Transaction initiated with ID: " << transactionId << "\n";
    } else {
        cerr << "Error initializing transaction.\n";
        return;
    }

    for (const auto& book : bookDetails) {
        string title = get<0>(book);
        string author = get<1>(book);
        string type = get<2>(book);
        int quantityToBuy = get<3>(book);

        string checkBookQuery = "SELECT id FROM books WHERE title = ? AND author = ? AND type = ?";
        vector<string> bookParams = { title, author, type };
        int bookId = dbHelper.executeScalarQueryWithParams(checkBookQuery, bookParams);
        if (bookId == -1) {
            cerr << "Error: Book not found.\n";
            continue;
        }

        string getQuantityQuery = "SELECT quantity FROM books WHERE id = ?";
        vector<string> getQuantityParams = { to_string(bookId) };
        int availableQuantity = dbHelper.executeScalarQueryWithParams(getQuantityQuery, getQuantityParams);
        if (availableQuantity < quantityToBuy) {
            cerr << "Error: Insufficient stock for " << title << " by " << author << ".\n";
            continue;
        }

        string updateQuantityQuery = "UPDATE books SET quantity = ? WHERE id = ?";
        vector<string> updateQuantityParams = { to_string(availableQuantity - quantityToBuy), to_string(bookId) };
        if (dbHelper.executeQueryWithParams(updateQuantityQuery, updateQuantityParams)) {
            cout << "Stock updated for " << title << " by " << author << ".\n";
        } else {
            cerr << "Error updating stock for " << title << " by " << author << ".\n";
            continue;
        }

        string getPriceQuery = "SELECT price FROM books WHERE id = ?";
        vector<string> getPriceParams = { to_string(bookId) };
        double bookPrice = dbHelper.executeScalarQueryWithParams(getPriceQuery, getPriceParams);

        string insertSaleQuery = "INSERT INTO sales (transaction_id, book_id, quantity, price_per_unit, total_price) VALUES (?, ?, ?, ?, ?)";
        vector<string> saleParams = { to_string(transactionId), to_string(bookId), to_string(quantityToBuy), to_string(bookPrice), to_string(quantityToBuy * bookPrice) };
        if (dbHelper.executeQueryWithParams(insertSaleQuery, saleParams)) {
            cout << "Sale recorded for " << title << " by " << author << ".\n";
            totalTransactionPrice += quantityToBuy * bookPrice;
        } else {
            cerr << "Error recording sale for " << title << " by " << author << ".\n";
        }
    }

    string updateTransactionQuery = "UPDATE transactions SET total_price = ? WHERE id = ?";
    vector<string> updateTransactionParams = { to_string(totalTransactionPrice), to_string(transactionId) };
    if (dbHelper.executeQueryWithParams(updateTransactionQuery, updateTransactionParams)) {
        cout << "Transaction completed successfully. Total price: " << totalTransactionPrice << "\n";
    } else {
        cerr << "Error updating transaction total price.\n";
    }
}
