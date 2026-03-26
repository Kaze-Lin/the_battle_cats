#ifndef STAGE_SELECTION_HPP
#define STAGE_SELECTION_HPP

#include "../Component/BackgroundImage.hpp"
#include "Phase.hpp"

class StageSelection: public Phase {
private:
    std::shared_ptr<BackgroundImage> m_BackgroundImage;

public:


    StageSelection();

    void Update();
};

#endif //STAGE_SELECTION_HPP