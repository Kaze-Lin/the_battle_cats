#ifndef FIGHT_HPP
#define FIGHT_HPP

#include "Phase/Phase.hpp"

#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"

#include "ImageScaling.hpp"
#include "SystemInfo.hpp"
#include "Component/Text.hpp"

class Fight: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Pause;
    std::shared_ptr<Button> m_b_RickUpgrade;
    std::shared_ptr<Button> m_b_CannonCharge;

    // stage name
    std::shared_ptr<Text> m_StageName;

    float m_timeScale = 1.0f;

    bool m_isGameOver = false;

public:

    Fight();
    ~Fight() override = default;

    void Update() override;
};

#endif //FIGHT_HPP
