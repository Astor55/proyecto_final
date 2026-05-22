#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"


class player : public character
{

    public:

        player(float _x, float _y, float _vida, float _fuerza, bool _en_suelo)
            : character(_x, _y, _vida, _fuerza, _en_suelo)
        {
        // Implementacion del constructor en el .h debido a lo simple que es
        }

        void moverse() override; // Sobreescribe este metodo heredado de la clase base

        void atacar() override; // El override confirma que este metodo se declaro como virtual en la clase base

        ~player() override = default; // Confirma que el destructor de la clase padre esta declarado como virtual

        void inmovilizar(float duracion);

        void aplicar_boost_velocidad(float multiplicador, float duracion);

        void activar_flash_verde();

};
#endif // PLAYER_H
