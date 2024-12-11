#include <iostream>
#include "../include/manager.h"
#include "../include/SQLiteHelper.h"
#include "../include/cashier.h"
#include <vector>
#include <tuple>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    bool isRunning = true;

    while (isRunning)
    {
        cout << "Select user type:\n";
        cout << "1. Manager\n";
        cout << "2. Cashier\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        string choiceString;
        getline(cin, choiceString); // Use getline to read input

        if (choiceString != "1" && choiceString != "2" && choiceString != "3")
        {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        int choice = stoi(choiceString);

        string managerPassphrase, cashierPassphrase;
        ifstream file("../passphrase.txt");
        if (file.is_open())
        {
            getline(file, managerPassphrase);
            getline(file, cashierPassphrase);
            file.close();
        }
        else
        {
            cout << "Error opening passphrase file.\n";
            return 1;
        }

        if (choice == 3)
        {
            isRunning = false;
            cout << "Exiting program.\n";
            break;
        }

        cout << "Enter passphrase: ";
        string passphrase;
        getline(cin, passphrase);

        if ((choice == 1 && passphrase != managerPassphrase) || (choice == 2 && passphrase != cashierPassphrase))
        {
            cout << "Invalid passphrase. Please try again.\n";
            continue;
        }

        if (choice == 1)
        {
            Manager manager;
            bool managerRunning = true;

            while (managerRunning)
            {
                cout << "Select operation:\n";
                cout << "1. Add stock\n";
                cout << "2. Remove stock\n";
                cout << "3. Delete stock\n";
                cout << "4. View stock details\n";
                cout << "5. Return to main menu\n";
                cout << "Enter choice: ";

                string operationString;
                getline(cin, operationString);

                if (operationString != "1" && operationString != "2" && operationString != "3" && operationString != "4" && operationString != "5")
                {
                    cout << "Invalid choice. Please try again.\n";
                    continue;
                }

                int operation = stoi(operationString);

                switch (operation)
                {
                case 1:
                {
                    string title, author, type;
                    double price, cost;
                    int quantity;

                    cout << "Enter title: ";
                    getline(cin, title);

                    cout << "Enter author: ";
                    getline(cin, author);

                    cout << "Enter type: ";
                    getline(cin, type);

                    cout << "Enter price: ";
                    cin >> price;
                    cin.ignore();

                    cout << "Enter cost: ";
                    cin >> cost;
                    cin.ignore();

                    cout << "Enter quantity: ";
                    cin >> quantity;
                    cin.ignore();

                    manager.addStock(title, author, type, price, cost, quantity);
                    break;
                }
                case 2:
                {
                    string title, author;
                    int quantity;

                    cout << "Enter title: ";
                    getline(cin, title);

                    cout << "Enter author: ";
                    getline(cin, author);

                    cout << "Enter quantity: ";
                    cin >> quantity;
                    cin.ignore();

                    manager.removeStock(title, author, quantity);
                    break;
                }
                case 3:
                {
                    string title, author;

                    cout << "Enter title: ";
                    getline(cin, title);

                    cout << "Enter author: ";
                    getline(cin, author);

                    manager.deleteStock(title, author);
                    break;
                }
                case 4:
                {
                    manager.viewStockDetails();
                    break;
                }
                case 5:
                {
                    managerRunning = false;
                    break;
                }
                default:
                {
                    cout << "Invalid choice. Please try again.\n";
                    break;
                }
                }
            }
        }

        if (choice == 2)
        {
            Cashier cashier;
            bool cashierRunning = true;

            while (cashierRunning)
            {
                cout << "Select operation:\n";
                cout << "1. Search book by title\n";
                cout << "2. Search book by author\n";
                cout << "3. Process sales\n";
                cout << "4. Return to main menu\n";
                cout << "Enter choice: ";

                string operationString;
                getline(cin, operationString);

                if (operationString != "1" && operationString != "2" && operationString != "3" && operationString != "4")
                {
                    cout << "Invalid choice. Please try again.\n";
                    continue;
                }

                int operation = stoi(operationString);

                switch (operation)
                {
                case 1:
                {
                    string title;
                    cout << "Enter title: ";
                    getline(cin, title);
                    cashier.searchBookByTitle(title);
                    break;
                }
                case 2:
                {
                    string author;
                    cout << "Enter author: ";
                    getline(cin, author);
                    cashier.searchBookByAuthor(author);
                    break;
                }
                case 3:
                {
                    string customerName, contact;
                    cout << "Enter customer name: ";
                    getline(cin, customerName);

                    cout << "Enter contact: ";
                    getline(cin, contact);

                    vector<tuple<string, string, string, int>> bookDetails;

                    while (true)
                    {
                        string title, author, type;
                        int quantity;
                        cout << "Enter title: ";
                        getline(cin, title);

                        cout << "Enter author: ";
                        getline(cin, author);

                        cout << "Enter type: ";
                        getline(cin, type);

                        cout << "Enter quantity: ";
                        cin >> quantity;
                        cin.ignore();

                        bookDetails.push_back(make_tuple(title, author, type, quantity));

                        cout << "Do you want to add more books? (y/n): ";
                        string choice;
                        getline(cin, choice);

                        if (choice == "n" || choice == "N")
                        {
                            break;
                        }
                    }

                    cashier.processSales(customerName, contact, bookDetails);
                    break;
                }
                case 4:
                {
                    cashierRunning = false;
                    break;
                }
                default:
                {
                    cout << "Invalid choice. Please try again.\n";
                    break;
                }
                }
            }
        }
    }

    return 0;
}
