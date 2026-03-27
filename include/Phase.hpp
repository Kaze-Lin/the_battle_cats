#ifndef PHASE_HPP
#define PHASE_HPP

#include "Util/GameObject.hpp"
#include "IStateful.hpp"

class Phase: public Util::GameObject {
public:
    Phase() = default;

    virtual ~Phase() = default;

    virtual void Update();

    virtual std::shared_ptr<Phase> GetDestinationPhase();
};

#endif //PHASE_HPP