#ifndef OPTION_BLOCK_HPP
#define OPTION_BLOCK_HPP

#include "Component/BackgroundImage.hpp"

class OptionBlock: public BackgroundImage {
private:
public:
    OptionBlock(
        const std::string& imagePath,
        float zIndex = 10.0F
        );

};

#endif //OPTION_BLOCK_HPP
