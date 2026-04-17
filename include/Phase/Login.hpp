#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Phase.hpp"
#include "Component/Text.hpp"
#include "SystemInfo.hpp"

class Login: public Phase {
private:
    std::shared_ptr<Text> m_Prompt;

public:
    Login();
    ~Login() override = default;
};

#endif //LOGIN_HPP
