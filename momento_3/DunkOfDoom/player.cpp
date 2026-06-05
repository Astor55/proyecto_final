#include "player.h"
#include "ball.h"
#include <cmath>

using namespace std;

void player::moverse(float dx, float dy)
{
    if(inmovilizado)
        return;

    dx_actual = dx;
    dy_actual = dy;

    x += dx * velocidad * config::DELTA_TIME;
    y += dy * velocidad * config::DELTA_TIME;

}

void player::atacar(character& objetivo, Ball& balon)
{
    float dx = objetivo.get_x() - x;
    float dy = objetivo.get_y() - y;

    float distancia = sqrt((dx * dx) + (dy * dy));

    if(distancia <= rango_ataque && balon.portador == &objetivo)
    {
        balon.recoger(this);
    }
}

void player::lanzar_balon(Ball& balon, float fuerza, float canasta_x, float canasta_y)
{
    float dx = canasta_x - x;
    float dy = canasta_y - y;
    float distancia = sqrt(dx * dx + dy * dy);

    float t = fuerza / 100.0f;

    float speed_h = (distancia / 600.0f) * (350.0f + t * 450.0f);

    if(distancia > 0)
        balon.vx = (dx / distancia) * speed_h;
    else
        balon.vx = 0.0f;

    balon.vy = -(500.0f + t * 200.0f);

    balon.activa = true;
    balon.en_suelo = false;
    balon.portador = nullptr;
}

void player::inmovilizar(float duracion)
{
    inmovilizado = true;
    timer_inmovilizacion = duracion;
}

void player::aplicar_boost_velocidad(float multiplicador, float duracion)
{
    velocidad = velocidad_base * multiplicador;
    boost_timer = duracion;
}

void player::actualizar(float dt)
{
    if(inmovilizado)
    {
        timer_inmovilizacion -= dt;

        if(timer_inmovilizacion <= 0.0f)
        {
            inmovilizado = false;
            timer_inmovilizacion = 0.0f;
        }
    }

    if(boost_timer > 0.0f)
    {
        boost_timer -= dt;

        if(boost_timer <= 0.0f)
        {
            velocidad = velocidad_base;
            boost_timer = 0.0f;
        }
    }
}

void player::activar_flash_verde()
{
    flash_verde_activo = true;
}


