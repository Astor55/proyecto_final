#ifndef BOSS_H
#define BOSS_H

#include "enemy.h"
#include "ball.h"

class Boss : public Enemy
{

    private:

        float velocidad_ataque;
        float posicion_predicha_x;
        float posicion_predicha_y;



    public:

        Boss(float _x, float _y, float _vida, float _fuerza,float _rango_ataque, bool _en_suelo, float _velocidad_ataque)
            : Enemy(_x,_y,_vida,_fuerza,_rango_ataque,_en_suelo), velocidad_ataque(_velocidad_ataque)
            , posicion_predicha_x(0), posicion_predicha_y(0)

        {

        }


        void atacar(Ball& balon);
        void calcular_posicion_jugador(character& jugador);
        void calcular_posible_nueva_posicion(character& jugador);


        ~Boss() = default;



};

#endif // BOSS_H
