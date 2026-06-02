#include "ball.h"
#include "level_1.h"
#include "config.h"
#include <QDebug>
#include <QTransform>

using namespace std;


Level_1 :: Level_1() : Level(), puntos_player(0), puntos_enemy(0), tiempo_restante(config::NIVEL1::DURACION),
    balon(nullptr),enemigo(nullptr),ganador(nullptr)
{

}


void Level_1::actualizar(float dt)
{

    // mostrar pantalla final si termino
    if(terminado)
    {
        if(ganador == jugador)
        {
            QPixmap you_win(":/assets/you_win1.png");
            pantalla_final->setPixmap(you_win.scaled(1280, 720, Qt::IgnoreAspectRatio));
            pantalla_final->setPos(0, 0);
            pantalla_final->setVisible(true);
        }
        else if(ganador == enemigo)
        {
            QPixmap game_over(":/assets/game_over1.png");
            pantalla_final->setPixmap(game_over.scaled(1280, 720, Qt::IgnoreAspectRatio));
            pantalla_final->setPos(0, 0);
            pantalla_final->setVisible(true);
        }
        return;
    }

    if(pausado) return;

    // actualizar tiempo
    tiempo_restante -= dt;
    if(tiempo_restante <= 0)
    {
        tiempo_restante = 0;
        definir_ganador();
        terminado = true;
        return;
    }

    // actualizar estados de animacion
    if(balon->portador == jugador)
    {
        if(jugador->getdx_actual() != 0 || jugador->getdy_actual() != 0)
            estado_jugador = EstadoAnimacion::CORRIENDO_CON_BALON;
        else
            estado_jugador = EstadoAnimacion::CAMINANDO_CON_BALON;
    }
    else
    {
        if(jugador->getdx_actual() != 0 || jugador->getdy_actual() != 0)
            estado_jugador = EstadoAnimacion::CORRIENDO_SIN_BALON;
    }

    if(balon->portador == enemigo)
        estado_enemigo = EstadoAnimacion::CAMINANDO_CON_BALON;
    else if(estado_enemigo == EstadoAnimacion::LANZANDO && balon->portador == nullptr)
        estado_enemigo = EstadoAnimacion::LANZANDO;
    else
        estado_enemigo = EstadoAnimacion::CORRIENDO_SIN_BALON;


    // actualizar invulnerabilidad
    if(timer_invulnerabilidad > 0)
        timer_invulnerabilidad -= dt;

    // actualizar balon
    balon->actualizar(dt);

    // limites de la cancha para el balon
    if(balon->x < 150)  { balon->x = 150;  balon->vx = -balon->vx; }
    if(balon->x > 1045) { balon->x = 1045; balon->vx = -balon->vx; }
    if(balon->y < 320)  { balon->y = 320;  balon->vy = -balon->vy; }
    if(balon->y > 600)  { balon->y = 600;  balon->vy = -balon->vy; }

    balon->verificar_colision_paredes(1280.0f, 720.0f);

    if(balon->portador == nullptr && timer_invulnerabilidad <= 0)
    {
        timer_invulnerabilidad = 0.3f;
        balon->verificar_colision_charater(jugador);
        balon->verificar_colision_charater(enemigo);
    }

    sumar_puntos_player();
    sumar_puntos_enemy();

    enemigo->percepcion(*jugador, *balon, dt);
    enemigo->razonamiento();
    enemigo->accion(*jugador, *balon, canasta_player_x, canasta_player_y, timer_invulnerabilidad);

    // actualizar timers de animacion
    timer_frame_jugador += dt;
    timer_frame_enemigo += dt;

    float duracion_frame = 0.15f;

    if(estado_jugador != estado_anterior_jugador)
    {
        frame_jugador = 0;
        timer_frame_jugador = 0.0f;
        estado_anterior_jugador = estado_jugador;
    }

    if(timer_frame_jugador >= duracion_frame)
    {
        timer_frame_jugador = 0.0f;
        int max_frames = (estado_jugador == EstadoAnimacion::CAMINANDO_CON_BALON ||
                          estado_jugador == EstadoAnimacion::CORRIENDO_CON_BALON) ? 5 : 6;
        frame_jugador = (frame_jugador + 1) % max_frames;
    }

    if(timer_frame_enemigo >= duracion_frame)
    {
        timer_frame_enemigo = 0.0f;
        if(estado_enemigo == EstadoAnimacion::CAMINANDO_CON_BALON ||
            estado_enemigo == EstadoAnimacion::CORRIENDO_CON_BALON)
            frame_enemigo = 5;
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
    QPixmap recorte_jugador = sheet_jugador.copy(
        frame_jugador * frame_w_jugador,
        fila_jugador * 94,
        frame_w_jugador, 88);

    // voltear si se mueve a la izquierda
    if(jugador->getdx_actual() < 0)
    {
        QTransform flip;
        flip.scale(-1, 1);
        recorte_jugador = recorte_jugador.transformed(flip);
    }
    sprite_jugador->setPixmap(recorte_jugador);


    // recortar y mostrar frame del enemigo
    static constexpr int FILA_Y_ENEMIGO[] = {18, 105, 206, 295};
    int fila_enemigo = calcular_fila(estado_enemigo);
    QPixmap recorte_enemigo = sheet_enemigo.copy(
        frame_enemigo * 111,
        FILA_Y_ENEMIGO[fila_enemigo],
        111, 87);

    if(enemigo->getdx_actual() < 0)
    {
        QTransform flip;
        flip.scale(-1, 1);
        recorte_enemigo = recorte_enemigo.transformed(flip);
    }
    sprite_enemigo->setPixmap(recorte_enemigo);

    // sincronizar posicion grafica con logica
    sprite_jugador->setPos(jugador->getx(), jugador->gety());
    sprite_enemigo->setPos(enemigo->getx(), enemigo->gety());

    // timer texto canasta
    if(timer_texto_canasta > 0)
    {
        timer_texto_canasta -= dt;
        if(timer_texto_canasta <= 0)
            texto_canasta->setVisible(false);
    }

    // marcador y tiempo
    texto_puntos_player->setPlainText(QString::number(puntos_player));
    texto_puntos_enemy->setPlainText(QString::number(puntos_enemy));
    texto_tiempo->setPlainText(QString::number((int)tiempo_restante));

    // sprite del balon
    if(balon_en_aire && balon->activa)
    {
        sprite_balon->setPos(balon->x, balon->y);
        sprite_balon->setVisible(true);
    }
    else
        sprite_balon->setVisible(false);

    if(balon->portador != nullptr && balon->portador != jugador)
        balon_en_aire = false;
}


void Level_1 :: inicializacion(player* p, QGraphicsScene* escena){

    iniciar_musica("C:/Users/EGONZALEZ/Desktop/Proyecto_final/proyecto_final/momento_3/DunkOfDoom/assets/music_level1.mp3");

    // fondo - se agrega primero para que quede detras de todo
    QPixmap fondo(":/assets/sprites/level1.doom.png");
    fondo = fondo.scaled(1280, 720); // escalar al tamaño de la pantalla
    QGraphicsPixmapItem* sprite_fondo = new QGraphicsPixmapItem(fondo);
    sprite_fondo->setZValue(-1); // asegura que quede detras de los personajes
    escena->addItem(sprite_fondo);


    jugador = p;
    this->escena = escena;

    canasta_player_x = 160;
    canasta_player_y = 363;

    canasta_enemy_x  = 1040;
    canasta_enemy_y  = 363;

    qDebug() << "Canasta enemiga inicializada en : " << float(canasta_enemy_x) << float(canasta_enemy_y);

    jugador->setx(300.0f);
    jugador->sety(500.0f);

    texto_puntos_player = new QGraphicsTextItem();
    texto_puntos_enemy  = new QGraphicsTextItem();
    texto_tiempo        = new QGraphicsTextItem();

    // estilo del texto
    QFont fuente("Arial", 24, QFont::Bold);
    texto_puntos_player->setFont(fuente);
    texto_puntos_enemy->setFont(fuente);
    texto_tiempo->setFont(fuente);

    texto_puntos_player->setDefaultTextColor(Qt::white);
    texto_puntos_enemy->setDefaultTextColor(Qt::white);
    texto_tiempo->setDefaultTextColor(Qt::white);

    // posicion en pantalla
    texto_puntos_player->setPos(100, 20);
    texto_puntos_enemy->setPos(1100, 20);
    texto_tiempo->setPos(590, 20);

    escena->addItem(texto_puntos_player);
    escena->addItem(texto_puntos_enemy);
    escena->addItem(texto_tiempo);

    rect_player = new QGraphicsRectItem(20, 10, 120, 60);
    rect_enemy  = new QGraphicsRectItem(1140, 10, 120, 60);

    QBrush fondo_rect(QColor(0, 0, 0, 150)); // negro semitransparente
    QPen borde(Qt::red); // borde rojo temático
    borde.setWidth(2);

    rect_player->setBrush(fondo_rect);
    rect_player->setPen(borde);
    rect_enemy->setBrush(fondo_rect);
    rect_enemy->setPen(borde);

    escena->addItem(rect_player);
    escena->addItem(rect_enemy);


    // labels
    QFont fuente_label("Arial", 10, QFont::Bold);
    texto_label_player = new QGraphicsTextItem("You");
    texto_label_enemy  = new QGraphicsTextItem("Enemy");
    texto_label_player->setFont(fuente_label);
    texto_label_enemy->setFont(fuente_label);
    texto_label_player->setDefaultTextColor(Qt::red);
    texto_label_enemy->setDefaultTextColor(Qt::red);
    texto_label_player->setPos(55, 12);
    texto_label_enemy->setPos(1165, 12);
    escena->addItem(texto_label_player);
    escena->addItem(texto_label_enemy);

    // puntos y tiempo dentro de los recuadros
    QFont fuente_puntos("Arial", 24, QFont::Bold);
    texto_puntos_player->setFont(fuente_puntos);
    texto_puntos_enemy->setFont(fuente_puntos);
    texto_tiempo->setFont(fuente_puntos);
    texto_puntos_player->setDefaultTextColor(Qt::white);
    texto_puntos_enemy->setDefaultTextColor(Qt::white);
    texto_tiempo->setDefaultTextColor(Qt::white);
    texto_puntos_player->setPos(60, 28);
    texto_puntos_enemy->setPos(1185, 28);
    texto_tiempo->setPos(610, 15);

    sheet_balon = QPixmap(":/assets/ball.png");
    sprite_balon = new QGraphicsPixmapItem();
    sprite_balon->setPixmap(sheet_balon.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    sprite_balon->setZValue(100);
    sprite_balon->setVisible(false);
    escena->addItem(sprite_balon);




    enemigo = new Enemy(900.0f, 500.0f, 4.6f,200.0f, 80.0f, 40.0f, false);

    modoBall modo = modoBall::basketball;
    ComportamientoColision comportamiento = ComportamientoColision :: Rebote;
    balon = new Ball(modo, comportamiento, 3);

    balon->x = 640.0f;
    balon->y = 360.0f;

    balon->recoger(jugador);

    // Carga de los sprites

    sheet_jugador = QPixmap(":assets/player/doom_comportamiento_lvl1.png");
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



    texto_canasta = new QGraphicsTextItem();
    QFont fuente_canasta("Arial", 20, QFont::Bold);
    texto_canasta->setFont(fuente_canasta);
    texto_canasta->setDefaultTextColor(Qt::yellow);
    texto_canasta->setVisible(false);
    texto_canasta->setZValue(2);
    escena->addItem(texto_canasta);


    pantalla_final = new QGraphicsPixmapItem();
    pantalla_final->setZValue(500); // valor muy alto
    pantalla_final->setVisible(false);
    escena->addItem(pantalla_final);

}


void Level_1 :: sumar_puntos_enemy(){

    if( balon->portador == nullptr &&  // agregar esta condicion
        balon->x >= canasta_player_x - config::NIVEL1::RADIO_CESTA &&
        balon->x <= canasta_player_x + config::NIVEL1::RADIO_CESTA &&
        balon->y >= config::NIVEL1::ALTO_CESTA)

    {
        puntos_enemy+= config::NIVEL1::PUNTOS_POR_CANASTA;

        balon->x = 640.0f;
        balon->y = 360.0f;

        texto_canasta->setPlainText("+2");
        texto_canasta->setPos(canasta_player_x, canasta_player_y);
        texto_canasta->setVisible(true);
        timer_texto_canasta = 1.0f; // mostrar 1 segundo

        balon->x = 640.0f;
        balon->y = 360.0f;
        balon->vx = 0.0f;
        balon->vy = 0.0f;
        balon->recoger(jugador); // el jugador siempre recibe el balon despues de una sesta
        timer_invulnerabilidad = 2.0f;
        balon_en_aire = false;


    }


}

void Level_1 :: sumar_puntos_player(){

    if(balon->portador == nullptr &&
        balon->x >= canasta_enemy_x - config::NIVEL1::RADIO_CESTA &&
        balon->x <= canasta_enemy_x + config::NIVEL1::RADIO_CESTA &&
        balon->y >= config::NIVEL1::ALTO_CESTA)
    {
        puntos_player += config::NIVEL1::PUNTOS_POR_CANASTA;
        texto_canasta->setPlainText("+2");
        texto_canasta->setPos(canasta_enemy_x, canasta_enemy_y);
        texto_canasta->setVisible(true);
        timer_texto_canasta = 1.0f;
        balon->x = 640.0f;
        balon->y = 360.0f;
        balon->vx = 0.0f;
        balon->vy = 0.0f;
        balon->recoger(jugador);
        timer_invulnerabilidad = 2.0f;
        balon_en_aire = false;
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


void Level_1::definir_ganador()
{
    if(puntos_player > puntos_enemy)
        ganador = jugador;
    else if(puntos_enemy > puntos_player)
        ganador = enemigo;
    else
        ganador = nullptr;
}


character* Level_1::getganador() const{

    return ganador;
}


float Level_1 :: gettiemporestante()const{

    return tiempo_restante;
}


Ball* Level_1 :: getBalon() const{

    return balon;
}

Enemy* Level_1 :: getEnemigo() const{

    return enemigo;
}

void Level_1 ::setBalonEnAire(bool valor)
{
    balon_en_aire = valor;
    if(valor) timer_invulnerabilidad = 0.5f; // 0.5 segundos

}

void Level_1::lanzar_balon_jugador(float fuerza)
{

    if(balon->portador == jugador)
    {
        jugador->lanzar_balon(*balon, fuerza, canasta_enemy_x, canasta_enemy_y);

        balon_en_aire = true;
        timer_invulnerabilidad = 0.5f;

    }
}



Level_1 :: ~Level_1(){

    delete enemigo;

    enemigo = nullptr;

    delete balon;

    balon = nullptr;

}
