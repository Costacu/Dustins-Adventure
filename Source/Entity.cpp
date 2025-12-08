#include "../header/Entity.h"
#include "../header/Exceptions.h"


sf::FloatRect Entity::getBounds() const {
    sf::FloatRect b = sprite_.getGlobalBounds();
    if (b.width <= 0.f || b.height <= 0.f)
        return {pos_.getX(), pos_.getY(), 48.f, 48.f};
    return b;
}

void Entity::setPosition(float x, float y) {
    if (x < 0 || y < 0)
        throw InvalidPositionError(x, y);

    pos_.set(x, y);
    sprite_.setPosition(x, y);
}


void Entity::move(float dx, float dy) {
    pos_.translate(dx, dy);
    sprite_.setPosition(pos_.getX(), pos_.getY());
}

void Entity::draw(sf::RenderWindow& window) const {
    window.draw(sprite_);
}

int Entity::entityCount = 0;

//constructor in clasa de baza
Entity::Entity(float x, float y) {
    pos_.set(x, y);
    sprite_.setPosition(x, y);
    entityCount++;
}
int Entity::getEntityCount() {
    return entityCount;
}



