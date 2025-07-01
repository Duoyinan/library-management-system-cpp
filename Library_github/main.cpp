#include "include/UserManager.h"
#include "include/BookManager.h"
#include "include/BorrowManager.h"
#include "include/ExceptionHelper.h"
#include <iostream>
#include <string>
#include <cstdlib>

void safeInputInt(const std::string& prompt, int& value) {
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "请输入有效的整数：" << std::endl << prompt;
    }
}

void readerMenu(User* user, UserManager& userManager, BookManager& bookManager, BorrowManager& borrowManager);
void adminMenu(User* user, UserManager& userManager, BookManager& bookManager, BorrowManager& borrowManager);

int main() {
    UserManager userManager;
    BookManager bookManager;
    BorrowManager borrowManager;
    ExceptionHelper::runWithCatch([&]() {
        userManager.load("data/users.txt");
        bookManager.load("data/books.txt");
        borrowManager.load("data/records.txt");
    }, "加载数据文件");

    int roleChoice = -1;
    while (roleChoice != 0) {
        std::cout << "\n请选择您的身份：\n1. 读者\n2. 管理者\n0. 退出\n选择：";
        safeInputInt("", roleChoice);
        if (roleChoice == 0) break;

        std::string username, password;
        std::cout << "用户名：";
        std::cin >> username;
        std::cout << "密码：";
        std::cin >> password;

        User* user = nullptr;
        if (roleChoice == 1) {
            ExceptionHelper::runWithCatch([&]() {
                user = userManager.login(username, password, Role::Reader);
                std::cout << "欢迎您，读者 " << user->username 
                          << (user->isVIP ? "（VIP)" : "") << "\n";
                readerMenu(user, userManager, bookManager, borrowManager);
            }, "读者登录");
            if (!user) {
                std::cout << "登录失败，可以注册新账号，是否注册？(y/n)：";
                char reg;
                std::cin >> reg;
                if (reg == 'y' || reg == 'Y') {
                    char vipchoose;
                    bool isVIP = false;
                    std::cout << "你想申请VIP吗？(y/n)：";
                    std::cin >> vipchoose;
                    if (vipchoose == 'y' || vipchoose == 'Y') {
                        std::string admpass;
                        std::cout << "请输入管理员密码：";
                        std::cin >> admpass;
                        if (admpass == "123456") isVIP = true;
                        else { std::cout << "管理员密码错误，注册普通用户。\n"; }
                    }
                    std::string newpwd;
                    std::cout << "请输入新密码：";
                    std::cin >> newpwd;
                    if (userManager.addUser(username, newpwd, Role::Reader, isVIP)) {
                        ExceptionHelper::runWithCatch([&]() {
                            userManager.save("data/users.txt");
                        }, "保存注册信息");
                        std::cout << "注册成功，请重新登录。\n";
                    } else {
                        std::cout << "用户名已存在。\n";
                    }
                }
            }
        } else if (roleChoice == 2) {
            ExceptionHelper::runWithCatch([&]() {
                user = userManager.login(username, password, Role::Admin);
                std::cout << "欢迎您，管理员 " << user->username << "！\n";
                adminMenu(user, userManager, bookManager, borrowManager);
            }, "管理员登录");
        } else {
            std::cout << "无效选项。\n";
        }
    }
    ExceptionHelper::runWithCatch([&]() {
        userManager.save("data/users.txt");
        bookManager.save("data/books.txt");
        borrowManager.save("data/records.txt");
    }, "保存数据文件");
    std::cout << "Bye!\n";
    return 0;
}

