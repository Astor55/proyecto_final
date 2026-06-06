#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    setWindowIcon(QIcon(config::Assets::ICON));

    ui->setupUi(this);

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1280, 720);
    this->setFixedSize(1280, 720);

    QGraphicsView* vista = new QGraphicsView(escena, this);
    vista->setFixedSize(1280, 720);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setRenderHint(QPainter::Antialiasing, false);
    vista->setRenderHint(QPainter::SmoothPixmapTransform, false);
    setCentralWidget(vista);

    juego = new Juego(escena, volumen_global);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::game_loop);
    timer->start(1000 / 60);

}

MainWindow::~MainWindow()
{

    if(nivel) { delete nivel; nivel = nullptr; }
    if(jugador) { delete jugador; jugador = nullptr; }

    juego->transferir_ownership(); // asegura que juego no haga doble delete
    delete juego;
    delete ui;

    if(musica_final)
    {
        musica_final->stop();
        delete musica_final;
        musica_final = nullptr;
    }

    if(audio_final)
    {
        delete audio_final;
        audio_final = nullptr;
    }
}


void MainWindow::keyPressEvent(QKeyEvent* event)
{
    // Input del menú
    if(estado_app == EstadoApp::MENU)
    {
        switch(event->key())
        {
        case Qt::Key_Return:
        case Qt::Key_Space: juego->confirmar();  break;
        case Qt::Key_Escape: juego->volver();    break;
        default: break;
        }
        return;
    }

    // Input del juego
    switch(event->key())
    {
    case Qt::Key_D: tecla_derecha   = true; break;
    case Qt::Key_A: tecla_izquierda = true; break;
    case Qt::Key_W: tecla_arriba    = true; break;
    case Qt::Key_S: tecla_abajo     = true; break;
    case Qt::Key_E: tecla_arrebatar = true; break;

    case Qt::Key_Space:
        if(!espacio_presionado && !event->isAutoRepeat())
        {
            espacio_presionado = true;
            lanzando           = true;
            timer_lanzamiento  = 0.0f;
        }
        break;

    case Qt::Key_Escape:
        if(estado_juego == EstadoJuego::JUGANDO)
            pausado ? ocultar_pausa() : mostrar_pausa();
        break;

    case Qt::Key_Plus:
        volumen_global = qMin(1.0f, volumen_global + 0.1f);
        if(nivel) nivel->set_volumen(volumen_global);
        if(musica_final) audio_final->setVolume(volumen_global);
        break;

    case Qt::Key_Minus:
        volumen_global = qMax(0.0f, volumen_global - 0.1f);
        if(nivel) nivel->set_volumen(volumen_global);
        if(musica_final) audio_final->setVolume(volumen_global);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(estado_app == EstadoApp::MENU) return;

    switch(event->key())
    {
    case Qt::Key_D: tecla_derecha   = false; break;
    case Qt::Key_A: tecla_izquierda = false; break;
    case Qt::Key_W: tecla_arriba    = false; break;
    case Qt::Key_S: tecla_abajo     = false; break;
    case Qt::Key_E: tecla_arrebatar = false; break;

    case Qt::Key_Space:
        if(!event->isAutoRepeat() && espacio_presionado)
        {
            espacio_presionado = false;
            lanzando           = false;
            if(nivel && !nivel->getEnCountdown())
                nivel->lanzar_balon_jugador(timer_lanzamiento * 100.0f);
            timer_lanzamiento = 0.0f;
        }
        break;
    }
}


void MainWindow::mousePressEvent(QMouseEvent* event)
{

    if(estado_app == EstadoApp::MENU)
    {

        juego->click(event->pos().x(), event->pos().y());
        return;

    }

    if(pausado)
    {

        QPointF pos = event->pos();

        if(txt_reanudar && txt_reanudar->sceneBoundingRect().contains(pos))
        {

            ocultar_pausa();
            return;

        }

        if(txt_reintentar_p && txt_reintentar_p->sceneBoundingRect().contains(pos))
        {

            reiniciar_nivel();
            return;

        }

        if(txt_menu_p && txt_menu_p->sceneBoundingRect().contains(pos))
        {

            ir_al_menu();
            return;

        }
    }

    if(estado_juego == EstadoJuego::JUGANDO) return;

    QPointF pos = event->pos();

    if(btn_reintentar && btn_reintentar->rect().contains(pos))
        reiniciar_nivel();

    if(btn_menu && btn_menu->rect().contains(pos))
        ir_al_menu();

}

void MainWindow::reiniciar_nivel()
{

    ocultar_pausa();
    limpiar_pantalla_final();
    if(musica_final) musica_final->stop();

    escena->clear();
    delete nivel;   nivel   = nullptr;
    delete jugador; jugador = nullptr;

    juego->reiniciar(escena);

    jugador = juego->get_jugador();
    nivel   = juego->get_nivel();
    juego->transferir_ownership();

    nivel->inicializacion(jugador, escena);
    nivel->set_volumen(volumen_global);

    estado_juego    = EstadoJuego::JUGANDO;
    esperando_final = false;
    timer->start(1000 / 60);

}

void MainWindow::ir_al_menu()
{
    ocultar_pausa();
    limpiar_pantalla_final();

    if(musica_final) musica_final->stop();

    delete nivel;   nivel   = nullptr;
    delete jugador; jugador = nullptr;

    escena->clear();

    juego->mostrar_pantalla_principal();

    estado_app   = EstadoApp::MENU;
    estado_juego = EstadoJuego::JUGANDO;

    esperando_final = false;
    timer->start(1000 / 60);
}

void MainWindow::limpiar_pantalla_final()
{
    fondo_final    = nullptr;
    btn_reintentar = nullptr;
    btn_menu       = nullptr;
    txt_reintentar = nullptr;
    txt_menu       = nullptr;
}

void MainWindow::mostrar_pantalla_final(bool gano)
{
    timer->stop();
    reproducir_musica_final(gano);

    QString img = gano
                      ? (juego->es_nivel1() ? config::Assets::VICTORY1 : config::Assets::VICTORY2)
                      : (juego->es_nivel1() ? config::Assets::GAME_OVER1 : config::Assets::GAME_OVER2);

    QPixmap px(img);
    px = px.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fondo_final = escena->addPixmap(px);
    fondo_final->setZValue(10);

    btn_reintentar = escena->addRect(440, 580, 180, 50, QPen(Qt::red, 2), QBrush(QColor(80, 0, 0, 200)));
    btn_reintentar->setZValue(11);
    txt_reintentar = escena->addText("REINTENTAR");
    txt_reintentar->setDefaultTextColor(Qt::red);
    txt_reintentar->setZValue(11);
    txt_reintentar->setPos(455, 593);

    btn_menu = escena->addRect(660, 580, 180, 50, QPen(Qt::red, 2), QBrush(QColor(80, 0, 0, 200)));
    btn_menu->setZValue(11);
    txt_menu = escena->addText("MENU PRINCIPAL");
    txt_menu->setDefaultTextColor(Qt::red);
    txt_menu->setZValue(11);
    txt_menu->setPos(668, 593);
}


void MainWindow::mostrar_pausa()
{
    pausado = true;
    nivel->pausar();
    timer->stop();
    nivel->bajar_volumen(0.3f);

    int id = QFontDatabase::addApplicationFont(config::Assets::FUENTE_DOOM);
    QString familia = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fuente_doom(familia, 28);

    overlay_pausa = escena->addRect(0, 0, 1280, 720, Qt::NoPen, QBrush(QColor(0, 0, 0, 160)));
    overlay_pausa->setZValue(20);

    txt_reanudar = escena->addText("REANUDAR", fuente_doom);
    txt_reanudar->setDefaultTextColor(Qt::red);
    txt_reanudar->setZValue(21);
    txt_reanudar->setPos(640 - txt_reanudar->boundingRect().width() / 2, 260);

    txt_reintentar_p = escena->addText("REINTENTAR", fuente_doom);
    txt_reintentar_p->setDefaultTextColor(Qt::red);
    txt_reintentar_p->setZValue(21);
    txt_reintentar_p->setPos(640 - txt_reintentar_p->boundingRect().width() / 2, 340);

    txt_menu_p = escena->addText("MENU PRINCIPAL", fuente_doom);
    txt_menu_p->setDefaultTextColor(Qt::red);
    txt_menu_p->setZValue(21);
    txt_menu_p->setPos(640 - txt_menu_p->boundingRect().width() / 2, 420);
}

void MainWindow::ocultar_pausa()
{
    if(!pausado) return;
    pausado = false;
    if(nivel) nivel->reanudar();

    if(overlay_pausa)    { escena->removeItem(overlay_pausa);    delete overlay_pausa;    overlay_pausa    = nullptr; }
    if(txt_reanudar)     { escena->removeItem(txt_reanudar);     delete txt_reanudar;     txt_reanudar     = nullptr; }
    if(txt_reintentar_p) { escena->removeItem(txt_reintentar_p); delete txt_reintentar_p; txt_reintentar_p = nullptr; }
    if(txt_menu_p)       { escena->removeItem(txt_menu_p);       delete txt_menu_p;       txt_menu_p       = nullptr; }

    if(nivel) nivel->subir_volumen(0.7f);
    timer->start(1000 / 60);
}

void MainWindow::reproducir_musica_final(bool gano)
{
    if(!musica_final)
    {
        musica_final = new QMediaPlayer(this);
        audio_final  = new QAudioOutput(this);
        musica_final->setAudioOutput(audio_final);
        audio_final->setVolume(volumen_global);
    }
    QString ruta = gano ? config::Assets::MUSICA_VICTORY : config::Assets::MUSICA_GAME_OVER;
    musica_final->setSource(QUrl::fromLocalFile(ruta));
    musica_final->setLoops(QMediaPlayer::Infinite);
    musica_final->play();
}

void MainWindow::game_loop()
{
    // ── MENÚ ────────────────────────────────────────────────
    if(estado_app == EstadoApp::MENU)
    {

        juego->actualizar_menu();

        if(juego->listo_para_jugar())
        {
            jugador  = juego->get_jugador();
            nivel    = juego->get_nivel();
            juego->transferir_ownership(); // null a los punteros para evitar doble delete
            juego->detener_musica_menu();
            juego->limpiar_menu();
            nivel->inicializacion(jugador, escena);
            nivel->set_volumen(volumen_global);
            estado_app   = EstadoApp::JUGANDO_NIVEL;
            estado_juego = EstadoJuego::JUGANDO;
        }

        return;

    }

    // ── JUGANDO ──────────────────────────────────────────────
    if(estado_juego != EstadoJuego::JUGANDO) return;

    float dx = 0, dy = 0;

    if(!nivel->getEnCountdown())
    {
        if(tecla_derecha)   dx =  1;
        if(tecla_izquierda) dx = -1;
        if(tecla_arriba)    dy = -1;
        if(tecla_abajo)     dy =  1;
    }

    if(dx != 0 && dy != 0) { dx *= 0.707f; dy *= 0.707f; }

    if(dx != 0 || dy != 0) jugador->moverse(dx, dy);
    else                    jugador->resetear_direccion();

    if(!nivel->getEnCountdown() && tecla_arrebatar)
        jugador->atacar(*nivel->getEnemigo(), *nivel->getBalon());

    if(lanzando)
        timer_lanzamiento = qMin(timer_lanzamiento + 1.0f / 30.0f, 1.0f);

    nivel->actualizar(config::DELTA_TIME);

    if(nivel->esta_terminado() && !esperando_final)
    {
        gano_nivel      = nivel->jugador_sobrevivio();
        esperando_final = true;
        delay_final     = DELAY_FINAL;
    }

    if(esperando_final)
    {
        delay_final -= config::DELTA_TIME;
        if(delay_final <= 0)
        {
            esperando_final = false;
            estado_juego = gano_nivel ? EstadoJuego::VICTORIA : EstadoJuego::GAME_OVER;
            nivel->finalizar();
            escena->clear();
            mostrar_pantalla_final(gano_nivel);
        }
    }
}

