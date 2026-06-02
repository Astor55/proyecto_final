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
    float angulo = atan2(dy, dx);
    balon.lanzar(fuerza, angulo);
    // proporcional a la fuerza del lanzamiento
    balon.vx = fuerza * 4.0f;
    balon.vy = -fuerza * 3.0f; // hacia arriba
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

