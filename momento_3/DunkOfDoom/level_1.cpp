#include "ball.h"
#include "level_1.h"
#include "config.h"

Level_1 :: Level_1() : Level(), puntos_player(0), puntos_enemy(0), tiempo_restante(config::NIVEL1::DURACION),
    balon(nullptr),enemigo(nullptr),ganador(nullptr)
{

}


void Level_1 :: actualizar(float dt){

    if(pausado || terminado) return;

    tiempo_restante-= dt;

    if(tiempo_restante <= 0){

        tiempo_restante = 0;

        definir_ganador();

        terminado = true;
        return;
    }


    balon->actualizar(dt);
    balon->verificar_colision_paredes(1280.0f,720.0f);
    balon->verificar_colision_charater(jugador);
    balon->verificar_colision_charater(enemigo);


    sumar_puntos_player();
    sumar_puntos_enemy();


    enemigo->percepcion(*jugador, *balon);
    enemigo->razonamiento();
    enemigo->accion(*jugador, *balon, canasta_player_x, canasta_player_y);
    enemigo->aprendizaje(puntos_player, puntos_enemy);
}


void Level_1 :: inicializacion(player* p, QGraphicsScene* escena){

    jugador = p;
    this->escena = escena;

    canasta_player_x = 120;
    canasta_player_y = 320;

    canasta_enemy_x = 1160;
    canasta_enemy_y = 320;

    jugador->setx(200.0f);
    jugador->sety(500.0f);

    enemigo = new Enemy(1000.0f, 500.0f, 200.0f, 80.0f, 40.0f, false);

    modoBall modo = modoBall::basketball;
    ComportamientoColision comportamiento = ComportamientoColision :: Rebote;
    balon = new Ball(modo, comportamiento, 3);

    balon->x = 640.0f;
    balon->y = 360.0f;

}


void Level_1 :: sumar_puntos_enemy(){

    if(balon->x >= canasta_player_x - config ::NIVEL1::RADIO_CESTA &&
        balon->x <= canasta_player_x + config :: NIVEL1::RADIO_CESTA &&
        balon->y >= config :: NIVEL1::ALTO_CESTA)
    {
        puntos_enemy+= config::NIVEL1::PUNTOS_POR_CANASTA;

        balon->x = 640.0f;
        balon->y = 360.0f;

    }
}

void Level_1 :: sumar_puntos_player(){

    if(balon->x >= canasta_enemy_x - config ::NIVEL1::RADIO_CESTA &&
        balon->x <= canasta_enemy_x + config :: NIVEL1::RADIO_CESTA &&
        balon->y >= config :: NIVEL1::ALTO_CESTA)

    {
        puntos_player+= config :: NIVEL1 :: PUNTOS_POR_CANASTA;

        balon->x = 640.0f;
        balon->y = 360.0f;
    }
}

void Level_1 :: definir_ganador(){

    if(pausado) return;

    else if(terminado){

        if(puntos_enemy > puntos_player){

            ganador = enemigo;
        }

        else if(puntos_player > puntos_enemy){

            ganador = jugador;
        }

        else if(puntos_player == puntos_enemy){

            ganador = nullptr;
        }
    }
}

void Level_1 :: finalizar(){

    if(pausado) return;

    puntos_enemy = 0;
    puntos_player = 0;

    delete enemigo;

    enemigo = nullptr;

    delete balon;

    balon = nullptr;


    detener_musica();

}



character* Level_1::getganador() const{

    return ganador;
}


float Level_1 :: gettiemporestante()const{

    return tiempo_restante;
}



Level_1 :: ~Level_1(){

    delete enemigo;

    enemigo = nullptr;

    delete balon;

    balon = nullptr;

}
