#include "boss.h"
#include <cmath>


using namespace std;

void Boss :: calcular_posicion_jugador(character& jugador){

    posicion_predicha_x = jugador.getx();

    posicion_predicha_y = jugador.gety();
}

void Boss :: calcular_posible_nueva_posicion(character& jugador){

    posicion_predicha_x = jugador.getx() + (jugador.getdx_actual() * jugador.getvelocidad());

    posicion_predicha_y = jugador.gety() + (jugador.getdy_actual() * jugador.getvelocidad());
}

void Boss :: atacar(Ball& balon){

    float dx = posicion_predicha_x - x;

    float dy = posicion_predicha_y - y;

    float angulo = atan2(dy,dx);

    balon.lanzar(fuerza,angulo);
}
