#pragma once
#include <string>

class BorrowRecord {
public:
    std::string recordId;
    std::string userId;
    std::string bookId;
    std::string borrowDate;
    std::string returnDate;
    bool isReturned;

    BorrowRecord();
    BorrowRecord(const std::string& recordId, const std::string& userId, const std::string& bookId,
                 const std::string& borrowDate, const std::string& returnDate, bool isReturned);

    void display() const;
};
