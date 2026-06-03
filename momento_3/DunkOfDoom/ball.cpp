#include "ball.h"
#include "character.h"
#include <cmath>
#include <QDebug>



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

    vy = -(fuerza * sin(angulo));

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

    static constexpr float RADIO = 15.0f; // mitad del círculo 30px

    if (modo == modoBall::basketball)
    {

        if (x <= RADIO)
        {
            x = RADIO;
            vx =  std::abs(vx) * Coef_rebote;
        }

        if (x >= ancho - RADIO)
        {
            x = ancho - RADIO; vx = -std::abs(vx) * Coef_rebote;
        }

        static constexpr float SUELO_JUEGO = 600.0f;
        if (y >= SUELO_JUEGO - RADIO)
        {
            y = SUELO_JUEGO - RADIO;

            en_suelo = true;

            rebotar_suelo();
        }

        return;
    }

    if(al_colisionar == ComportamientoColision::Roto)
    {

        if(x <= 0.0f || x >= ancho ||
            y <= 0.0f || y >= alto)
        {

            activa = false;

            sprite_actual = sprite_ball::Destruida;

        }

        return;

    }

    // modo rebote del proyectil

    bool colisiono = false;

    if (x <= 0.0f)  { x = 0.0f;  vx =  std::abs(vx); colisiono = true; }
    if (x >= ancho) { x = ancho; vx = -std::abs(vx);  colisiono = true; }
    if (y <= 0.0f)  { y = 0.0f;  vy =  std::abs(vy);  colisiono = true; }
    if (y >= alto)  { y = alto;  vy = -std::abs(vy);   colisiono = true; }

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

    c->modificar_vida(-DANIO_PROYECTIL);
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

