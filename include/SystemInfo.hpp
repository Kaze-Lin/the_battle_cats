#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP

#include "Core/Context.hpp"

namespace System {
    inline float GetWindowWidth() {
        return static_cast<float>(Core::Context::GetInstance()->GetWindowWidth());
    }
    inline float GetWindowHeight() {
        return static_cast<float>(Core::Context::GetInstance()->GetWindowHeight());
    }
} // namespace System



#endif //SYSTEM_INFO_HPP