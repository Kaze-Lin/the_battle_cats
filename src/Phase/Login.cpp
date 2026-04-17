#include "Phase/Login.hpp"

Login::Login(): Phase() {
    m_Prompt =
        std::make_shared<Text>(
            60,
            "Please Enter Your Account In Terminal_",
            0);

    // layout
    m_Prompt->Place({0.0F, 0.0F});
    AddChild(m_Prompt);
}