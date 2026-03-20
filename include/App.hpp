#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "BackgroundImage.hpp"
#include "Button.hpp"
#include "ImageScaling.hpp"
#include "SystemInfo.hpp"
#include "Util/Renderer.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    Util::Renderer m_Root;

    State m_CurrentState = State::START;

    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_LobbyBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;
    std::shared_ptr<BackgroundImage> m_PlayerLevel;
    std::shared_ptr<BackgroundImage> m_CatBaseCatFace;

    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Start;
    std::shared_ptr<Button> m_b_Upgrade;
    std::shared_ptr<Button> m_b_TeamBuild;
    std::shared_ptr<Button> m_b_RestoreIcon;
    std::shared_ptr<Button> m_b_NormalGachaIcon;
    std::shared_ptr<Button> m_SpecialGachaIcon;
    std::shared_ptr<Button> m_PropsStore;
    std::shared_ptr<Button> m_Back;
};

#endif
