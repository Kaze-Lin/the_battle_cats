#ifndef UPGRADE_HPP
#define UPGRADE_HPP

#include "Phase.hpp"
#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "../Component/Block/UpgradeBlock.hpp"
#include "ImageScaling.hpp"
#include "Component/ScrollManager.hpp"
#include "Component/TwoLayerText.hpp"
#include "Phase/CatBase.hpp"
#include "Component/ResourceDisplay.hpp"

class Upgrade: public Phase {
private:
    ScrollManager<UpgradeBlock> m_ScrollManager;
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_UpgradeBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;
    std::shared_ptr<BackgroundImage> m_SubTitleBanner;

    std::shared_ptr<TwoLayerText> m_SubTitleText;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;
    std::shared_ptr<Button> m_b_Upgrade;
    std::shared_ptr<Button> m_b_XP;


    // upgrade selections
    std::vector<std::shared_ptr<UpgradeBlock>> m_UpgradeSelectionBar;
    void BuildSelectionBar();

    std::shared_ptr<ResourceDisplay> m_ResourceDisplay;


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