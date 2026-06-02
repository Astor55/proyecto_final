#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include "level_1.h"
#include "player.h"
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void game_loop();

private:
    Ui::MainWindow* ui;
    QGraphicsScene* escena;
    QTimer* timer;
    Level_1* nivel;
    player* jugador;
    bool tecla_derecha = false;
    bool tecla_izquierda = false;
    bool tecla_arriba = false;
    bool tecla_abajo = false;
    bool tecla_arrebatar = false;
    bool tecla_lanzar = false;
    float timer_lanzamiento = 0.0f;
    bool lanzando = false;
    bool espacio_presionado = false;
};
#endif // MAINWINDOW_H
