#pragma once
#include <string>

class User {
public:
    std::string userId;
    std::string username;
    std::string password;
    int role; // 0=普通用户，1=管理员

    User();
    User(const std::string& id, const std::string& name, const std::string& pwd, int role);

    bool login(const std::string& username, const std::string& password);
    void display() const;
};
