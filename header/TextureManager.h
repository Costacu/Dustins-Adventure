#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Exceptions.h"

class TextureManager {
public:
    static TextureManager& getInstance();

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    sf::Texture& getTexture(const std::string& path);

private:
    TextureManager() = default;
    std::map<std::string, sf::Texture> textures_;
};

#endif