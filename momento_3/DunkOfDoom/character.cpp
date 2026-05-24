#include "character.h"
#include "ball.h"

character::character(float _x, float _y, float _vida, float _fuerza, float _rango_ataque, bool _en_suelo){

    x = _x;

    y = _y;

    vida = _vida;

    fuerza = _fuerza;

    rango_ataque = _rango_ataque;

    en_suelo = _en_suelo;

}

void character :: modificar_vida(float cantidad){

    vida+= cantidad; // Si cantidad es negativa, disminuye la vida, si es positiva, aumenta
}



float character :: getx() const{

    return x;
}

float character :: gety() const{

    return y;
}

float character :: getvelocidad() const{

    return velocidad;
}

float character :: getfuerza() const{

    return fuerza;
}

float character :: getvida() const{

    return vida;
}

bool character :: getensuelo() const{

    return en_suelo;
}

float character :: getdx_actual() const{

    return dx_actual;
}

float character :: getdy_actual() const{

    return dy_actual;
}

void character :: setx(float num){

    x = num;
}

void character :: sety(float num){

    y = num;
}
