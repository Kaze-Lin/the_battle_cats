#ifndef CAT_BASE_HPP
#define CAT_BASE_HPP

#include "BackgroundImage.hpp"
#include "Button.hpp"
#include "ImageScaling.hpp"
#include "Util/Renderer.hpp"

class CatBase {
private:


    // background image (without interaction image)
    std::shared_ptr<BackgroundImage> m_BackgroundImage;
    std::shared_ptr<BackgroundImage> m_LobbyBanner;
    std::shared_ptr<BackgroundImage> m_BottomBanner;
    std::shared_ptr<BackgroundImage> m_PlayerLevel;
    std::shared_ptr<BackgroundImage> m_CatBaseCatFace;

    // button image (with interaction image)
    std::shared_ptr<Button> m_b_Start;
    std::shared_ptr<Button> m_b_Upgrade;
    std::shared_ptr<Button> m_b_TeamBuild;
    std::shared_ptr<Button> m_b_RestoreIcon;
    std::shared_ptr<Button> m_b_NormalGachaIcon;
    std::shared_ptr<Button> m_b_SpecialGachaIcon;
    std::shared_ptr<Button> m_b_PropsStore;
    std::shared_ptr<Button> m_b_Back;

public:
    std::vector<std::shared_ptr<Util::GameObject>> m_catBase;

    CatBase();

    void Update();
};


#endif //CAT_BASE_HPP