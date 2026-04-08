#ifndef PROPS_STORE_HPP
#define PROPS_STORE_HPP

#include "Phase.hpp"
#include "Component/BackgroundImage.hpp"
#include "Component/Button.hpp"

class PropsStore: public Phase {
private:
    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_PropsStoreBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;


    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Back;
    std::shared_ptr<Button> m_b_CatCan;

public:

    std::shared_ptr<Phase> GetDestinationPhase() override;

    PropsStore();
    ~PropsStore() override = default;
    void GoBack();
};

#endif //PROPS_STORE_HPP