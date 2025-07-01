#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

struct Book {
    std::string bookId;
    std::string title;
    std::string author;
    int totalCount;
    int availableCount;
    Book() = default;
    Book(const std::string& id, const std::string& t, const std::string& a, int total, int avail)
        : bookId(id), title(t), author(a), totalCount(total), availableCount(avail) {}
};

class BookManager {
public:
    std::vector<Book> books;
    void load(const std::string& filename) {
        books.clear();
        std::ifstream fin(filename);
        if (!fin.is_open()) throw std::ios_base::failure("无法打开图书数据文件: " + filename);
        std::string line;
        while (getline(fin, line)) {
            std::istringstream iss(line);
            std::string id, t, a, total, avail;
            getline(iss, id, ',');
            getline(iss, t, ',');
            getline(iss, a, ',');
            getline(iss, total, ',');
            getline(iss, avail, ',');
            books.emplace_back(id, t, a, std::stoi(total), std::stoi(avail));
        }
        fin.close();
    }
    void save(const std::string& filename) {
        std::ofstream fout(filename);
        if (!fout.is_open()) throw std::ios_base::failure("无法写入图书数据文件: " + filename);
        for (const auto& b : books) {
            fout << b.bookId << "," << b.title << "," << b.author << "," << b.totalCount << "," << b.availableCount << "\n";
        }
        fout.close();
    }
    void addBook(const Book& book) {
        books.push_back(book);
    }
    bool removeBook(const std::string& bookId) {
        auto it = std::remove_if(books.begin(), books.end(), [&](const Book& b){ return b.bookId == bookId; });
        if(it != books.end()) {
            books.erase(it, books.end());
            return true;
        }
        return false;
    }
    Book* findBook(const std::string& bookId) {
        for (auto& b : books) {
            if (b.bookId == bookId) return &b;
        }
        return nullptr;
    }
};
