#ifndef I_PLACEABLE_HPP
#define I_PLACEABLE_HPP

class IPlaceable {
public:
    virtual void Place() = 0;

    virtual ~IPlaceable();
};

#endif //I_PLACEABLE_HPP
