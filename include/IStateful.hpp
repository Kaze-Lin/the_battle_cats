#ifndef I_STATEFUL_HPP
#define I_STATEFUL_HPP

class IStateful {
public:
    virtual void Update() = 0;

    virtual ~IStateful() = default;
};

#endif //I_STATEFUL_HPP