#pragma once

#include <string>

namespace notex::msg {
    void error(const std::string& message);
    void info(const std::string& message);

    void throwGLFWError();
    void checkGLFWError();
}