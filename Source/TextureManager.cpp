#include "../header/TextureManager.h"

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

sf::Texture& TextureManager::getTexture(const std::string& path) {
    if (textures_.find(path) == textures_.end()) {
        sf::Texture tex;
        std::vector<std::string> possiblePaths = {
            path,
            "textures/" + path,
            "../textures/" + path
        };

        bool loaded = false;
        for (const auto& p : possiblePaths) {
            if (tex.loadFromFile(p)) {
                loaded = true;
                break;
            }
        }

        if (!loaded) {
            sf::Image img;
            img.create(32, 32, sf::Color::Magenta);
            tex.loadFromImage(img);
        }

        tex.setSmooth(true);
        textures_[path] = tex;
    }

    return textures_[path];
}