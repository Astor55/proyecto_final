#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
public:
    float x, y;
    float vida;

    Character(float x, float y)
        : x(x), y(y), vida(100.0f) {}

    void modificar_vida(float cantidad) {
        vida += cantidad;
    }
};

#endif // CHARACTER_H
