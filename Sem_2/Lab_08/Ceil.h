//
// Created by dudko on 11.05.2020.
//

#ifndef LAB_08_CEIL_H
#define LAB_08_CEIL_H


class Ceil {
public:
    explicit Ceil(int color) : color_(color) {}
    Ceil() {color_ = 0;}
    Ceil(const Ceil& ceil) = default;

    int get_color() const {
        return color_;
    }

    void set_color(int color) {
        color_ = color;
    }

    bool operator==(const Ceil& o) const {
        return this->color_ == o.color_;
    }

    bool operator!=(const Ceil& o) const {
        return this->color_ != o.color_;
    }

private:
    int color_;
};

#endif //LAB_08_CEIL_H
