#include "Phase/Login.hpp"
#include "iostream"

Login::Login(): Phase() {
    m_Prompt =
        std::make_shared<Text>(
            60,
            "Please Enter Your Account In Terminal_",
            0);

    // layout
    m_Prompt->Place({0.0F, 0.0F});
    AddChild(m_Prompt);

    m_tb_ToBeContinue =
        std::make_shared<TextButton>(
            32,
            "Continue..",
            0,
            [this](){this->ToCatbase();}
            );

    // layout
    m_tb_ToBeContinue->Place({473,-120});
}

void Login::Update() {
    Phase::Update();

    if (!m_hasEnteredName) {
        std::cout << "Please enter your account in the terminal: ";
        std::cin >> AccountName;

        if (!AccountName.empty()) {
            m_hasEnteredName = true;
            LOG_DEBUG("Welcome " + AccountName);
            AddChild(m_tb_ToBeContinue);
        }
    }
}

std::shared_ptr<Phase> Login::GetDestinationPhase() {
    return PhaseManager::GetNextPhase("Login", this->m_DestinationPhase);
}

void Login::ToCatbase() {
    this->m_DestinationPhase = "CatBase";
}
