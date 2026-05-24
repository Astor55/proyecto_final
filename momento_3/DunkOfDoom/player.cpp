#include "player.h"
#include "ball.h"
#include <cmath>

using namespace std;


void player :: moverse(float dx, float dy){

    dx_actual = dx;

    dy_actual = dy;

    x+= dx * velocidad;

    y+= dy * velocidad;
}


void player :: atacar(character& objetivo, Ball& balon){

    float dx = objetivo.get_x() - x;

    float dy = objetivo.get_y() - y;

    float distancia = sqrt((dx*dx) + (dy*dy));

    if(distancia <= rango_ataque && balon.portador == &objetivo){ // Estan lo suficientemente cerca para robarle la pelota al rival

        balon.recoger(this);
    }

}

void player :: lanzar_balon(Ball &balon, float fuerza, float canasta_x, float canasta_y){


    float fuerza_lanzamiento = fuerza;

    float dx = canasta_x - x;

    float dy = canasta_y - y;

    float angulo = atan2(dy,dx);

    balon.lanzar(fuerza_lanzamiento, angulo);

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
