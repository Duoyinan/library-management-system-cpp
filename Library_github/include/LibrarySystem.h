#pragma once
#include <vector>
#include "Book.h"
#include "User.h"
#include "BorrowRecord.h"

class LibrarySystem {
public:
    LibrarySystem();

    void mainMenu();
    void addBook();
    void addUser();
    void borrowBook();
    void returnBook();
    void showBooks();
    void showUsers();
    void showRecords();

private:
    std::vector<Book> books;
    std::vector<User> users;
    std::vector<BorrowRecord> records;
};
