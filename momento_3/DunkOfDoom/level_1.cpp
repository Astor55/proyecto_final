#include "ball.h"
#include "level_1.h"
#include "config.h"


Level_1 :: Level_1() : Level(), puntos_player(0), puntos_enemy(0), tiempo_restante(config::NIVEL1::DURACION),
    balon(nullptr),enemigo(nullptr),ganador(nullptr)
{

}


void Level_1 :: actualizar(float dt){

    // estado del jugador
    if(balon->portador == jugador)
    {
        if(jugador->getdx_actual() != 0 || jugador->getdy_actual() != 0)
            estado_jugador = EstadoAnimacion::CORRIENDO_CON_BALON;
        else
            estado_jugador = EstadoAnimacion::CAMINANDO_CON_BALON;
    }
    else if(estado_jugador == EstadoAnimacion::LANZANDO && balon->portador == nullptr)
        estado_jugador = EstadoAnimacion::LANZANDO;
    else
        estado_jugador = EstadoAnimacion::CORRIENDO_SIN_BALON;

    // estado del enemigo
    if(balon->portador == enemigo){

            estado_enemigo = EstadoAnimacion::CAMINANDO_CON_BALON;

        }

    else if(estado_enemigo == EstadoAnimacion::LANZANDO && balon->portador == nullptr){


            estado_enemigo = EstadoAnimacion::LANZANDO;

        }

    else{

             estado_enemigo = EstadoAnimacion::CORRIENDO_SIN_BALON;

        }



    if(pausado || terminado) return;

    tiempo_restante-= dt;

    if(tiempo_restante <= 0){

        tiempo_restante = 0;

        definir_ganador();

        terminado = true;
        return;
    }


    balon->actualizar(dt);
    balon->verificar_colision_paredes(1280.0f, 720.0f);
    balon->verificar_colision_charater(jugador);
    balon->verificar_colision_charater(enemigo);

    sumar_puntos_player();
    sumar_puntos_enemy();

    // luego el agente
    enemigo->percepcion(*jugador, *balon);
    enemigo->razonamiento();
    enemigo->accion(*jugador, *balon, canasta_player_x, canasta_player_y);



    // actualizar timer de frames
    timer_frame_jugador += dt;
    timer_frame_enemigo += dt;

    float duracion_frame = 0.1f; // cada frame dura 0.1 segundos

    if(estado_jugador != estado_anterior_jugador)
    {
        frame_jugador = 0;
        timer_frame_jugador = 0.0f;
        estado_anterior_jugador = estado_jugador;
    }

    /*if(balon->portador == nullptr)
    {
        float dx = jugador->getx() - balon->x;
        float dy = jugador->gety() - balon->y;
        float distancia = std::sqrt(dx*dx + dy*dy);
        if(distancia <= jugador->getrango_ataque())
            balon->recoger(jugador);
    }
*/



    // actualizar frame del jugador
    if(timer_frame_jugador >= duracion_frame)
    {
        timer_frame_jugador = 0.0f;
        int max_frames = (estado_jugador == EstadoAnimacion::CAMINANDO_CON_BALON ||
                          estado_jugador == EstadoAnimacion::CORRIENDO_CON_BALON) ? 5 : 6;
        frame_jugador = (frame_jugador + 1) % max_frames;
    }

    // actualizar frame del enemigo
    if(timer_frame_enemigo >= duracion_frame)
    {
        timer_frame_enemigo = 0.0f;
        if(estado_enemigo == EstadoAnimacion::CAMINANDO_CON_BALON ||
            estado_enemigo == EstadoAnimacion::CORRIENDO_CON_BALON)
            frame_enemigo = 5; // fijo en el frame donde se ve el balon
        else
            frame_enemigo = (frame_enemigo + 1) % 6;
    }

    // calcular fila segun estado
    auto calcular_fila = [](EstadoAnimacion estado) -> int {
        switch(estado) {
        case EstadoAnimacion::CAMINANDO_CON_BALON:  return 0;
        case EstadoAnimacion::CORRIENDO_CON_BALON:  return 1;
        case EstadoAnimacion::LANZANDO:             return 2;
        case EstadoAnimacion::CORRIENDO_SIN_BALON:  return 3;
        default: return 0;
        }
    };

    // recortar y mostrar frame del jugador
    int fila_jugador = calcular_fila(estado_jugador);
    int frame_w_jugador = (fila_jugador <= 1) ? 135 : 112;
    sprite_jugador->setPixmap(sheet_jugador.copy(
        frame_jugador * frame_w_jugador,
        fila_jugador * 94,
        frame_w_jugador, 88));

    // recortar y mostrar frame del enemigo

    static constexpr int FILA_Y_ENEMIGO[] = {18, 105, 206, 295};
    int fila_enemigo = calcular_fila(estado_enemigo);

    sprite_enemigo->setPixmap(sheet_enemigo.copy(
        frame_enemigo * 112,
        FILA_Y_ENEMIGO[fila_enemigo],
        111, 87));

    // sincronizar posicion grafica con logica
    sprite_jugador->setPos(jugador->getx(), jugador->gety());
    sprite_enemigo->setPos(enemigo->getx(), enemigo->gety());


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

    enemigo = new Enemy(1000.0f, 500.0f, 60.0f,200.0f, 80.0f, 40.0f, false);

    modoBall modo = modoBall::basketball;
    ComportamientoColision comportamiento = ComportamientoColision :: Rebote;
    balon = new Ball(modo, comportamiento, 3);

    balon->x = 640.0f;
    balon->y = 360.0f;

    balon->recoger(jugador);

    // Carga de los sprites

    sheet_jugador = QPixmap(":/assets/player/doom_comportamiento_lvl1.png");
    sheet_enemigo = QPixmap(":/assets/enemy/demonio_comportamiento_lvl1.png");

    // Crear items graficos y agregarlos a la escena
    sprite_jugador = new QGraphicsPixmapItem();
    sprite_enemigo = new QGraphicsPixmapItem();
    escena->addItem(sprite_jugador);
    escena->addItem(sprite_enemigo);

    // Posicion inicial en la escena
    sprite_jugador->setPos(jugador->getx(), jugador->gety());
    sprite_enemigo->setPos(enemigo->getx(), enemigo->gety());

    // Estado inicial
    estado_jugador = EstadoAnimacion::CORRIENDO_SIN_BALON;
    estado_enemigo = EstadoAnimacion::CORRIENDO_SIN_BALON;
    frame_jugador = 0; // Frame dentro del mapa de sprites, que sea el primero y no uno aleatorio
    frame_enemigo = 5;
    timer_frame_jugador = 0.0f; // Tiempo que demora en cambiar de sprite
    timer_frame_enemigo = 0.0f;

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
