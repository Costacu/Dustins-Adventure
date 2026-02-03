#pragma once
#ifndef GAMETEMPLATES_H
#define GAMETEMPLATES_H

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

namespace math {
    template <typename T>
    float getDistSq(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
        float dx = static_cast<float>(v1.x - v2.x);
        float dy = static_cast<float>(v1.y - v2.y);
        return dx * dx + dy * dy;
    }
}

template <typename T>
class GameProperty {
public:
    GameProperty(T val = T()) : current_(val), max_(val) {}
    
    void set(T val) { current_ = val; }
    void setMax(T val) { max_ = val; if (current_ > max_) current_ = max_; }
    void reset() { current_ = max_; }

    T get() const { return current_; }
    T getMax() const { return max_; }

    GameProperty<T>& operator=(const T& val) {
        current_ = val;
        return *this;
    }

    operator T() const { return current_; }

private:
    T current_;
    T max_;
};

#endif