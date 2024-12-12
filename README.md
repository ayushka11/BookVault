# BookVault

## Overview
The **Bookstore Management System** is a CLI-based application for managing bookstore operations, including stock management by managers and sales processing by cashiers. It uses `SQLite3` as its database backend.

---

## Features
1. **Manager Operations**: Add, remove, delete, and view stock.
2. **Cashier Operations**: Search for books, process sales, and generate bills.

---

## Technologies Used
- **Programming Language**: C++
- **Database**: SQLite3

---

## Steps to Run the System
1. **Install Dependencies**: Ensure `sqlite3` is installed on your system. You can install it using your package manager:
   - For Ubuntu/Debian: `sudo apt-get install sqlite3`

2. **Prepare the Database**:
   - Navigate to the `database` directory: `cd database`.
   - Create the database file:
     ```bash
     sqlite3 bookstore.db < schema.sql
     ```
   - This will initialize the required tables in `bookstore.db`.

3. **Compile the Code**:
   - Navigate to the `src` directory: `cd src`.
   - Compile the program using `g++`:
     ```bash
     g++ -o bookstore main.cpp SQLiteHelper.cpp manager.cpp cashier.cpp -lsqlite3 -lssl -lcrypto
     ```

4. **Run the Program**:
   - Execute the compiled binary:
     ```bash
     ./bookstore
     ```

5. **Passphrase Setup**:
   - Ensure the `passphrase.txt` file is in the parent directory (`../` relative to the executable).
   - Add passphrases for the **manager** and **cashier** in the file (line 1 for manager, line 2 for cashier).

6. **Using the System**:
   - Follow the on-screen instructions to log in as either a manager or a cashier.
   - Perform operations as per your role.

---

## Notes
- Ensure the database (`bookstore.db`) and passphrase file (`passphrase.txt`) are correctly set up before running the program.
- All data is stored persistently in the `SQLite3` database.
