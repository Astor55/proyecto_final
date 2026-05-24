#include "enemy.h"
#include "ball.h"
#include <cmath>


using namespace std;


void Enemy :: moverse(float dx ,float dy){

    x += dx * velocidad;

    y += dy * velocidad;
}

void Enemy :: atacar(character& objetivo, Ball& balon){

    float dx = objetivo.getx() - x;

    float dy = objetivo.gety() - y;

    float distancia = sqrt((dx*dx) + (dy*dy));

    if(distancia <= rango_ataque && balon.portador == &objetivo){

        balon.recoger(this);
    }
}

void Enemy :: percepcion(character& jugador, Ball& balon){

    float dx = jugador.getx() - x;
    float dy = jugador.gety() - y;

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
}

void Enemy :: accion(character& jugador, Ball& balon, float canasta_x, float canasta_y){


    if(decision == ALEJARSE){

        moverse(x - jugador.getx(), y - jugador.gety());
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


void Enemy :: aprendizaje(unsigned short puntos_player, unsigned short puntos_enemy){


    if(puntos_enemy < puntos_player){

        if(rango_ataque < rango_maximo){

            rango_ataque*= 1.1f;
        }
        decision = ARREBATAR;
    }

    else if(puntos_enemy > puntos_player){

        if(rango_ataque > rango_minimo){

            rango_ataque*= 0.9f;
        }

        decision = LANZAR;
    }
}


void Enemy :: lanzar_balon(Ball& balon, float canasta_x, float canasta_y){

    if(balon.portador == this){

        float fuerza_lanzamiento = fuerza;

        float dx = canasta_x - x;

        float dy = canasta_y - y;

        float angulo = atan2(dy,dx);

        balon.lanzar(fuerza_lanzamiento, angulo);
    }
}
