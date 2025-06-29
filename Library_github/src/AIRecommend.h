#pragma once
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <json/json.h> // 需安装jsoncpp

struct BookInfo {
    std::string id, title, author;
};

class AIRecommend {
public:
    static std::vector<BookInfo> recommendBooks(const std::string& username, const std::string& mode="auto") {
        std::vector<BookInfo> recBooks;
        std::string cmd = "python3 recommend.py " + username + " " + mode;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) return recBooks;
        std::string jsonStr;
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe)) jsonStr += buffer;
        pclose(pipe);

        Json::Value root;
        Json::Reader reader;
        if (reader.parse(jsonStr, root) && root.isArray()) {
            for (const auto& item : root) {
                BookInfo b;
                b.id = item["id"].asString();
                b.title = item["title"].asString();
                b.author = item["author"].asString();
                recBooks.push_back(b);
            }
        }
        return recBooks;
    }
};
