#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "level_2.h"
#include "player.h"
#include "player_sprites.h"
#include <QGraphicsRectItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event)   override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void game_loop();

private:
    Ui::MainWindow* ui;
    QGraphicsScene* escena;
    QTimer*   timer;
    Level_2*  nivel;
    player*   jugador;
    QGraphicsRectItem* rect_jugador;
    //PlayerSprites* sprite_jugador = nullptr;

    bool tecla_derecha   = false;
    bool tecla_izquierda = false;
    bool tecla_arriba    = false;
    bool tecla_abajo     = false;
};

#endif // MAINWINDOW_H
