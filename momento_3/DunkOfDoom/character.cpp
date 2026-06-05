#include "character.h"
#include "ball.h"

character::character(float _x, float _y, float _velocidad, float _vida, float _fuerza, float _rango_ataque, bool _en_suelo)
{
    x            = _x;
    y            = _y;
    velocidad    = _velocidad;
    vida         = _vida;
    vida_max     = _vida;
    fuerza       = _fuerza;
    rango_ataque = _rango_ataque;
    en_suelo     = _en_suelo;
    dx_actual    = 0.0f;
    dy_actual    = 0.0f;
}

void character::modificar_vida(float cantidad)
{
    vida += cantidad;
    if(vida > vida_max) vida = vida_max;
    if(vida < 0.0f)     vida = 0.0f;
}

float character::getx()           const { return x; }
float character::gety()           const { return y; }
float character::getvelocidad()   const { return velocidad; }
float character::getvida()        const { return vida; }
float character::getfuerza()      const { return fuerza; }
bool  character::getensuelo()     const { return en_suelo; }
float character::getdx_actual()   const { return dx_actual; }
float character::getdy_actual()   const { return dy_actual; }
float character::getrango_ataque()const { return rango_ataque; }

void character::setx(float num)   { x         = num; }
void character::sety(float num)   { y         = num; }
void character::setdx(float num)  { dx_actual = num; }
void character::setdy(float num)  { dy_actual = num; }
