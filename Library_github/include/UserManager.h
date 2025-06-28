#pragma once
#include "User.h"
#include <vector>
#include <fstream>

class UserManager {
public:
    std::vector<User> users;

    void load(const std::string& filename) {
        users.clear();
        std::ifstream fin(filename);
        std::string line;
        while (getline(fin, line)) {
            users.push_back(User::fromString(line));
        }
        fin.close();
    }

    void save(const std::string& filename) {
        std::ofstream fout(filename);
        for (const auto& u : users) {
            fout << u.toString() << std::endl;
        }
        fout.close();
    }

    bool registerUser(const std::string& id, const std::string& name, const std::string& pwd, Role r) {
        for (const auto& u : users) if (u.username == name) return false;
        users.emplace_back(id, name, pwd, r);
        return true;
    }

    User* login(const std::string& name, const std::string& pwd) {
        for (auto& u : users) {
            if (u.username == name && u.password == pwd)
                return &u;
        }
        return nullptr;
    }
};
