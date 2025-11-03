//
// Created by stefa on 11/3/2025.
//
#pragma once
#ifndef DECOY_H
#define DECOY_H
#include <SFML/Graphics.hpp>

class Decoy {
public:
    Decoy() : pos_(0.f,0.f), vel_(0.f,0.f), lifetime_(0.f), radius_(10.f) {
        shape_.setRadius(radius_);
        shape_.setFillColor(sf::Color(180, 180, 180));
        shape_.setOrigin(radius_, radius_);
    }

    void spawn(const sf::Vector2f& start, const sf::Vector2f& dir, float speed, float lifetime) {
        pos_ = start;
        vel_ = dir * speed;
        lifetime_ = lifetime;
        shape_.setPosition(pos_);
    }

    void update(float dt, const sf::FloatRect& playArea) {
        if (lifetime_ <= 0.f) return;
        pos_ += vel_ * dt;
        vel_ *= 0.98f; // light drag
        if (pos_.x - radius_ < playArea.left) { pos_.x = playArea.left + radius_; vel_.x = -vel_.x * 0.4f; }
        if (pos_.y - radius_ < playArea.top)  { pos_.y = playArea.top + radius_;  vel_.y = -vel_.y * 0.4f; }
        if (pos_.x + radius_ > playArea.left + playArea.width)  { pos_.x = playArea.left + playArea.width - radius_; vel_.x = -vel_.x * 0.4f; }
        if (pos_.y + radius_ > playArea.top + playArea.height) { pos_.y = playArea.top + playArea.height - radius_; vel_.y = -vel_.y * 0.4f; }
        shape_.setPosition(pos_);
        lifetime_ -= dt;
    }

    bool active() const { return lifetime_ > 0.f; }
    const sf::Vector2f& position() const { return pos_; }
    float timeLeft() const { return lifetime_; }
    void draw(sf::RenderWindow& w) const { if (active()) w.draw(shape_); }

private:
    sf::Vector2f pos_;
    sf::Vector2f vel_;
    float lifetime_;
    float radius_;
    sf::CircleShape shape_;
};


#endif //DECOY_H
