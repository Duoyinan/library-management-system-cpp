#pragma once
#include <string>
#include <iostream>

class Book {
public:
    std::string bookId;
    std::string title;
    std::string author;
    int totalCount;
    int availableCount;

    Book() = default;
    Book(const std::string& id, const std::string& t, const std::string& a, int total, int avail)
        : bookId(id), title(t), author(a), totalCount(total), availableCount(avail) {}

    std::string toString() const {
        return bookId + "," + title + "," + author + "," + std::to_string(totalCount) + "," + std::to_string(availableCount);
    }

    static Book fromString(const std::string& s) {
        Book b;
        size_t p1 = s.find(','), p2 = s.find(',', p1 + 1), p3 = s.find(',', p2 + 1), p4 = s.find(',', p3 + 1);
        b.bookId = s.substr(0, p1);
        b.title = s.substr(p1 + 1, p2 - p1 - 1);
        b.author = s.substr(p2 + 1, p3 - p2 - 1);
        b.totalCount = std::stoi(s.substr(p3 + 1, p4 - p3 - 1));
        b.availableCount = std::stoi(s.substr(p4 + 1));
        return b;
    }
};
