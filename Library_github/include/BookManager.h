#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "BookManager.h"
struct BorrowRecord {
    std::string recordId;
    std::string userId;
    std::string bookId;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    BorrowRecord() = default;
    BorrowRecord(const std::string& rid, const std::string& uid, const std::string& bid,
                 const std::string& bdate, const std::string& ddate)
        : recordId(rid), userId(uid), bookId(bid), borrowDate(bdate), dueDate(ddate), returnDate("") {}
};

inline std::string getToday() {
    time_t t = std::time(nullptr);
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}

inline std::string getFutureDate(const std::string& date, int days) {
    std::tm tm = {};
    std::istringstream ss(date);
    if (!(ss >> std::get_time(&tm, "%Y-%m-%d"))) {
        throw std::runtime_error("日期格式错误，应为 YYYY-MM-DD");
    }
    time_t t = mktime(&tm);
    t += 60*60*24*days;
    std::tm* ntm = localtime(&t);
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ntm);
    return buf;
}

inline int dateDiff(const std::string& date1, const std::string& date2) {
    std::tm tm1 = {}, tm2 = {};
    std::istringstream ss1(date1), ss2(date2);
    if (!(ss1 >> std::get_time(&tm1, "%Y-%m-%d")) || !(ss2 >> std::get_time(&tm2, "%Y-%m-%d"))) {
        throw std::runtime_error("日期格式错误，应为 YYYY-MM-DD");
    }
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    return static_cast<int>((t2 - t1) / (60*60*24));
}

class BorrowManager {
public:
    std::vector<BorrowRecord> records;

    void load(const std::string& filename) {
        records.clear();
        std::ifstream fin(filename);
        if (!fin.is_open()) throw std::ios_base::failure("无法打开借阅数据文件: " + filename);
        std::string line;
        while (getline(fin, line)) {
            std::istringstream iss(line);
            std::string rid, uid, bid, bdate, ddate, rdate;
            getline(iss, rid, ',');
            getline(iss, uid, ',');
            getline(iss, bid, ',');
            getline(iss, bdate, ',');
            getline(iss, ddate, ',');
            getline(iss, rdate, ',');
            BorrowRecord rec;
            rec.recordId = rid;
            rec.userId = uid;
            rec.bookId = bid;
            rec.borrowDate = bdate;
            rec.dueDate = ddate;
            rec.returnDate = rdate;
            records.push_back(rec);
        }
        fin.close();
    }
    void save(const std::string& filename) {
        std::ofstream fout(filename);
        if (!fout.is_open()) throw std::ios_base::failure("无法写入借阅数据文件: " + filename);
        for (const auto& r : records) {
            fout << r.recordId << "," << r.userId << "," << r.bookId << "," << r.borrowDate << "," << r.dueDate << "," << r.returnDate << "\n";
        }
        fout.close();
    }

    bool hasOverdueUnreturned(const std::string& userId) {
        std::string today = getToday();
        for (const auto& r : records) {
            if (r.userId == userId && r.returnDate.empty()) {
                if (today > r.dueDate)
                    return true;
            }
        }
        return false;
    }

    int currentBorrowedCount(const std::string& userId) {
        int cnt = 0;
        for (const auto& r : records)
            if (r.userId == userId && r.returnDate.empty())
                cnt++;
        return cnt;
    }

    bool borrowBook(const std::string& recordId, User& user, BookManager& bookManager, const std::string& bookId) {
        if (user.balance < 0) throw std::runtime_error("您的余额小于0，无法借书，请充值。");
        if (hasOverdueUnreturned(user.userId)) throw std::runtime_error("您有超期未还书籍，无法借书，请先归还。");
        int limit = user.isVIP ? 10 : 5;
        if (currentBorrowedCount(user.userId) >= limit) throw std::runtime_error("已达借书上限，无法借书。");
        Book* book = bookManager.findBook(bookId);
        if (!book) throw std::runtime_error("没有找到该书。");
        if (book->availableCount <= 0) throw std::runtime_error("图书已无可借库存。");
        std::string today = getToday();
        std::string due = getFutureDate(today, user.isVIP ? 30 : 15);
        book->availableCount -= 1;
        records.emplace_back(recordId, user.userId, bookId, today, due);
        return true;
    }

    bool returnBook(User& user, BookManager& bookManager, const std::string& bookId) {
        for (auto& r : records) {
            if (r.userId == user.userId && r.bookId == bookId && r.returnDate.empty()) {
                Book* book = bookManager.findBook(bookId);
                if (!book) throw std::runtime_error("没有找到该书。");
                book->availableCount += 1;
                r.returnDate = getToday();
                if (r.returnDate > r.dueDate) {
                    int days = dateDiff(r.dueDate, r.returnDate);
                    double fine = days * (user.isVIP ? 4.0 : 8.0);
                    user.balance -= fine;
                    std::cout << "归还超期，已自动扣除罚款 " << fine << " 元，当前余额：" << user.balance << std::endl;
                }
                return true;
            }
        }
        throw std::runtime_error("没有找到待归还的该书借阅记录。");
    }
};
