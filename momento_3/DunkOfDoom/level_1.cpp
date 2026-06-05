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
        else
        {
            // Empate: mostrar texto sobre fondo negro semitransparente
            pantalla_final->setPixmap(QPixmap()); // pixmap vacío
            pantalla_final->setVisible(false);
            if(!texto_canasta->isVisible())
            {
                texto_canasta->setPlainText("¡EMPATE!");
                texto_canasta->setPos(530, 320);
                texto_canasta->setDefaultTextColor(Qt::cyan);
                QFont f("Arial", 40, QFont::Bold);
                texto_canasta->setFont(f);
                texto_canasta->setVisible(true);
            }
        }
        return;
    }

    if(pausado) return;

    // ── TIEMPO ──────────────────────────────────────────────
    tiempo_restante -= dt;
    if(tiempo_restante <= 0)
    {
        tiempo_restante = 0;
        definir_ganador();
        terminado = true;
        return;
    }

    // ── TIMERS ──────────────────────────────────────────────
    if(timer_invulnerabilidad > 0)
        timer_invulnerabilidad -= dt;

    if(timer_canasta_anotada > 0)
    {
        timer_canasta_anotada -= dt;
        if(timer_canasta_anotada <= 0)
            canasta_anotada = false;
    }

    if(timer_texto_canasta > 0)
    {
        timer_texto_canasta -= dt;
        if(timer_texto_canasta <= 0)
            texto_canasta->setVisible(false);
    }

    // timer de animación LANZANDO: vuelve al estado normal al terminar
    if(timer_lanzando > 0.0f)
    {
        timer_lanzando -= dt;
        if(timer_lanzando <= 0.0f)
            timer_lanzando = 0.0f;
    }

    if(en_countdown)
    {
        jugador->setdx(0);
        jugador->setdy(0);

        timer_countdown -= dt;
        if(timer_countdown <= 0.0f)
        {
            cuenta--;
            if(cuenta <= 0)
            {
                en_countdown = false;
                texto_countdown->setVisible(false);
                // El balon queda libre en el centro: quien llegue primero lo agarra
                balon->soltar();
                balon->activa = true;
                balon->vx = 0.0f;
                balon->vy = 0.0f;
            }
            else
            {
                timer_countdown = 1.0f;
                texto_countdown->setPlainText(QString::number(cuenta));
            }
        }
    }

    // ── BALON ───────────────────────────────────────────────
    if(!en_countdown)
    {
        balon->actualizar(dt);

        balon->verificar_colision_paredes(1280.0f, 720.0f);

        if(balon->portador == nullptr && timer_invulnerabilidad <= 0)
        {
            balon->verificar_colision_charater(jugador);
            balon->verificar_colision_charater(enemigo);
        }
    }

    balon_en_aire = (balon->portador == nullptr && balon->activa);

    // ── PUNTOS ──────────────────────────────────────────────
    sumar_puntos_player();
    sumar_puntos_enemy();

    // ── ENEMIGO (IA) ─────────────────────────────────────────
    if(!en_countdown)
    {
        enemigo->percepcion(*jugador, *balon, dt, canasta_player_x, canasta_player_y);
        enemigo->razonamiento();
        enemigo->accion(*jugador, *balon, canasta_player_x, canasta_player_y, timer_invulnerabilidad);
    }
    else
    {
        enemigo->setdx(0);
        enemigo->setdy(0);
    }

    // limitar dentro de la pantalla
    if(jugador->get_x() < 150)  jugador->setx(150);
    if(jugador->get_x() > 1045) jugador->setx(1045);
    if(jugador->get_y() < 350)  jugador->sety(350);
    if(jugador->get_y() > 600)  jugador->sety(600);

    // ── ESTADOS DE ANIMACION ─────────────────────────────────
    if(timer_lanzando > 0.0f)
    {
        estado_jugador = EstadoAnimacion::LANZANDO;
    }
    else if(balon->portador == jugador)
    {
        estado_jugador = (jugador->getdx_actual() != 0 || jugador->getdy_actual() != 0)
        ? EstadoAnimacion::CORRIENDO_CON_BALON
        : EstadoAnimacion::IDLE_CON_BALON;
    }
    else
    {
        estado_jugador = (jugador->getdx_actual() != 0 || jugador->getdy_actual() != 0)
        ? EstadoAnimacion::CORRIENDO_SIN_BALON
        : EstadoAnimacion::IDLE;
    }

    if(balon->portador == enemigo)
        estado_enemigo = (enemigo->getdx_actual() != 0 || enemigo->getdy_actual() != 0)
                             ? EstadoAnimacion::CORRIENDO_CON_BALON
                             : EstadoAnimacion::IDLE_CON_BALON;
    else
        estado_enemigo = (enemigo->getdx_actual() != 0 || enemigo->getdy_actual() != 0)
                             ? EstadoAnimacion::CORRIENDO_SIN_BALON
                             : EstadoAnimacion::IDLE;

    // ── TIMERS DE FRAMES ─────────────────────────────────────
    timer_frame_jugador += dt;
    timer_frame_enemigo += dt;

    static constexpr float DURACION_FRAME = 0.15f;

    if(estado_jugador != estado_anterior_jugador)
    {
        frame_jugador = 0;
        timer_frame_jugador = 0.0f;
        estado_anterior_jugador = estado_jugador;
    }

    if(timer_frame_jugador >= DURACION_FRAME)
    {
        timer_frame_jugador = 0.0f;
        frame_jugador = (frame_jugador + 1) % 8;
    }

    if(timer_frame_enemigo >= DURACION_FRAME)
    {
        timer_frame_enemigo = 0.0f;
        frame_enemigo = (frame_enemigo + 1) % 6;
    }

    // ── SPRITES ──────────────────────────────────────────────
    auto calcular_fila_jugador = [](EstadoAnimacion estado) -> int {
        switch(estado) {
        case EstadoAnimacion::IDLE_CON_BALON:       return 0;
        case EstadoAnimacion::CORRIENDO_CON_BALON:  return 1;
        case EstadoAnimacion::LANZANDO:             return 2;
        case EstadoAnimacion::CORRIENDO_SIN_BALON:  return 3;
        case EstadoAnimacion::IDLE:                 return 4;
        default:                                    return 4;
        }
    };

    auto calcular_fila_enemigo = [](EstadoAnimacion estado) -> int {
        switch(estado) {
        case EstadoAnimacion::IDLE_CON_BALON:       return 0;
        case EstadoAnimacion::CORRIENDO_CON_BALON:  return 1;
        case EstadoAnimacion::LANZANDO:             return 2;
        case EstadoAnimacion::CORRIENDO_SIN_BALON:  return 3;
        case EstadoAnimacion::IDLE:                 return 3; // enemigo no tiene fila IDLE propia → reusar CORR_SIN_BALON
        default:                                    return 3;
        }
    };

    // ── JUGADOR ──────────────────────────────────────────────
    // Escalamos a 160px de alto (más grande, se ve mejor en cancha 1280x720)
    static constexpr int JUGADOR_DEST_H = 100;
    static constexpr int JUGADOR_DEST_W = 100;

    int fila = calcular_fila_jugador(estado_jugador);
    QPixmap recorte_jugador = sheet_jugador.copy(
        FILA_COL_X[frame_jugador],
        FILA_Y[fila],
        FW,
        FILA_H[fila]);

    recorte_jugador = recorte_jugador.scaled(
        JUGADOR_DEST_W, JUGADOR_DEST_H,
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

    if(jugador->getdx_actual() < 0)
    {
        QTransform flip;
        flip.scale(-1, 1);
        recorte_jugador = recorte_jugador.transformed(flip);
    }
    sprite_jugador->setPixmap(recorte_jugador);
    sprite_jugador->setPos(jugador->getx() - JUGADOR_DEST_W / 2,
                           jugador->gety() - JUGADOR_DEST_H);

    // ── ENEMIGO ──────────────────────────────────────────────
    static constexpr int ENEMIGO_DEST_H = 120;
    static constexpr int ENEMIGO_DEST_W = 120;
    static constexpr int ENEMIGO_OFFSET_PIES[4] = {111, 106, 119, 103};

    int fila_e = calcular_fila_enemigo(estado_enemigo);
    QPixmap recorte_enemigo = sheet_enemigo.copy(
        frame_enemigo * FW_ENEMIGO,
        FILA_Y_ENEMIGO[fila_e],
        FW_ENEMIGO, FH_ENEMIGO);

    recorte_enemigo = recorte_enemigo.scaled(
        ENEMIGO_DEST_W, ENEMIGO_DEST_H,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation);

    if(enemigo->getdx_actual() < 0)
    {
        QTransform flip;
        flip.scale(-1, 1);
        recorte_enemigo = recorte_enemigo.transformed(flip);
    }
    sprite_enemigo->setPixmap(recorte_enemigo);
    sprite_enemigo->setPos(enemigo->getx() - recorte_enemigo.width() / 2,
                           enemigo->gety() - ENEMIGO_OFFSET_PIES[fila_e]);

    // ── BALON SPRITE ─────────────────────────────────────────
    if(balon_en_aire)
    {
        sprite_balon->setPos(balon->x - 30, balon->y - 30);
        sprite_balon->setVisible(true);
    }
    else
        sprite_balon->setVisible(false);

    // ── HUD ──────────────────────────────────────────────────
    texto_puntos_player->setPlainText(QString::number(puntos_player));
    texto_puntos_enemy->setPlainText(QString::number(puntos_enemy));
    texto_tiempo->setPlainText(QString::number((int)tiempo_restante));
}


