#pragma once
#include <string>
#include <vector>
#include "BorrowRecord.h"

class User {
public:
    std::string userId;
    std::string username;
    std::string password;
    int role; // 0:普通用户 1:管理员
    std::vector<BorrowRecord> borrowHistory;

    User();
    User(const std::string& id, const std::string& name, const std::string& pwd, int role);

    bool login(const std::string& name, const std::string& pwd);
    void registerUser();
    // 其他方法...
};
