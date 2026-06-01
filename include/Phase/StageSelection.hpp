#ifndef STAGE_SELECTION_HPP
#define STAGE_SELECTION_HPP

#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"
#include "Component/OptionBlock.hpp"

#include "ImageScaling.hpp"

#include "Component/ScrollManager.hpp"
#include "Phase.hpp"
#include "Component/StageBlock.hpp"

class StageSelection: public Phase {
private:
    ScrollManager<StageBlock> m_ScrollManager;

    int m_CurrentChapter = 1;
    int m_CurrentStage = 1;

    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_StageSelectionBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Start;
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;

    std::vector<std::shared_ptr<StageBlock>> m_StageSelectionBar;
    void BuildSelectionBar();

public:

    StageSelection();
    ~StageSelection() override = default;

    std::shared_ptr<Phase> GetDestinationPhase() override;

    void Update() override;

    void ToFight();
    void ToPropsStore();
    void GoBack();
};

#endif //STAGE_SELECTION_HPP