#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "SystemInfo.hpp"

class BackgroundImage: public Util::GameObject {
protected:
    glm::vec2 m_OriginalSize = { };
    glm::vec2 m_BaseScale = {1.0F, 1.0F};

public:
    explicit BackgroundImage(
        const std::string& imagePath,
        float zIndex = -10.0F
        );

    void AlignWithWindow();
    void AlignWithWindowWidth();
    void ScaleSize(glm::vec2 s = {1.0F, 1.0F});
    void SetImage(const std::string& ImagePath);

    virtual void Place(const glm::vec2 &p) override;

    glm::vec2 GetSize();
    glm::vec2 GetCoordinate() const override;
};


#endif //BACKGROUNDIMAGE_HPP