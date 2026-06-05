#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsRectItem>

#include "level.h"
#include "level_2.h"
#include "juego.h"
#include "player.h"

enum class EstadoJuego
{
    JUGANDO,
    GAME_OVER,
    VICTORIA
};

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
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
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void game_loop();

private:

    Juego* juego = nullptr;
    enum class EstadoApp { MENU, JUGANDO_NIVEL } estado_app = EstadoApp::MENU;

    EstadoJuego estado_juego = EstadoJuego::JUGANDO;

    void reiniciar_nivel();
    void ir_al_menu();
    void mostrar_pantalla_final(bool gano);
    void limpiar_pantalla_final();
    void reproducir_musica_final(bool gano);
    void mostrar_pausa();
    void ocultar_pausa();

    Ui::MainWindow* ui;
    QGraphicsScene* escena;
    QTimer* timer;
    Level* nivel;
    player* jugador;
    QGraphicsRectItem* rect_jugador = nullptr;

    // pantalla final
    QGraphicsPixmapItem* fondo_final = nullptr;
    QGraphicsRectItem* btn_reintentar = nullptr;
    QGraphicsRectItem* btn_menu = nullptr;
    QGraphicsTextItem* txt_reintentar = nullptr;
    QGraphicsTextItem* txt_menu = nullptr;
    QMediaPlayer* musica_final = nullptr;
    QAudioOutput* audio_final = nullptr;

    // pausa
    QGraphicsRectItem* overlay_pausa = nullptr;
    QGraphicsTextItem* txt_reanudar = nullptr;
    QGraphicsTextItem* txt_reintentar_p = nullptr;
    QGraphicsTextItem* txt_menu_p = nullptr;

    // teclas comunes
    bool tecla_derecha = false;
    bool tecla_izquierda = false;
    bool tecla_arriba = false;
    bool tecla_abajo = false;

    // teclas level_1
    bool tecla_arrebatar = false;
    bool espacio_presionado = false;
    bool lanzando = false;
    float timer_lanzamiento = 0.0f;

    // estado
    bool esperando_final = false;
    bool gano_nivel = false;
    bool pausado = false;
    bool es_dificil = true;
    bool es_nivel_1 = false;

    float delay_final = 0.0f;
    float volumen_global = 0.5f;

    static constexpr float DELAY_FINAL = 2.5f;
};

#endif // MAINWINDOW_H
