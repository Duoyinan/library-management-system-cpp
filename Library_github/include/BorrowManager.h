#pragma once
#include "BorrowRecord.h"
#include "BookManager.h"
#include <vector>
#include <fstream>
#include <ctime>

class BorrowManager {
public:
    std::vector<BorrowRecord> records;

    void load(const std::string& filename) {
        records.clear();
        std::ifstream fin(filename);
        std::string line;
        while (getline(fin, line)) {
            records.push_back(BorrowRecord::fromString(line));
        }
        fin.close();
    }

    void save(const std::string& filename) {
        std::ofstream fout(filename);
        for (const auto& r : records) {
            fout << r.toString() << std::endl;
        }
        fout.close();
    }

    std::string getCurrentDate() {
        time_t t = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
        return buf;
    }

    bool borrowBook(const std::string& rid, const std::string& uid, BookManager& bm, const std::string& bookId) {
        Book* book = bm.findBook(bookId);
        if (book && book->availableCount > 0) {
            book->availableCount--;
            records.emplace_back(rid, uid, bookId, getCurrentDate(), "");
            return true;
        }
        return false;
    }

    bool returnBook(const std::string& uid, BookManager& bm, const std::string& bookId) {
        for (auto& r : records) {
            if (r.userId == uid && r.bookId == bookId && r.returnDate.empty()) {
                r.returnDate = getCurrentDate();
                Book* book = bm.findBook(bookId);
                if (book) book->availableCount++;
                return true;
            }
        }
        return false;
    }
};
