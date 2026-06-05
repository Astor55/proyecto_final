#include "enemy.h"
#include "ball.h"
#include <cmath>

using namespace std;


void Enemy :: moverse(float dx ,float dy){

    x += dx * velocidad;

    y += dy * velocidad;
}


void Enemy :: percepcion(character& jugador, Ball& balon){

    float dx = jugador.get_x() - x;
    float dy = jugador.get_y() - y;

    distancia_al_jugador = sqrt((dx*dx) + (dy*dy));

    balon_en_mano = (balon.portador == this);

    balon_con_jugador = (balon.portador == &jugador);
}


void Enemy :: razonamiento(){

    if(balon_en_mano){

        if(distancia_al_jugador <= rango_ataque){

            decision = ALEJARSE;
        }

        else{

            decision = LANZAR;
        }
    }

    else if(balon_con_jugador){

        if(distancia_al_jugador <= rango_ataque){

            decision = ARREBATAR;

        }
    }

    else
    {

        decision = RECOGER;

    }

}

void Enemy :: accion(character& jugador, Ball& balon, float canasta_x, float canasta_y){


    if(decision == ALEJARSE){

        moverse(x - jugador.get_x(), y - jugador.get_y());
    }

    else if(decision == LANZAR){

        lanzar_balon(balon, canasta_x, canasta_y);
    }

    else if(decision == ARREBATAR){

        atacar(jugador,balon);
    }

    else if(decision == RECOGER){

        balon.recoger(this);
    }
}


void Enemy :: aprendizaje(){}


void Enemy :: lanzar_balon(Ball& balon, float canasta_x, float canasta_y){

    if(balon.portador == this){

        float fuerza_lanzamiento = fuerza;

        float dx = canasta_x - x;

        float dy = canasta_y - y;

        float angulo = atan2(dy,dx);

        balon.lanzar(fuerza_lanzamiento, angulo);
    }
}