void readerMenu(User* user, UserManager& userManager, BookManager& bookManager, BorrowManager& borrowManager) {
    int choice;
    do {
        std::cout << "\n----- 读者菜单 -----\n";
        std::cout << "1. 查看所有图书\n2. 借书\n3. 还书\n4. 查看我的借阅历史\n5. 存款\n6. 查看余额\n0. 退出\n选择：";
        safeInputInt("", choice);
        switch (choice) {
            case 1:
                ExceptionHelper::runWithCatch([&]() {
                    std::cout << "图书列表：\n";
                    for (const auto& b : bookManager.books)
                        std::cout << b.bookId << " | " << b.title << " | " << b.author
                                  << " | " << b.availableCount << "/" << b.totalCount << std::endl;
                }, "展示书籍列表");
                break;
            case 2: {
                std::string bid;
                std::cout << "请输入要借的图书ID：";
                std::cin >> bid;
                std::string rid = "R" + std::to_string(rand() % 100000);
                ExceptionHelper::runWithCatch([&]() {
                    borrowManager.borrowBook(rid, *user, bookManager, bid);
                    std::cout << "借阅成功。\n";
                }, "借阅图书");
                break;
            }
            case 3: {
                std::string bid;
                std::cout << "请输入要归还的图书ID：";
                std::cin >> bid;
                ExceptionHelper::runWithCatch([&]() {
                    borrowManager.returnBook(*user, bookManager, bid);
                    std::cout << "归还成功。\n";
                }, "归还图书");
                break;
            }
            case 4:
                ExceptionHelper::runWithCatch([&]() {
                    std::cout << "你的借阅历史如下：\n";
                    for (const auto& r : borrowManager.records) {
                        if (r.userId == user->userId) {
                            std::cout << "记录号：" << r.recordId
                                    << " | 图书ID：" << r.bookId
                                    << " | 借出日期：" << r.borrowDate
                                    << " | 到期日：" << r.dueDate
                                    << " | 归还日期：" << (r.returnDate.empty() ? "未归还" : r.returnDate)
                                    << std::endl;
                        }
                    }
                }, "查看借阅历史");
                break;
            case 5: { // 存款
                std::cout << "请输入管理员密码：";
                std::string admpass;
                std::cin >> admpass;
                if (admpass != "123456") { std::cout << "管理员密码错误！\n"; break; }
                double amount;
                std::cout << "请输入存款金额：";
                while (!(std::cin >> amount)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "请输入有效的金额：" << std::endl << "请输入存款金额：";
                }
                user->balance += amount;
                std::cout << "存款成功，当前余额：" << user->balance << " 元\n";
                break;
            }
            case 6:
                std::cout << "当前余额：" << user->balance << " 元\n";
                break;
            case 0:
                break;
            default:
                std::cout << "无效选项。\n";
        }
    } while (choice != 0);
}

void adminMenu(User* user, UserManager& userManager, BookManager& bookManager, BorrowManager& borrowManager) {
    int choice;
    do {
        std::cout << "\n----- 管理员菜单 -----\n";
        std::cout << "1. 查看所有图书\n2. 添加图书\n3. 删除图书\n4. 查看所有用户\n5. 删除用户\n6. 查看所有借阅记录\n0. 退出\n选择：";
        safeInputInt("", choice);
        switch (choice) {
            case 1:
                ExceptionHelper::runWithCatch([&]() {
                    std::cout << "图书列表：\n";
                    for (const auto& b : bookManager.books)
                        std::cout << b.bookId << " | " << b.title << " | " << b.author
                                  << " | " << b.availableCount << "/" << b.totalCount << std::endl;
                }, "展示书籍列表");
                break;
            case 2: {
                std::string bookId, title, author;
                int total;
                std::cout << "Book ID: "; std::cin >> bookId;
                std::cout << "Title: "; std::cin >> title;
                std::cout << "Author: "; std::cin >> author;
                safeInputInt("Total Count: ", total);
                ExceptionHelper::runWithCatch([&]() {
                    bookManager.addBook(Book(bookId, title, author, total, total));
                    std::cout << "添加成功。\n";
                }, "添加图书");
                break;
            }
            case 3: {
                std::string bid;
                std::cout << "请输入要删除的图书ID：";
                std::cin >> bid;
                ExceptionHelper::runWithCatch([&]() {
                    if (bookManager.removeBook(bid)) {
                        std::cout << "删除成功！\n";
                    } else {
                        throw std::runtime_error("没有找到该书。");
                    }
                }, "删除图书");
                break;
            }
            case 4:
                ExceptionHelper::runWithCatch([&]() {
                    std::cout << "所有用户：\n";
                    for (const auto& u : userManager.users) {
                        std::cout << u.userId << " | " << u.username << " | "
                                  << (u.role == Role::Admin ? "管理员" : (u.isVIP ? "VIP读者" : "普通读者"))
                                  << " | 余额：" << u.balance << std::endl;
                    }
                }, "查看所有用户");
                break;
            case 5: {
                std::string uid;
                std::cout << "请输入要删除的用户ID：";
                std::cin >> uid;
                ExceptionHelper::runWithCatch([&]() {
                    if (userManager.removeUser(uid)) {
                        std::cout << "删除成功！\n";
                    } else {
                        throw std::runtime_error("未找到该用户。");
                    }
                }, "删除用户");
                break;
            }
            case 6:
                ExceptionHelper::runWithCatch([&]() {
                    std::cout << "所有借阅记录：\n";
                    for (const auto& r : borrowManager.records) {
                        std::cout << "记录号：" << r.recordId
                                << " | 用户ID：" << r.userId
                                << " | 图书ID：" << r.bookId
                                << " | 借出日期：" << r.borrowDate
                                << " | 到期日：" << r.dueDate
                                << " | 归还日期：" << (r.returnDate.empty() ? "未归还" : r.returnDate)
                                << std::endl;
                    }
                }, "查看所有借阅记录");
                break;
            case 0:
                break;
            default:
                std::cout << "无效选项。\n";
        }
    } while (choice != 0);
}
