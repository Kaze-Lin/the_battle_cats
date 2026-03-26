#include "Phase/StageSelection.hpp"

StageSelection::StageSelection(): Phase() {
    // background image (without interaction image)
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/background/EOC_map.jpg",
            -10.0F);
    AddChild(m_BackgroundImage);

    // button image (with interaction image)



    // --- Layout settings ---
    // background image (without interaction image)
    m_BackgroundImage->AlignWithWindow();
}

void StageSelection::Update() {
    return;
}
