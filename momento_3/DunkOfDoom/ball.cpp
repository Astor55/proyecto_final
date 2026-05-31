#include "ball.h"
#include "character.h"
#include <cmath>


using namespace std;

// |constructor|
Ball::Ball(modoBall modo,
           ComportamientoColision comportamiento,
           unsigned short rebotes_max)
    : x(0.0f),
    y(0.0f),
    vx(0.0f),
    vy(0.0f),
    en_suelo(false),
    activa(true),
    modo(modo),
    al_colisionar(comportamiento),
    sprite_actual(sprite_ball::Normal),
    rebotes_restantes(rebotes_max),
    energia(1.0f),
    portador(nullptr)
{
    radio = 15.0f;
}



// |funcion de lanzamiento del balón|
void Ball::lanzar(float fuerza, float angulo)
{

    vx = fuerza * cos(angulo);

    vy = (fuerza * sin(angulo));

    activa = true;

    en_suelo = false;

    portador = nullptr;

    rebotes_restantes = REBOTES_MAX_DEF;

    energia = 1.0f;

    sprite_actual = sprite_ball::Normal;

}



// |unciones de recoger y soltar el balón|
void Ball::recoger(character* c)
{

    if(!c) return;

    portador = c;

    vx = 0.0f;

    vy = 0.0f;

    en_suelo = false;

}



void Ball::soltar()
{

    portador = nullptr;

}




// |actualizar|
void Ball::actualizar(float cambio)
{

    if(!activa) return;

    //si tiene portador
    if(portador)
    {

        x = portador->get_x() + OFFSET_X;
        y = portador->get_y() + OFFSET_Y;
        return;

    }

    if(modo == modoBall::basketball) aplicar_gravedad(cambio);

    mover(cambio);

}



// |fisicar internas|
void Ball::aplicar_gravedad(float cambio)
{

    if(!en_suelo)
    {
        vy += Gravedad * cambio;
    }

    else
    {
        vx *= config::FISICAS::FRICCION;
    }

    if(abs(vx) < Vel_minima)
    {
        vx = 0.0f;
    }

}



void Ball::mover(float cambio)
{

    x += vx * cambio;
    y += vy * cambio;

}



void Ball::rebotar_suelo()
{

    vy *= -Coef_rebote;
    en_suelo = false;

    if (abs(vy) < Vel_minima)
    {

        vy = 0.0f;
        en_suelo = true;

    }
}



void Ball::aplicar_rebote_proyectil()
{

    if(rebotes_restantes == 0)
    {

        activa = false;

        sprite_actual = sprite_ball::Destruida;

        return;

    }

    rebotes_restantes--;

    energia *= Perdida_energia;

    vx *= energia;

    vy *= energia;

    actualizar_sprite();

    if (rebotes_restantes == 0 ||
        (abs(vx) < Vel_minima && abs(vy) < Vel_minima))
    {

        activa = false;

        sprite_actual = sprite_ball::Destruida;

    }
}




// |colision con paredes|
void Ball::verificar_colision_paredes(float ancho, float alto)
{

    // limites reales del area jugable (dentro de los bordes decorativos)
    static constexpr float BORDE_IZQ  = 50.0f;
    static constexpr float BORDE_DER  = 1230.0f;
    static constexpr float BORDE_ARR  = 30.0f;
    static constexpr float BORDE_ABA  = 630.0f;

    // modo Roto
    if(al_colisionar == ComportamientoColision::Roto)
    {
        if(x <= BORDE_IZQ || x >= BORDE_DER ||
            y <= BORDE_ARR || y >= BORDE_ABA)
        {
            activa = false;
            sprite_actual = sprite_ball::Destruida;
        }
        return;
    }

    // modo rebote
    bool colisiono = false;
    if(x <= BORDE_IZQ)  { x = BORDE_IZQ;  vx =  std::abs(vx); colisiono = true; }
    if(x >= BORDE_DER)  { x = BORDE_DER;  vx = -std::abs(vx); colisiono = true; }
    if(y <= BORDE_ARR)  { y = BORDE_ARR;  vy =  std::abs(vy);  colisiono = true; }
    if(y >= BORDE_ABA)  { y = BORDE_ABA;  vy = -std::abs(vy);  colisiono = true; }

    if(colisiono) aplicar_rebote_proyectil();

}



// |colision con Character (el jugador o el enemigo)|
void Ball::verificar_colision_charater(character* c)
{

    if(!activa || !c || portador) return;

    const float dx = x - c->get_x();
    const float dy = y - c->get_y();
    const float distancia = sqrt(dx * dx + dy* dy);

    if(distancia > Radio_colision) return;

    if(modo == modoBall::basketball)
    {

        recoger(c);
        return;

    }

    // el proyectil se daña y desaparce

    c->modificar_vida(-DANIO);
    activa = false;
    sprite_actual = sprite_ball::Destruida;

}



//actualizar sprite según rebotes restantes
void Ball::actualizar_sprite()
{

    switch (rebotes_restantes)
    {
    case 3: sprite_actual = sprite_ball::Normal; break;

    case 2: sprite_actual = sprite_ball::Agrietada; break;

    case 1: sprite_actual = sprite_ball::Daniada; break;

    default: sprite_actual = sprite_ball::Destruida; break;

    }
}

