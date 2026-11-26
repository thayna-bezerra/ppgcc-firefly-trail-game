#include "CollisionManager.hpp"


bool CollisionManager::add(NPC* thing)
{
    if (thing == nullptr)
        return false;

    for (const NPC* other : m_things)
    {
        if (other == thing)
            return false;
    }

    if (thing->id != 0)
    {
        if (find(thing->id) != nullptr)
            return false;
    }
    else
    {
        // para pular ids que ja estao em uso
        while (find(m_nextId) != nullptr)
            ++m_nextId;

        thing->id = m_nextId;
        ++m_nextId;
    }

    m_things.push_back(thing);
    return true;
}

// -------------------------------------------------------------------
bool CollisionManager::remove(EntityId id)
{
    for (std::size_t i = 0; i < m_things.size(); ++i)
    {
        if (m_things[i]->id == id)
        {
            m_things.erase(m_things.begin() + static_cast<std::ptrdiff_t>(i));
            return true;
        }
    }

    return false;
}

// -------------------------------------------------------------------
NPC* CollisionManager::find(EntityId id) const
{
    for (NPC* thing : m_things)
    {
        if (thing->id == id)
            return thing;
    }

    return nullptr;
}

std::size_t CollisionManager::count() const noexcept
{
    return m_things.size();
}

// -------------------------------------------------------------------
// cada par aparece uma vez, na ordem de registro e as coisas inativas sao ignoradas
std::vector<CollisionManager::Pair> CollisionManager::collisions() const
{
    std::vector<Pair> pairs;

    for (std::size_t i = 0; i < m_things.size(); ++i)
    {
        if (!m_things[i]->active)
            continue;

        const AABB boxI = m_things[i]->bounds();

        for (std::size_t j = i + 1; j < m_things.size(); ++j)
        {
            if (!m_things[j]->active)
                continue;

            if (boxI.intersects(m_things[j]->bounds()))
                pairs.emplace_back(m_things[i]->id, m_things[j]->id);
        }
    }

    return pairs;
}
