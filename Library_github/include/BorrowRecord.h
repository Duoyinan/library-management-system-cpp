#pragma once
#include <string>
#include <ctime>
#include <iostream>

class BorrowRecord {
public:
    std::string recordId;
    std::string userId;
    std::string bookId;
    std::string borrowDate;
    std::string returnDate;

    BorrowRecord() = default;
    BorrowRecord(const std::string& rid, const std::string& uid, const std::string& bid, const std::string& bdate, const std::string& rdate)
        : recordId(rid), userId(uid), bookId(bid), borrowDate(bdate), returnDate(rdate) {}

    std::string toString() const {
        return recordId + "," + userId + "," + bookId + "," + borrowDate + "," + returnDate;
    }

    static BorrowRecord fromString(const std::string& s) {
        BorrowRecord br;
        size_t p1 = s.find(','), p2 = s.find(',', p1 + 1), p3 = s.find(',', p2 + 1), p4 = s.find(',', p3 + 1);
        br.recordId = s.substr(0, p1);
        br.userId = s.substr(p1 + 1, p2 - p1 - 1);
        br.bookId = s.substr(p2 + 1, p3 - p2 - 1);
        br.borrowDate = s.substr(p3 + 1, p4 - p3 - 1);
        br.returnDate = s.substr(p4 + 1);
        return br;
    }
};
