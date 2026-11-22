#pragma once

#include "NPC.hpp"
#include "TileMap.hpp"

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

/**
 * @brief Displays a Dungeon2D/TileMap scenario using SFML, 
 *    and displays NPC objects.
 *
 * The class deliberately exposes no SFML objects to its users.  A TileMap
 * cell is displayed as a TILE_SIZE x TILE_SIZE square.
 *
 * The class relies on the tilemap matrix to display walls.
 *
 * The position of a NPC is given by body.position in tile coordinates, 
 *   and a NPC has the same size of a tile.
 *
 * Navigation is tile-based: an arrow-key press attempts to move the centre
 * of the view by one tile.  The move is accepted only when the destination
 * cell is EMPTY_TILE, so the centre of the view is never placed on a wall.
 * 
 * The class updates variables m_centerX and m_centerY with the coordinates
 *  of the centre of the view, in tile units.
 */
class GameVisualizer {
public:

    float m_centerX{0.0f}, m_centerY{0.0f};

    using UpdateFunction = void (*)(float deltaTime, sf::Event event);
    UpdateFunction m_updateFunction{nullptr};

    explicit GameVisualizer(TileMap* tileMap);

    /// The pointed-to NPCs must be alive while this object is used.
    void registerNPC(NPC* the_npc);
    std::vector<NPC*> m_npc;

    /// Creates the window and fits the view to the whole map. Returns true on success.
    bool init(unsigned widthPixels, unsigned heightPixels);

    /// Initial centre, in TileMap coordinates. If the cell is a wall or outside
    /// the map, the centre is left unchanged.
    void setInitialCenter(int col, int row);

    void setBackgroundColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
    void setWallColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
    void setUpdateFunction(UpdateFunction function);

    /// Runs the event/render loop until the window is closed (or Esc is pressed).
    void run();

private:
    TileMap* m_tileMap;
    sf::RenderWindow m_window;
    sf::View m_view;

    sf::Color m_backgroundColor{sf::Color::White};
    sf::Color m_wallColor{139, 69, 19};

    int m_centerCol{0};
    int m_centerRow{0};

    void handleEvent(const sf::Event& event);
    void tryMoveCenter(int deltaCol, int deltaRow);
    void render();
};
