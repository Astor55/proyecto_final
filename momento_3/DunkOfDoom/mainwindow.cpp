#include "mainwindow.h"
#include "ui_mainwindow.h"

void test_ball();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ── Configurar ventana ──
    setFixedSize(1280, 720);

    // ── Crear escena ──
    escena = new QGraphicsScene(0, 0, 1280, 720, this);
    escena->setBackgroundBrush(Qt::black);

    // ── Crear círculo que representa la pelota ──
    circulo = escena->addEllipse(0, 0, 30, 30,
                                 QPen(Qt::red),
                                 QBrush(Qt::red));

    // ── Asignar escena a la vista del ui ──
    ui->graphicsView->setScene(escena);
    ui->graphicsView->setFixedSize(1280, 720);

    // ── Crear pelota en modo BASKETBALL ──
    balon = new Ball(modoBall::basketball);
    balon->x  = 640.0f;
    balon->y  = 100.0f;
    balon->lanzar(300.0f, 0.785f); // 45 grados

    // ── Timer 60fps ──
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizar);
    timer->start(16);
}

MainWindow::~MainWindow() {
    delete balon;
    delete ui;
}

void MainWindow::actualizar() {
    balon->actualizar(0.016f);
    balon->verificar_colision_paredes(1280.0f - 15.0f, 720.0f - 15.0f);

    // Mover círculo a la posición de la pelota
    circulo->setPos(balon->x - 15.0f, balon->y - 15.0f);
}
