#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "SystemInfo.hpp"

class BackgroundImage: public Util::GameObject {
private:
    std::shared_ptr<Util::Image> img = nullptr;

    glm::vec2 m_OriginalSize = { };
    glm::vec2 m_BaseScale = {1.0F, 1.0F};
public:
    explicit BackgroundImage(const std::string& imagePath, float zIndex = -10.0F);

    void AlignWithWindow();
    void AlignWithWindowWidth();
    void ScaleSize(glm::vec2 s = {1.0F, 1.0F});

    void Place(glm::vec2 p);

    glm::vec2 GetSize();
    glm::vec2 GetCoordinate();
};


#endif //BACKGROUNDIMAGE_HPP