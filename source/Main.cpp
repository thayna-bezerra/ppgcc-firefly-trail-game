// Etapa 1 do cronograma: movimentacao do player.
// Adaptado do Main.cpp do exemplo do professor: o "Heroi" virou o "Player" e o labirinto saiu.
//
// Compilar (dentro de source/):
//   g++ -std=c++17 -Wno-misleading-indentation *.cpp -lsfml-system -lsfml-graphics -lsfml-window -o Jogo
// Rodar (dentro de source/, porque a imagem e lida pelo caminho relativo):
//   ./Jogo
// Setas movem o quadrado, Esc fecha.

#include "Global.hpp"
#include "Vector2D.hpp"
#include "Collision2D.hpp"
#include "RigidBody2D.hpp"
#include "NPC.hpp"
#include "Entity.hpp"
#include "GameVisualizer.hpp"

NPC Player;

GameVisualizer view;

//--------------------------------------------------------------

void UpdateInsideGameLoop (float deltaTime, sf::Event event) {

   // Quando o usuario usa as setas, a classe GameVisualizer muda o centro da vista.
   // O Player fica sempre nesse centro (a posicao e dada em tiles).
   Player.body.position.x = view.m_centerX;
   Player.body.position.y = view.m_centerY;

}

//--------------------------------------------------------------

int main () {

    Player.sprite ("um-quadrado.png");
    view.registerNPC (&Player);

    view.setUpdateFunction  (UpdateInsideGameLoop);

    view.init(1024, 768);

    view.setInitialCenter   (7, 5);
    view.setBackgroundColor (30, 30, 45);

    view.run();

    return 0;
}
