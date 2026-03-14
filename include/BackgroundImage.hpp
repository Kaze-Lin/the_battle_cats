#ifndef BACKGROUNDIMAGE_HPP
#define BACKGROUNDIMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "SystemInfo.hpp"

class BackgroundImage: public Util::GameObject {
private:
    std::shared_ptr<Util::Image> img = nullptr;
    glm::vec2 m_OriginalSize = { };
public:
    explicit BackgroundImage(const std::string& imagePath);
    void AlignWindow();
    glm::vec2 GetSize();
};


#endif //BACKGROUNDIMAGE_HPP