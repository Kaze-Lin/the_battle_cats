#include "Phase/Fight.hpp"

Fight::Fight(): Phase() {
    m_BackgroundImage =
        std::make_shared<BackgroundImage>(
            RESOURCE_DIR "/stage_background/Bg000.png",
            -10.0F);
    AddChild(m_BackgroundImage);

    m_BackgroundImage->AlignWithWindow();
}