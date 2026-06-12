#include "Phase/Login.hpp"
#include "iostream"
#include <limits>

Login::Login(): Phase(){
    m_Prompt =
        std::make_shared<TwoLayerText>(
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

    if (!m_hasEnteredName and !m_hasEnteredPassword) {
        std::cout << "Please select an option. Press 1 to register an account or press 2 to login your account: ";
        std::cin >> LoginSelector;
        bool success = false;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            LOG_WARN("Invalid input! Please enter number 1 or 2!");
            return;
        }
        if (LoginSelector == 1) {
            std::cout << "Please enter your account in the terminal: ";
            std::cin >> AccountName;
            std::cout << "Please enter your password: ";
            std::cin >> Password;

            success = UserManager::GetInstance().Register(AccountName, Password);
            if (!success) {
                AccountName = "";
                Password = "";
            }
            else {
                UserManager::GetInstance().Login(AccountName, Password);
            }
        }
        else if (LoginSelector == 2) {
            std::cout << "Please enter your account in the terminal: ";
            std::cin >> AccountName;
            std::cout << "Please enter your password: ";
            std::cin >> Password;

            success = UserManager::GetInstance().Login(AccountName, Password);
            if (!success) {
                LOG_WARN("Login failed. Incorrect username or password!");
                AccountName = "";
                Password = "";
            }
        }

        else {
            LOG_WARN("Please select a valid option!") ;
            Login::Update();
        }

        if (success and UserManager::GetInstance().GetCurrentUser() != nullptr) {
            m_hasEnteredName = true;
            m_hasEnteredPassword = true;
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
