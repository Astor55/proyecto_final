#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"


class player : public character
{

    public:

        player(float _x, float _y, float _vida, float _fuerza,float _rango_ataque, bool _en_suelo)
            : character(_x, _y, _vida, _fuerza,_rango_ataque, _en_suelo)
        {
        // Implementacion del constructor en el .h debido a lo simple que es
        }

        void moverse(float dx, float dy) override; // Sobreescribe este metodo heredado de la clase base

        void atacar(character& objetivo, Ball& balon) override; // El override confirma que este metodo se declaro como virtual en la clase base

        void lanzar_balon(Ball& balon, float fuerza, float canasta_x, float canasta_y);

        ~player() override = default; // Confirma que el destructor de la clase padre esta declarado como virtual

        void inmovilizar(float duracion);

        void aplicar_boost_velocidad(float multiplicador, float duracion);

        void activar_flash_verde();

};
#endif // PLAYER_H
