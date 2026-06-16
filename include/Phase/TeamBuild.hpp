#ifndef TEAM_BUILD_HPP
#define TEAM_BUILD_HPP

#include "Phase.hpp"
#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/ScrollManager.hpp"
#include "Component/Block/DeployBlock.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"
#include "Component/ResourceDisplay.hpp"
#include "Component/TwoLayerText.hpp"

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

    std::shared_ptr<TwoLayerText> m_SubTitleText;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;
    std::shared_ptr<Button> m_b_Upgrade;


    // upgrade selections
    std::vector<std::shared_ptr<DeployBlock>> m_CatSelectionBar;
    void BuildSelectionBar();

    std::shared_ptr<ResourceDisplay> m_ResourceDisplay;

    // === Drag and Drop state ===
    std::shared_ptr<DeployBlock> m_PressedBlock = nullptr;
    int m_PressedGridIndex = -1; // -1 means not from top grid
    
    glm::vec2 m_PressStartPos = {0.0F, 0.0F};
    Util::ms_t m_PressStartTime = 0;
    bool m_IsDraggingBlock = false;
    std::shared_ptr<BackgroundImage> m_DragGhost = nullptr;

    bool IsBlockHovered(const std::shared_ptr<DeployBlock>& block);

    // === Grid State ===
    std::array<int, 10> m_CurrentTeam;
    std::array<std::shared_ptr<BackgroundImage>, 10> m_TeamImages;
    std::array<std::array<glm::vec2, 2>, 10> m_BlocksRange;
    
    void RefreshTeamImages();
    int FindNearestEmptySlot(int target_index);
    void CalculateBlocksRange();
    
};

#endif //TEAM_BUILD_HPP
