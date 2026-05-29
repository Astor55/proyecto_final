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
    vista->setRenderHint(QPainter::Antialiasing, false);
    vista->setRenderHint(QPainter::SmoothPixmapTransform, false);
    setCentralWidget(vista);

    jugador = new player(640.0f, 360.0f, 150.0f, 80.0f, 40.0f, false);
    nivel   = new Level_2(false);
    nivel->inicializacion(jugador, escena);

    rect_jugador = escena->addRect(0, 0, 30, 30, QPen(Qt::green), QBrush(Qt::green));

    rect_jugador->setPos(jugador->get_x(), jugador->get_y());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::game_loop);
    timer->start(1000 / 60);
}

MainWindow::~MainWindow() {
    delete nivel;
    delete jugador;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_D: tecla_derecha   = true; break;
    case Qt::Key_A: tecla_izquierda = true; break;
    case Qt::Key_W: tecla_arriba    = true; break;
    case Qt::Key_S: tecla_abajo     = true; break;
    case Qt::Key_Escape: close();           break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_D: tecla_derecha   = false; break;
    case Qt::Key_A: tecla_izquierda = false; break;
    case Qt::Key_W: tecla_arriba    = false; break;
    case Qt::Key_S: tecla_abajo     = false; break;
    }
}
void MainWindow::game_loop() {
    float dx = 0, dy = 0;

    if(tecla_derecha)   dx =  1;
    if(tecla_izquierda) dx = -1;
    if(tecla_arriba)    dy = -1;
    if(tecla_abajo)     dy =  1;

    // Normalizar diagonal
    if(dx != 0 && dy != 0) {
        dx *= 0.707f;
        dy *= 0.707f;
    }

    // Guardar posición y mover — solo una vez
    if(dx != 0 || dy != 0)
    {
        jugador->moverse(dx, dy);
    }

    nivel->actualizar(config::DELTA_TIME);
    rect_jugador->setPos(jugador->get_x(), jugador->get_y());

    // Debug — ver posición cuando intentas cruzar al puente
    qDebug() << "x:" << jugador->get_x()
             << "y:" << jugador->get_y();
}
