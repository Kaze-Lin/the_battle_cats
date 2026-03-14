#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Button : public Util::GameObject {
private:
    std::shared_ptr<Util::Image> img = nullptr;
    glm::vec2 b_OriginalSize = { };
public:
    Button(const std::string& imagePath);

    void ScaleSize(glm::vec2 s);
    void Place(glm::vec2 p);

    glm::vec2 GetCoordinate();
    glm::vec2 GetSize();
};

#endif //BUTTON_HPP