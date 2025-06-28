#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h"
#include <iostream>
#include <string>

int main() {
    UserManager userManager;
    BookManager bookManager;
    BorrowManager borrowManager;
    userManager.load("data/users.txt");
    bookManager.load("data/books.txt");
    borrowManager.load("data/records.txt");

    std::cout << "==== Library Management System ====" << std::endl;
    std::string username, password;
    std::cout << "Login\nUsername: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    User* user = userManager.login(username, password);
    if (!user) {
        std::cout << "Login failed!" << std::endl;
        return 0;
    }
    std::cout << "Welcome, " << user->username << "!\n";

    int choice;
    do {
        std::cout << "\n1. List Books\n2. Borrow Book\n3. Return Book\n4. Add Book (admin)\n0. Exit\nChoice: ";
        std::cin >> choice;
        if (choice == 1) {
            for (const auto& b : bookManager.books)
                std::cout << b.bookId << " | " << b.title << " | " << b.author
                          << " | " << b.availableCount << "/" << b.totalCount << std::endl;
        } else if (choice == 2) {
            std::string bid;
            std::cout << "Book ID to borrow: ";
            std::cin >> bid;
            std::string rid = "R" + std::to_string(rand() % 100000);
            if (borrowManager.borrowBook(rid, user->userId, bookManager, bid)) {
                std::cout << "Borrowed successfully.\n";
            } else {
                std::cout << "Borrow failed.\n";
            }
        } else if (choice == 3) {
            std::string bid;
            std::cout << "Book ID to return: ";
            std::cin >> bid;
            if (borrowManager.returnBook(user->userId, bookManager, bid)) {
                std::cout << "Returned successfully.\n";
            } else {
                std::cout << "Return failed.\n";
            }
        } else if (choice == 4 && user->role == Role::Admin) {
            std::string bookId, title, author;
            int total;
            std::cout << "Book ID: "; std::cin >> bookId;
            std::cout << "Title: "; std::cin >> title;
            std::cout << "Author: "; std::cin >> author;
            std::cout << "Total Count: "; std::cin >> total;
            bookManager.addBook(Book(bookId, title, author, total, total));
            std::cout << "Book added.\n";
        }
    } while (choice != 0);

    userManager.save("data/users.txt");
    bookManager.save("data/books.txt");
    borrowManager.save("data/records.txt");
    std::cout << "Bye!\n";
    return 0;
}
