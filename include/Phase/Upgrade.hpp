#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "Phase.hpp"
#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "ImageScaling.hpp"
#include "Phase/CatBase.hpp"

class Upgrade: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_UpgradeBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;


public:

    std::shared_ptr<Phase> GetDestinationPhase() override;

    Upgrade();
    ~Upgrade() override = default;
    void GoBack(); // this is for go back
    void ToPropsStore();
};

#endif //UPGRADE_HPP