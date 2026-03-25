#include "Phase/StageSelection.hpp"

StageSelection::StageSelection() {
    // background image (without interaction image)
    m_BackgroundImage = std::make_shared<BackgroundImage>(RESOURCE_DIR "/lobby/Img009_3.png", -10.0F);
    m_stageSelection.push_back(m_BackgroundImage);


    // button image (with interaction image)



    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();
}

void StageSelection::Update() {
    return;
}
