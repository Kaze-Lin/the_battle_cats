#ifndef FIGHT_HPP
#define FIGHT_HPP

#include "Phase/Phase.hpp"

#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"

#include "ImageScaling.hpp"
#include "SystemInfo.hpp"

class Fight: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Pause;
    std::shared_ptr<Button> m_b_test;

public:

    Fight();
    ~Fight() override = default;
};

#endif //FIGHT_HPP
