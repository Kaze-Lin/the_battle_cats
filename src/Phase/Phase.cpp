#include "Phase/Phase.hpp"

void Phase::Update() {
    for (auto& i: m_Children) {
        std::shared_ptr<IStateful> temp =
            std::dynamic_pointer_cast<IStateful>(i);

        if (temp != nullptr) {
            temp->Update();
        }
    }
}

std::shared_ptr<Phase> Phase::GetDestinationPhase() {
    return nullptr;
}

