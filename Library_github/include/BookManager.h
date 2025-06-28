#pragma once
#include "Book.h"
#include <vector>
#include <fstream>

class BookManager {
public:
    std::vector<Book> books;

    void load(const std::string& filename) {
        books.clear();
        std::ifstream fin(filename);
        std::string line;
        while (getline(fin, line)) {
            books.push_back(Book::fromString(line));
        }
        fin.close();
    }

    void save(const std::string& filename) {
        std::ofstream fout(filename);
        for (const auto& b : books) {
            fout << b.toString() << std::endl;
        }
        fout.close();
    }

    void addBook(const Book& book) {
        books.push_back(book);
    }

    bool removeBook(const std::string& bookId) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->bookId == bookId) {
                books.erase(it);
                return true;
            }
        }
        return false;
    }

    Book* findBook(const std::string& bookId) {
        for (auto& b : books)
            if (b.bookId == bookId)
                return &b;
        return nullptr;
    }
};
