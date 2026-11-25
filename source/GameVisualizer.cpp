#include "GameVisualizer.hpp"

#include "Global.hpp"

#include <iostream>
#include <stdexcept>

GameVisualizer::GameVisualizer(TileMap* tileMap)
    : m_tileMap(tileMap)
{
    if (m_tileMap == nullptr)
        throw std::invalid_argument("GameVisualizer: null tileMap");
}

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

std::string GameVisualizer::scoreboardText() const
{
    return "VIDAS: " + std::to_string(m_lives) + "    PONTOS: " + std::to_string(m_points);
}

bool GameVisualizer::init(unsigned widthPixels, unsigned heightPixels)
{
    if (widthPixels == 0 || heightPixels == 0)
        return false;

    if (!m_font.loadFromFile("DejaVuSans.ttf"))
    {
        std::cerr << "Erro: nao foi possivel carregar a fonte DejaVuSans.ttf" << std::endl;
        return false;
    }

    m_scoreText.setFont(m_font);
    m_scoreText.setCharacterSize(28);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setOutlineColor(sf::Color::Black);
    m_scoreText.setOutlineThickness(2.0f);
    m_scoreText.setPosition(20.0f, 14.0f);

    m_window.create(sf::VideoMode(widthPixels, heightPixels), "Firefly Trail");
    if (!m_window.isOpen())
        return false;

    m_window.setFramerateLimit(60);

    const float mapWidth  = static_cast<float>(m_tileMap->width())  * TILE_SIZE;
    const float mapHeight = static_cast<float>(m_tileMap->height()) * TILE_SIZE;
    m_view.setSize(mapWidth, mapHeight);
    m_view.setCenter(mapWidth / 2.0f, mapHeight / 2.0f);
    m_window.setView(m_view);
    return true;
}

void GameVisualizer::setInitialCenter(int col, int row)
{
    if (col < 0 || row < 0 ||
        col >= m_tileMap->width() ||
        row >= m_tileMap->height())
        return;

    if (m_tileMap->isWall(col, row))
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

void GameVisualizer::setWallColor(std::uint8_t r, std::uint8_t g, std::uint8_t b)
{
    m_wallColor = sf::Color(r, g, b);
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

    if (newCol < 0 || newRow < 0 ||
        newCol >= m_tileMap->width() ||
        newRow >= m_tileMap->height())
        return;

    if (m_tileMap->isWall(newCol, newRow))
        return;

    m_centerCol = newCol;
    m_centerRow = newRow;
    m_centerX = static_cast<float>(newCol);
    m_centerY = static_cast<float>(newRow);
}

void GameVisualizer::render()
{
    m_window.clear(m_backgroundColor);

    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    tile.setFillColor(m_wallColor);

    for (int row = 0; row < m_tileMap->height(); ++row)
    {
        for (int col = 0; col < m_tileMap->width(); ++col)
        {
            if (m_tileMap->at(col, row) == WALL_TILE)
            {
                tile.setPosition(static_cast<float>(col) * TILE_SIZE,
                                 static_cast<float>(row) * TILE_SIZE);
                m_window.draw(tile);
            }
        }
    }

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

    m_window.setView(m_window.getDefaultView());
    m_scoreText.setString(scoreboardText());
    m_window.draw(m_scoreText);
    m_window.setView(m_view);

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
