#include "../header/Decoy.h"

sf::Texture Decoy::texture_;
bool Decoy::textureLoaded_ = false;

Decoy::Decoy() : Entity(0.f, 0.f), lifetime_(0.f), active_(false) {
    if (!textureLoaded_) {
        if (!texture_.loadFromFile("textures/Budinca.png")) {
            texture_.loadFromFile("../textures/Budinca.png");
        }
        textureLoaded_ = true;
    }

    sprite_.setTexture(texture_);

    sf::FloatRect bounds = sprite_.getLocalBounds();
    sprite_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite_.setScale(40.f / bounds.width, 40.f / bounds.height);
}

Decoy::~Decoy() = default;

Entity* Decoy::clone() const {
    return new Decoy(*this);
}

void Decoy::print(std::ostream& os) const {
    os << "Decoy(active=" << active_ << ", time=" << lifetime_ << ")";
}

sf::FloatRect Decoy::getBounds() const {
    return sprite_.getGlobalBounds();
}

void Decoy::setMap(const Map* map) {
    mapRef_ = map;
}

void Decoy::spawn(const sf::Vector2f& pos, float duration, const sf::Vector2f& velocity) {
    active_ = true;
    lifetime_ = duration;
    setPosition(pos.x, pos.y);
    velocity_ = velocity;
}

void Decoy::update(float dt) {
    if (!active_) return;

    lifetime_ -= dt;
    if (lifetime_ <= 0.f) {
        active_ = false;
        return;
    }

    if (velocity_.x != 0.f || velocity_.y != 0.f) {
        sf::Vector2f currentPos(pos_.getX(), pos_.getY());
        sf::Vector2f nextPos = currentPos + velocity_ * dt;

        sf::FloatRect nextBounds(nextPos.x - 20.f, nextPos.y - 20.f, 40.f, 40.f);

        if (mapRef_ && mapRef_->collidesWithWall(nextBounds, true)) {
            velocity_ = {0.f, 0.f};
        } else {
            setPosition(nextPos.x, nextPos.y);
        }
    }
}

void Decoy::draw(sf::RenderWindow& window) const {
    if (active_) {
        Entity::draw(window);
    }
}

bool Decoy::active() const { return active_; }