#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

enum class Role { Reader, Admin };

struct User {
    std::string userId;
    std::string username;
    std::string password;
    Role role;
    bool isVIP;
    double balance;
    User() : isVIP(false), balance(0.0) {}
    User(const std::string& id, const std::string& name, const std::string& pwd, Role r, bool vip = false, double bal = 0.0)
        : userId(id), username(name), password(pwd), role(r), isVIP(vip), balance(bal) {}
};

class UserManager {
public:
    std::vector<User> users;

    UserManager() {
        if (users.empty()) {
            users.push_back(User("A001", "admin", "123456", Role::Admin, false, 0.0));
        }
    }
    void load(const std::string& filename) {
        users.clear();
        std::ifstream fin(filename);
        if (!fin.is_open()) throw std::ios_base::failure("无法打开用户数据文件: " + filename);
        std::string line;
        while (getline(fin, line)) {
            std::istringstream iss(line);
            std::string id, name, pwd, r, vip, bal;
            getline(iss, id, ',');
            getline(iss, name, ',');
            getline(iss, pwd, ',');
            getline(iss, r, ',');
            getline(iss, vip, ',');
            getline(iss, bal, ',');
            Role role = (r == "Admin") ? Role::Admin : Role::Reader;
            bool isVIP = (vip == "1");
            double balance = bal.empty() ? 0.0 : std::stod(bal);
            users.emplace_back(id, name, pwd, role, isVIP, balance);
        }
        fin.close();
        bool hasAdmin = false;
        for (auto& u : users) {
            if (u.role == Role::Admin) hasAdmin = true;
        }
        if (!hasAdmin) users.push_back(User("A001", "admin", "123456", Role::Admin, false, 0.0));
    }
    void save(const std::string& filename) {
        std::ofstream fout(filename);
        if (!fout.is_open()) throw std::ios_base::failure("无法写入用户数据文件: " + filename);
        for (const auto& u : users) {
            fout << u.userId << "," << u.username << "," << u.password << "," 
                << (u.role == Role::Admin ? "Admin" : "Reader") << ","
                << (u.isVIP ? "1" : "0") << ","
                << u.balance << "\n";
        }
        fout.close();
    }
    User* login(const std::string& username, const std::string& password, Role role) {
        for (auto& u : users) {
            if (u.username == username && u.password == password && u.role == role) return &u;
        }
        throw std::runtime_error("未找到该用户或密码错误。");
    }
    bool addUser(const std::string& username, const std::string& password, Role role, bool vip = false) {
        for (const auto& u : users) {
            if (u.username == username && u.role == role) return false;
        }
        std::string id = (role == Role::Admin ? "A" : "U") + std::to_string(1000 + users.size());
        users.emplace_back(id, username, password, role, vip, 0.0);
        return true;
    }
    bool removeUser(const std::string& userId) {
        auto it = std::remove_if(users.begin(), users.end(), [&](const User& u){ return u.userId == userId; });
        if (it != users.end()) {
            users.erase(it, users.end());
            return true;
        }
        return false;
    }
    User* findById(const std::string& userId) {
        for (auto& u : users)
            if (u.userId == userId) return &u;
        return nullptr;
    }
};
