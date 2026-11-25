

//
// Compilar (dentro de source/):
//   g++ -std=c++17 -Wno-misleading-indentation *.cpp -lsfml-system -lsfml-graphics -lsfml-window -o Jogo
//   ./Jogo



#include <iostream>

#include "Global.hpp"
#include "Vector2D.hpp"
#include "Collision2D.hpp"
#include "RigidBody2D.hpp"
#include "TileMap.hpp"
#include "NPC.hpp"
#include "Entity.hpp"
#include "GameVisualizer.hpp"

NPC       Player;
TileMap   map;
GameVisualizer view (&map);

//--------------------------------------------------------------

void UpdateInsideGameLoop (float deltaTime, sf::Event event) {
   (void) deltaTime;
   (void) event;

   Player.body.position.x = view.m_centerX;
   Player.body.position.y = view.m_centerY;

}

//--------------------------------------------------------------

int main () {

    
    
    if (!map.loadCSV ("maze.csv")) {
        std::cerr << "Erro: nao foi possivel carregar maze.csv" << std::endl;
        return 1;
    }

    Player.sprite ("um-quadrado.png");
    view.registerNPC (&Player);

    view.setUpdateFunction  (UpdateInsideGameLoop);
    view.setBackgroundColor (200, 255, 200);
    view.setWallColor       (139, 69, 19);

    if (!view.init (1024, 768))
        return 1;
    view.setInitialCenter (3, 4);    

    view.run ();

    return 0;
}
