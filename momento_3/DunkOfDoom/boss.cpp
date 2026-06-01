#include "boss.h"
#include <cmath>
#include "ball.h"
#include <QRandomGenerator>

using namespace std;

void Boss :: calcular_posicion_jugador(character& jugador){

    posicion_predicha_x = jugador.get_x();

    posicion_predicha_y = jugador.get_y();
}

void Boss :: calcular_posible_nueva_posicion(character& jugador){

    posicion_predicha_x = jugador.get_x() + (jugador.getdx_actual() * jugador.getvelocidad());

    posicion_predicha_y = jugador.get_y() + (jugador.getdy_actual() * jugador.getvelocidad());
}

void Boss :: lanzar_proyectil(Ball& balon){

    balon.modo = modoBall::projectile;

    unsigned char prob = static_cast<unsigned char>(QRandomGenerator::global()->bounded(100));

    balon.al_colisionar = (prob < 50) ? ComportamientoColision::Rebote : ComportamientoColision::Roto;

    float dx = posicion_predicha_x - x;

    float dy = posicion_predicha_y - y;

    float angulo = atan2(dy,dx);

    balon.lanzar(velocidad_ataque, angulo);

}
