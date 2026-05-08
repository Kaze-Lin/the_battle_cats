#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Phase.hpp"
#include "Component/Text.hpp"
#include "Component/TextButton.hpp"
#include "SystemInfo.hpp"
#include "PhaseManager.hpp"
#include "UserManager.hpp"

class Login: public Phase{
private:
    std::shared_ptr<Text> m_Prompt;

    std::shared_ptr<TextButton> m_tb_ToBeContinue;

    int LoginSelector = 0;
    std::string AccountName = "";
    std::string Password = "";

    bool m_hasEnteredName = false;
    bool m_hasEnteredPassword = false;
    std::shared_ptr<Phase> m_NextPhase = nullptr;

public:
    Login();
    ~Login() override = default;

    std::shared_ptr<Phase> GetDestinationPhase() override;

    void Update() override;
    void ToCatbase();
};

#endif //LOGIN_HPP
