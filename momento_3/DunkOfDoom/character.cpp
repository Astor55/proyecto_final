#include "character.h"

character::character(float _x, float _y, float _vida, float _fuerza, bool _en_suelo){

    x = _x;

    y = _y;

    vida = _vida;

    fuerza = _fuerza;

    en_suelo = _en_suelo;

}

void character :: modificar_vida(float cantidad){

    vida+= cantidad; // Si cantidad es negativa, disminuye la vida, si es positiva, aumenta
}
