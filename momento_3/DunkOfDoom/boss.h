#ifndef BOSS_H
#define BOSS_H
#include "enemy.h"

class Ball;

class Boss : public Enemy
{
private:
    float velocidad_ataque;
    float posicion_predicha_x;
    float posicion_predicha_y;

<<<<<<< HEAD
public:
    Boss(float _x, float _y, float _velocidad, float _vida, float _fuerza, float _rango_ataque, bool _en_suelo, float _velocidad_ataque)
        : Enemy(_x, _y, _velocidad, _vida, _fuerza, _rango_ataque, _en_suelo),
        velocidad_ataque(_velocidad_ataque),
        posicion_predicha_x(0), posicion_predicha_y(0)
    {}
=======
    private:

        float velocidad_ataque;
        float posicion_predicha_x;
        float posicion_predicha_y;



    public:

        Boss(float _x, float _y, float _velocidad ,float _vida,float _fuerza,float _rango_ataque, bool _en_suelo, float _velocidad_ataque)
            : Enemy(_x,_y,_velocidad,_vida,_fuerza,_rango_ataque,_en_suelo), velocidad_ataque(_velocidad_ataque)
            , posicion_predicha_x(0), posicion_predicha_y(0)

        {

        }


        void lanzar_proyectil(Ball& balon);
        void calcular_posicion_jugador(character& jugador);
        void calcular_posible_nueva_posicion(character& jugador);


        ~Boss() = default;

>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

    void lanzar_proyectil(Ball& balon);
    void calcular_posicion_jugador(character& jugador);
    void calcular_posible_nueva_posicion(character& jugador);

    ~Boss() = default;
};

#endif // BOSS_H
