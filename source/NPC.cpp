#include "NPC.hpp"
#include <iostream>

void NPC::sprite (std::string path) {
    if (!m_texture.loadFromFile(path)) {
        std::cerr << "Erro: nao foi possivel carregar a imagem " << path << std::endl;
        exit(1);
    }
    m_sprite.setTexture(m_texture);
}
