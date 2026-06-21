#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "Phase.hpp"
#include "Component/TwoLayerText.hpp"
#include "Component/TextButton.hpp"
#include "SystemInfo.hpp"
#include "PhaseManager.hpp"
#include "UserManager.hpp"

#include "Util/BGM.hpp"
#include "Util/SFX.hpp"

class Login: public Phase{
private:
    std::shared_ptr<Util::BGM> m_BGM;
    std::shared_ptr<Util::SFX> m_PressButton;
    std::shared_ptr<TwoLayerText> m_Prompt;

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
