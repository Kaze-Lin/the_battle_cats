#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "Phase.hpp"
#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/UpgradeBlock.hpp"
#include "ImageScaling.hpp"
#include "Component/Text.hpp"
#include "Phase/CatBase.hpp"

class Upgrade: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_UpgradeBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;
    std::shared_ptr<BackgroundImage> m_SubTitleBanner;

    std::shared_ptr<Text> m_SubTitleText;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;
    std::shared_ptr<Button> m_b_Upgrade;


    // upgrade selections
    std::vector<std::shared_ptr<UpgradeBlock>> m_UpgradeSelectionBar;
    void BuildSelectionBar();


public:

    Upgrade();
    ~Upgrade() override = default;

    std::shared_ptr<Phase> GetDestinationPhase() override;

    void GoBack(); // this is for go back
    void ToPropsStore();
    void Update() override;

    void UpgradeLevel();

};

#endif //UPGRADE_HPP