#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "Component/Text.hpp"
#include "Util/GameObject.hpp"

class TextButton: public Text, public Util::GameObject {
private:
    std::shared_ptr<Text> toBeContinue;

public:
    TextButton();
};

#endif //TEXT_BUTTON_HPP
