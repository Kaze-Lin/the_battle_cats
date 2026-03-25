#ifndef STAGE_SELECTION_HPP
#define STAGE_SELECTION_HPP

#include "BackgroundImage.hpp"

class StageSelection {
private:
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

public:
    std::vector<std::shared_ptr<Util::GameObject>> m_stageSelection;

    StageSelection();

    void Update();
};

#endif //STAGE_SELECTION_HPP