#pragma once

#include "NPC.hpp"

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

/**
 * @brief Opens the SFML window, reads the arrow keys and draws the NPCs.
 *
 * Adapted from the professor's DungeonVisualizer, without the dungeon and the
 * tilemap (they are not part of this step). The window is divided in tiles of
 * TILE_SIZE pixels. A press of an arrow key moves the centre of the view by one
 * tile, and the centre never leaves the window. The class keeps the position of
 * the centre, in tile units, in m_centerX and m_centerY; the game copies it to
 * the player body.
 */
class GameVisualizer {
public:

    float m_centerX{0.0f}, m_centerY{0.0f};

    /// Called once per rendered frame with the elapsed time (s) and the last event.
    using UpdateFunction = void (*)(float deltaTime, sf::Event event);
    UpdateFunction m_updateFunction{nullptr};

    /// The pointed-to NPCs must be alive while this object is used.
    void registerNPC(NPC* the_npc);
    std::vector<NPC*> m_npc;

    /// Creates the window. Returns true if it was created.
    bool init(unsigned widthPixels, unsigned heightPixels);

    /// Initial centre, in tile coordinates. Ignored if outside the window.
    void setInitialCenter(int col, int row);

    void setBackgroundColor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
    void setUpdateFunction(UpdateFunction function);

    /// Runs the event/render loop until the window is closed (or Esc).
    void run();

private:
    sf::RenderWindow m_window;
    sf::Color m_backgroundColor{sf::Color::White};

    int m_cols{0};
    int m_rows{0};
    int m_centerCol{0};
    int m_centerRow{0};

    void handleEvent(const sf::Event& event);
    void tryMoveCenter(int deltaCol, int deltaRow);
    void render();
};
