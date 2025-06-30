#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

class ExceptionHelper {
public:
    // 统一捕获异常并输出错误信息
    template <typename Func>
    static void runWithCatch(const Func& f, const std::string& context = "") {
        try {
            f();
        } catch (const std::ios_base::failure& e) {
            std::cerr << "[IO Error] " << context << ": " << e.what() << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "[Runtime Error] " << context << ": " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[Exception] " << context << ": " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "[Unknown Error] " << context << std::endl;
        }
    }
};
