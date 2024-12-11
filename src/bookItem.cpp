// #include "../include/bookItem.h"
// // #include <iomanip>
// // #include <sstream>
// #include <iostream>
// #include <vector>
// #include "../include/SQLiteHelper.h"
// using namespace std;

// BookItem::BookItem(const string& title, const string& author, const string& type, double price, double cost, int quantity)
//     : title(title), author(author), type(type), price(price), cost(cost), quantity(quantity) {

//     SQLiteHelper dbHelper("../database/bookstore.db");

//     string query = "INSERT INTO books (title, author, type, price, cost, quantity) VALUES (?, ?, ?, ?, ?, ?, ?)";
//     vector<string> params = {
//         title, author, type, to_string(price), to_string(cost), to_string(quantity)
//     };

//     if (!dbHelper.executeQueryWithParams(query, params)) {
//         cerr << "Error inserting book into database.\n";
//     }
// }

// string BookItem::getTitle() const {
//     return title;
// }

// string BookItem::getAuthor() const {
//     return author;
// }

// string BookItem::getType() const {
//     return type;
// }

// double BookItem::getPrice() const {
//     return price;
// }

// int BookItem::getQuantity() const {
//     return quantity;
// }

// double BookItem::getCost() const {
//     return cost;
// }

// string BookItem::getAddedDate() const {
//     char buffer[20];
//     strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&addedDate));
//     return string(buffer);
// }

// void BookItem::setPrice(double newPrice) {
//     price = newPrice;

//     SQLiteHelper dbHelper("../database/bookstore.db");
//     string query = "UPDATE books SET price = ? WHERE title = ? AND author = ?";
//     vector<string> params = { to_string(newPrice), title, author };

//     if (!dbHelper.executeQueryWithParams(query, params)) {
//         cerr << "Error updating price in database for book : " << title << "\n";
//     }
// }

// void BookItem::setQuantity(int newQuantity) {
//     quantity = newQuantity;

//     SQLiteHelper dbHelper("../database/bookstore.db");
//     string query = "UPDATE books SET quantity = ? WHERE title = ? AND author = ?";
//     vector<string> params = { to_string(newQuantity), title, author };

//     if (!dbHelper.executeQueryWithParams(query, params)) {
//         cerr << "Error updating quantity in database for book : " << title << "\n";
//     }
// }

// void BookItem::displayDetails() const {
//     cout << "Title: " << title << "\n"
//               << "Author: " << author << "\n"
//               << "Type: " << type << "\n"
//               << "Price: $" << price << "\n"
//               << "Quantity: " << quantity << "\n"
//               << "Cost to Store: $" << cost << "\n"
//               << "Added Date: " << getAddedDate() << "\n";
// }

// void BookItem::updateStock(int amount) {
//     quantity += amount;

//     SQLiteHelper dbHelper("../database/bookstore.db");
//     string query = "UPDATE books SET quantity = ? WHERE title = ? AND author = ?";
//     vector<string> params = { to_string(quantity), title, author };

//     if (!dbHelper.executeQueryWithParams(query, params)) {
//         cerr << "Error updating quantity in database for book : " << title << "\n";
//     }
// }
