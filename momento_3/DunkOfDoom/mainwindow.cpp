#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1280, 720);
    this->setFixedSize(1280, 720);

    QGraphicsView* vista = new QGraphicsView(escena, this);
    vista->setFixedSize(1280, 720);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vista);

    jugador = new player(200.0f, 500.0f, 5.0f, 100.0f, 50.0f, 60.0f, false);

    nivel = new Level_1();
    nivel->inicializacion(jugador, escena);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::game_loop);
    timer->start(1000 / 60);
}


MainWindow::~MainWindow()
{
    delete nivel;
    delete jugador;
    delete ui;

}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_D: tecla_derecha = true; break;
    case Qt::Key_A: tecla_izquierda = true; break;
    case Qt::Key_W: tecla_arriba = true; break;
    case Qt::Key_S: tecla_abajo = true; break;
    case Qt::Key_E:     tecla_arrebatar = true; break;
    case Qt::Key_Space:
        if(!espacio_presionado && !event->isAutoRepeat())
        {
            espacio_presionado = true;
            lanzando = true;
            timer_lanzamiento = 0.0f;
        }
        break;

    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    case Qt::Key_D: tecla_derecha = false; break;
    case Qt::Key_A: tecla_izquierda = false; break;
    case Qt::Key_W: tecla_arriba = false; break;
    case Qt::Key_S: tecla_abajo = false; break;
    case Qt::Key_E:     tecla_arrebatar = false; break;
    case Qt::Key_Space:
        if(!event->isAutoRepeat() && espacio_presionado)
        {
            espacio_presionado = false;
            lanzando = false;
            if(!nivel->getEnCountdown())
                nivel->lanzar_balon_jugador(timer_lanzamiento * 100.0f);
            timer_lanzamiento = 0.0f;
        }
        break;

    }
}

void MainWindow::game_loop()
{
    float dx = 0, dy = 0;
    if(!nivel->getEnCountdown())
    {
        if(tecla_derecha)   dx =  1;
        if(tecla_izquierda) dx = -1;
        if(tecla_arriba)    dy = -1;
        if(tecla_abajo)     dy =  1;
    }

    if(dx != 0 || dy != 0)
        jugador->moverse(dx, dy);
    else
    {
        jugador->setdx(0);
        jugador->setdy(0);
    }

    if(lanzando)
        timer_lanzamiento = qMin(timer_lanzamiento + 1.0f/30.0f, 1.0f); // 6 veces mas rapido

    if(!nivel->getEnCountdown())
    {
        if(tecla_arrebatar)
            jugador->atacar(*nivel->getEnemigo(), *nivel->getBalon());
    }

    nivel->actualizar(1.0f / 60.0f);
}