void Level_1 :: inicializacion(player* p, QGraphicsScene* escena){

    iniciar_musica(config::Assets::MUSICA_NIVEL1);

    // fondo - se agrega primero para que quede detras de todo
    QPixmap fondo(":/assets/sprites/level1.doom.png");
    fondo = fondo.scaled(1280, 720); // escalar al tamaño de la pantalla
    QGraphicsPixmapItem* sprite_fondo = new QGraphicsPixmapItem(fondo);
    sprite_fondo->setZValue(-1); // asegura que quede detras de los personajes
    escena->addItem(sprite_fondo);


    jugador = p;
    jugador->set_velocidad(config::NIVEL1::VEL_JUGADOR);
    this->escena = escena;

    canasta_player_x = 145;
    canasta_player_y = 282;

    canasta_enemy_x  = 1132;
    canasta_enemy_y  = 282;

    qDebug() << "Canasta enemiga inicializada en : " << float(canasta_enemy_x) << float(canasta_enemy_y);

    jugador->setx(440.0f);
    jugador->sety(500.0f);

    // crear texto_puntos_player antes de usarlo
    texto_puntos_player = new QGraphicsTextItem();
    texto_puntos_enemy  = new QGraphicsTextItem();
    texto_tiempo        = new QGraphicsTextItem();

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

    sheet_balon = QPixmap(":/assets/ball.png");
    sprite_balon = new QGraphicsPixmapItem();
    sprite_balon->setPixmap(sheet_balon.scaled(60, 60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    sprite_balon->setZValue(100);
    sprite_balon->setVisible(false);
    escena->addItem(sprite_balon);

    enemigo = new Enemy(840.0f, 500.0f, 4.6f, 200.0f, 80.0f, 65.0f, false);

    modoBall modo = modoBall::basketball;
    ComportamientoColision comportamiento = ComportamientoColision :: Rebote;
    balon = new Ball(modo, comportamiento, 3);

    balon->x = 640.0f;
    balon->y = 360.0f;

    balon->recoger(jugador);

    // Carga de los sprites
    sheet_jugador = QPixmap(":/assets/player/Doom_comportamientos_level1.png");
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
    estado_jugador          = EstadoAnimacion::CORRIENDO_SIN_BALON;
    estado_anterior_jugador = EstadoAnimacion::CORRIENDO_SIN_BALON;
    estado_enemigo          = EstadoAnimacion::CORRIENDO_SIN_BALON;
    frame_jugador = 0; // Frame dentro del mapa de sprites, que sea el primero y no uno aleatorio
    frame_enemigo = 0;
    timer_frame_jugador = 0.0f; // Tiempo que demora en cambiar de sprite
    timer_frame_enemigo = 0.0f;
    timer_lanzando      = 0.0f;

    texto_canasta = new QGraphicsTextItem();

    texto_countdown = new QGraphicsTextItem();
    QFont fuente_countdown("Arial", 80, QFont::Bold);
    texto_countdown->setFont(fuente_countdown);
    texto_countdown->setDefaultTextColor(Qt::red);
    texto_countdown->setVisible(false);
    texto_countdown->setZValue(10);
    escena->addItem(texto_countdown);

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


void Level_1::sumar_puntos_enemy()
{

    if(canasta_anotada) return;

    if(balon->portador == nullptr &&
        balon->x >= canasta_player_x - config::NIVEL1::RADIO_CESTA &&
        balon->x <= canasta_player_x + config::NIVEL1::RADIO_CESTA &&
        balon->y >= config::NIVEL1::ALTO_CESTA - 30.0f &&
        balon->y <= config::NIVEL1::ALTO_CESTA + 60.0f)
    {
        canasta_anotada = true;
        timer_canasta_anotada = 0.5f;
        puntos_enemy += config::NIVEL1::PUNTOS_POR_CANASTA;

        texto_canasta->setPlainText("+2");
        texto_canasta->setPos(canasta_player_x, canasta_player_y);
        texto_canasta->setVisible(true);
        timer_texto_canasta = 1.0f;

        // resetear posiciones equidistantes del centro
        jugador->setx(440.0f);
        jugador->sety(500.0f);
        enemigo->setx(840.0f);
        enemigo->sety(500.0f);

        // balon al centro sin portador
        balon->x  = 640.0f;
        balon->y  = 360.0f;
        balon->vx = 0.0f;
        balon->vy = 0.0f;
        balon->portador = nullptr;

        timer_invulnerabilidad = 2.0f;
        balon_en_aire = false;

        // iniciar countdown
        en_countdown  = true;
        cuenta        = 3;
        timer_countdown = 1.0f;
        texto_countdown->setPlainText("3");
        texto_countdown->setPos(610, 280);
        texto_countdown->setVisible(true);
    }
}

void Level_1::sumar_puntos_player()
{

    if(canasta_anotada) return;

    if(balon->portador == nullptr &&
        balon->x >= canasta_enemy_x - config::NIVEL1::RADIO_CESTA &&
        balon->x <= canasta_enemy_x + config::NIVEL1::RADIO_CESTA &&
        balon->y >= config::NIVEL1::ALTO_CESTA - 30.0f &&
        balon->y <= config::NIVEL1::ALTO_CESTA + 60.0f)
    {
        canasta_anotada = true;
        timer_canasta_anotada = 0.5f;
        puntos_player += config::NIVEL1::PUNTOS_POR_CANASTA;

        texto_canasta->setPlainText("+2");
        texto_canasta->setPos(canasta_enemy_x, canasta_enemy_y);
        texto_canasta->setVisible(true);
        timer_texto_canasta = 1.0f;

        // resetear posiciones equidistantes del centro
        jugador->setx(440.0f);
        jugador->sety(500.0f);
        enemigo->setx(840.0f);
        enemigo->sety(500.0f);

        // balon al centro sin portador
        balon->x  = 640.0f;
        balon->y  = 360.0f;
        balon->vx = 0.0f;
        balon->vy = 0.0f;
        balon->portador = nullptr;

        timer_invulnerabilidad = 2.0f;
        balon_en_aire = false;

        // iniciar countdown
        en_countdown  = true;
        cuenta        = 3;
        timer_countdown = 1.0f;
        texto_countdown->setPlainText("3");
        texto_countdown->setPos(610, 280);
        texto_countdown->setVisible(true);
    }
}

void Level_1 :: finalizar(){

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
        float objetivo_x = (jugador->getdx_actual() >= 0) ? canasta_enemy_x : canasta_player_x;
        float objetivo_y = canasta_enemy_y;
        jugador->lanzar_balon(*balon, fuerza, objetivo_x, objetivo_y);
        balon_en_aire = true;
        timer_invulnerabilidad = 0.5f;
        timer_lanzando = 0.45f;
    }
}


Level_1 :: ~Level_1(){

    delete enemigo;
    enemigo = nullptr;

    delete balon;
    balon = nullptr;

}
