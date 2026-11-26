#pragma once

#include "Global.hpp"
#include "NPC.hpp"

#include <cstddef>
#include <utility>
#include <vector>

class CollisionManager {
public:
    using Pair = std::pair<EntityId, EntityId>;

    bool add(NPC* thing);
    bool remove(EntityId id);
    [[nodiscard]] NPC* find(EntityId id) const;
    [[nodiscard]] std::size_t count() const noexcept;
    [[nodiscard]] std::vector<Pair> collisions() const;

private:
    std::vector<NPC*> m_things;
    EntityId m_nextId{INITIAL_ENTITY_ID};
};
