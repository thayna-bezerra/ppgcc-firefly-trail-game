#include "GameVisualizer.hpp"

#include "Global.hpp"

void GameVisualizer::registerNPC(NPC* the_npc)
{
    if (the_npc == nullptr)
        return;

    for (NPC* npc : m_npc)
    {
        if (npc == the_npc)
            return;
    }

    m_npc.push_back(the_npc);
}

bool GameVisualizer::init(unsigned widthPixels, unsigned heightPixels)
{
    if (widthPixels == 0 || heightPixels == 0)
        return false;

    m_window.create(sf::VideoMode(widthPixels, heightPixels), "Firefly Trail");
    if (!m_window.isOpen())
        return false;

    m_window.setFramerateLimit(60);
    m_cols = static_cast<int>(widthPixels / TILE_SIZE);
    m_rows = static_cast<int>(heightPixels / TILE_SIZE);
    return true;
}

void GameVisualizer::setInitialCenter(int col, int row)
{
    if (col < 0 || row < 0 || col >= m_cols || row >= m_rows)
        return;

    m_centerCol = col;
    m_centerRow = row;
    m_centerX = static_cast<float>(col);
    m_centerY = static_cast<float>(row);
}

void GameVisualizer::setBackgroundColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    m_backgroundColor = sf::Color(r, g, b);
}

void GameVisualizer::setUpdateFunction(UpdateFunction function)
{
    m_updateFunction = function;
}

void GameVisualizer::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed)
    {
        m_window.close();
        return;
    }

    if (event.type != sf::Event::KeyPressed)
        return;

    switch (event.key.code)
    {
        case sf::Keyboard::Left:
            tryMoveCenter(-1, 0);
            break;

        case sf::Keyboard::Right:
            tryMoveCenter(1, 0);
            break;

        case sf::Keyboard::Up:
            tryMoveCenter(0, -1);
            break;

        case sf::Keyboard::Down:
            tryMoveCenter(0, 1);
            break;

        case sf::Keyboard::Escape:
            m_window.close();
            break;

        default:
            break;
    }
}

void GameVisualizer::tryMoveCenter(int deltaCol, int deltaRow)
{
    const int newCol = m_centerCol + deltaCol;
    const int newRow = m_centerRow + deltaRow;

    if (newCol < 0 || newRow < 0 || newCol >= m_cols || newRow >= m_rows)
        return;

    m_centerCol = newCol;
    m_centerRow = newRow;
    m_centerX = static_cast<float>(newCol);
    m_centerY = static_cast<float>(newRow);
}

void GameVisualizer::render()
{
    m_window.clear(m_backgroundColor);

    for (NPC* npc : m_npc)
    {
        if (npc == nullptr || !npc->active)
            continue;

        npc->m_sprite.setPosition(
            npc->body.position.x * TILE_SIZE,
            npc->body.position.y * TILE_SIZE
        );

        m_window.draw(npc->m_sprite);
    }

    m_window.display();
}

void GameVisualizer::run()
{
    if (!m_window.isOpen())
        return;

    sf::Clock clock;

    while (m_window.isOpen())
    {
        sf::Event event{};

        while (m_window.pollEvent(event))
            handleEvent(event);

        if (!m_window.isOpen())
            break;

        if (m_updateFunction != nullptr)
        {
            const float deltaTime = clock.restart().asSeconds();
            m_updateFunction(deltaTime, event);
        }

        render();
    }
}
