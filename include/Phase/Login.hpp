#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Phase.hpp"
#include "Component/Text.hpp"
#include "Component/TextButton.hpp"
#include "SystemInfo.hpp"
#include "PhaseManager.hpp"

class Login: public Phase {
private:
    std::shared_ptr<Text> m_Prompt;

    std::shared_ptr<TextButton> m_tb_ToBeContinue;

    std::string AccountName = "";

    bool m_hasEnteredName = false;
    std::shared_ptr<Phase> m_NextPhase = nullptr;

public:
    Login();
    ~Login() override = default;

    std::shared_ptr<Phase> GetDestinationPhase() override;

    void Update() override;
    void ToCatbase();
};

#endif //LOGIN_HPP
