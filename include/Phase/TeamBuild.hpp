#ifndef TEAM_BUILD_HPP
#define TEAM_BUILD_HPP

#include "Phase.hpp"
#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/ScrollManager.hpp"
#include "Component/Block/DeployBlock.hpp"

class TeamBuild: public Phase {
public:
    TeamBuild();

    std::shared_ptr<Phase> GetDestinationPhase() override;

    void GoBack(); // this is for go back
    void ToPropsStore();
    void Update() override;


private:
    ScrollManager<DeployBlock> m_ScrollManager;
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_TeamBuildBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;
    std::shared_ptr<BackgroundImage> m_SubTitleBanner;

    std::shared_ptr<Text> m_SubTitleText;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;
    std::shared_ptr<Button> m_b_Upgrade;


    // upgrade selections
    std::vector<std::shared_ptr<DeployBlock>> m_CatSelectionBar;
    void BuildSelectionBar();

};

#endif //TEAM_BUILD_HPP
