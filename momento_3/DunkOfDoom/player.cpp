#include "player.h"
#include "ball.h"
#include <cmath>

using namespace std;


void player :: moverse(float dx, float dy){

    dx_actual = dx;
    dy_actual = dy;
    x += dx * velocidad;
    y += dy * velocidad;

    // limitar dentro de la pantalla
    if(x < 150)  x = 150;
    if(x > 1045) x = 1045;
    if(y < 350) y = 350;
    if(y > 600)  y = 600;
}


void player :: atacar(character& objetivo, Ball& balon){

    float dx = objetivo.getx() - x;

    float dy = objetivo.gety() - y;

    float distancia = sqrt((dx*dx) + (dy*dy));

    if(distancia <= rango_ataque && balon.portador == &objetivo){ // Estan lo suficientemente cerca para robarle la pelota al rival

        balon.recoger(this);
    }

}

void player::lanzar_balon(Ball &balon, float fuerza, float canasta_x, float canasta_y)
{
    float dx = canasta_x - x;
    float dy = canasta_y - y;
    float distancia = sqrt(dx*dx + dy*dy);

    float t = fuerza / 100.0f; // 0..1 según carga del lanzamiento

    // La velocidad horizontal es proporcional a la distancia a la canasta.
    // Más lejos → necesita más carga para llegar. Cerca se puede tirar con poca carga.
    // Rango efectivo de tiro: ~200-400px de distancia con carga máxima.
    float speed_h = (distancia / 600.0f) * (350.0f + t * 450.0f);

    if(distancia > 0)
        balon.vx = (dx / distancia) * speed_h;
    else
        balon.vx = 0.0f;

    // Componente vertical: fija para garantizar arco mínimo, aumenta con carga
    balon.vy = -(500.0f + t * 200.0f);

    balon.activa   = true;
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


void player::activar_flash_verde()
{

    flash_verde_activo = true;

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


