#include "UserManager.h"
#include "BookManager.h"
#include "BorrowManager.h"
#include "ExceptionHelper.h"
#include <iostream>
#include <string>

int main() {
    UserManager userManager;
    BookManager bookManager;
    BorrowManager borrowManager;
    ExceptionHelper::runWithCatch([&]() {
        userManager.load("data/users.txt");
        bookManager.load("data/books.txt");
        borrowManager.load("data/records.txt");
    }, "加载数据文件");

    std::cout << "==== Library Management System ====" << std::endl;
    std::string username, password;
    std::cout << "Login\nUsername: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    User* user = nullptr;
    ExceptionHelper::runWithCatch([&]() {
        user = userManager.login(username, password);
    }, "用户登录");

    if (!user) {
        std::cout << "Login failed!" << std::endl;
        return 0;
    }
    std::cout << "Welcome, " << user->username << "!\n";

    int choice;
    do {
        std::cout << "\n1. List Books\n2. Borrow Book\n3. Return Book\n4. Add Book (admin)\n5. AI推荐\n0. Exit\nChoice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                ExceptionHelper::runWithCatch([&]() {
                    for (const auto& b : bookManager.books)
                        std::cout << b.bookId << " | " << b.title << " | " << b.author
                                << " | " << b.availableCount << "/" << b.totalCount << std::endl;
                }, "展示书籍列表");
                break;
            case 2: {
                std::string bid;
                std::cout << "Book ID to borrow: ";
                std::cin >> bid;
                std::string rid = "R" + std::to_string(rand() % 100000);
                ExceptionHelper::runWithCatch([&]() {
                    if (borrowManager.borrowBook(rid, user->userId, bookManager, bid)) {
                        std::cout << "Borrowed successfully.\n";
                    } else {
                        std::cout << "Borrow failed.\n";
                    }
                }, "借阅图书");
                break;
            }
            case 3: {
                std::string bid;
                std::cout << "Book ID to return: ";
                std::cin >> bid;
                ExceptionHelper::runWithCatch([&]() {
                    if (borrowManager.returnBook(user->userId, bookManager, bid)) {
                        std::cout << "Returned successfully.\n";
                    } else {
                        std::cout << "Return failed.\n";
                    }
                }, "归还图书");
                break;
            }
            case 4:
                if (user->role == Role::Admin) {
                    std::string bookId, title, author;
                    int total;
                    std::cout << "Book ID: "; std::cin >> bookId;
                    std::cout << "Title: "; std::cin >> title;
                    std::cout << "Author: "; std::cin >> author;
                    std::cout << "Total Count: "; std::cin >> total;
                    ExceptionHelper::runWithCatch([&]() {
                        bookManager.addBook(Book(bookId, title, author, total, total));
                        std::cout << "Book added.\n";
                    }, "添加图书");
                } else {
                    std::cout << "Permission denied: only admin can add books.\n";
                }
                break;
            case 5:
                ExceptionHelper::runWithCatch([&]() {
                    auto recs = AIRecommend::recommendBooks(user->username, "auto");
                    std::cout << "为你推荐图书：" << std::endl;
                    for (const auto& b : recs)
                        std::cout << b.id << " | " << b.title << " | " << b.author << std::endl;
                }, "AI智能推荐");
                break;
            case 0:
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    } while (choice != 0);

    ExceptionHelper::runWithCatch([&]() {
        userManager.save("data/users.txt");
        bookManager.save("data/books.txt");
        borrowManager.save("data/records.txt");
    }, "保存数据文件");
    std::cout << "Bye!\n";
    return 0;
}
