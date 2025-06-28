#pragma once
#include <string>
#include <iostream>

enum class Role { User, Admin };

class User {
public:
    std::string userId;
    std::string username;
    std::string password;
    Role role;

    User() = default;
    User(const std::string& id, const std::string& name, const std::string& pwd, Role r)
        : userId(id), username(name), password(pwd), role(r) {}

    std::string toString() const {
        return userId + "," + username + "," + password + "," + std::to_string(static_cast<int>(role));
    }

    static User fromString(const std::string& s) {
        User u;
        size_t pos1 = s.find(','), pos2 = s.find(',', pos1 + 1), pos3 = s.find(',', pos2 + 1);
        u.userId = s.substr(0, pos1);
        u.username = s.substr(pos1 + 1, pos2 - pos1 - 1);
        u.password = s.substr(pos2 + 1, pos3 - pos2 - 1);
        u.role = (s.substr(pos3 + 1) == "1") ? Role::Admin : Role::User;
        return u;
    }
};
