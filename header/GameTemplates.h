#pragma once
#ifndef GAMETEMPLATES_H
#define GAMETEMPLATES_H

#include <algorithm>

template <typename T>
class GameProperty {
public:
    explicit GameProperty(T val = T()) : current_(val), max_(val) {}

    T get() const { return current_; }

    void set(T val) {
        current_ = val;
        if (current_ > max_) current_ = max_;
    }

    void reset() { current_ = max_; }

    GameProperty& operator-=(T val) {
        current_ -= val;
        return *this;
    }

    bool operator<=(T val) const { return current_ <= val; }
    bool operator>(T val) const { return current_ > val; }

private:
    T current_;
    T max_;
};

namespace math {
    template <typename V>
    float getDistSq(const V& a, const V& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return dx * dx + dy * dy;
    }
}

#endif