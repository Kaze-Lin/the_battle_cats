#ifndef STAGE_SELECTION_HPP
#define STAGE_SELECTION_HPP

#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"

#include "ImageScaling.hpp"

#include "Phase.hpp"

class StageSelection: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_StageSelectionBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Start;
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;

public:

    StageSelection();
    ~StageSelection() override = default;

    std::shared_ptr<Phase> GetDestinationPhase() override;

    void ToFight();
    void ToPropsStore();
    void GoBack();
};

#endif //STAGE_SELECTION_HPP