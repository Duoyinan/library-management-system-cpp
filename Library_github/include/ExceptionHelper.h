#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

class ExceptionHelper {
public:
    template <typename Func>
    static void runWithCatch(const Func& f, const std::string& context = "") {
        try {
            f();
        } catch (const std::ios_base::failure& e) {
            logException(context, e.what());
            std::cerr << "[文件错误] " << context << ": " << e.what() << std::endl;
        } catch (const std::runtime_error& e) {
            logException(context, e.what());
            std::cerr << "[操作失败] " << context << ": " << e.what() << std::endl;
        } catch (const std::exception& e) {
            logException(context, e.what());
            std::cerr << "[异常] " << context << ": " << e.what() << std::endl;
        } catch (...) {
            logException(context, "未知异常");
            std::cerr << "[未知异常] " << context << std::endl;
        }
    }
private:
    static void logException(const std::string& context, const std::string& msg) {
        std::ofstream fout("error.log", std::ios::app);
        fout << "[Error] " << context << ": " << msg << std::endl;
        fout.close();
    }
};
