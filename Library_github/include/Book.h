#pragma once
#include <string>

class Book {
public:
    std::string id;
    std::string title;
    std::string author;
    std::string publisher;
    int totalCount;
    int availableCount;

    Book();
    Book(const std::string& id, const std::string& title, const std::string& author,
         const std::string& publisher, int totalCount);

    void display() const;
};
