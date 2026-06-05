#ifndef JUEGO_H
#define JUEGO_H

#include "level.h"
#include "level_2.h"
#include "player.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <vector>

enum class EstadoMenu : unsigned char
{

    INTRO_LOGO,          // pantalla negra con logo
    INTRO_FONDO,         // fondo aparece
    PRINCIPAL,           //pantalla inicial con logo
    SELECCION_NIVEL,     // elegir nivel 1 o nivel 2
    DIFICULTAD,          // solo si eligió nivel 2
    CONTROLES,           // pantalla de controles

};

class Juego
{

public:

    //constructor y destructor
    Juego(QGraphicsScene* escena, float volumen);
    ~Juego();

    //prohibir copias
    Juego(const Juego& otro) = delete;
    Juego& operator=(const Juego&) = delete;

    //Llamar cada frame mientras está en el menú
    void actualizar_menu();

    // input del menu
    void confirmar();
    void click(float x, float y);

    //getters para el mainwindow
    bool listo_para_jugar() const { return nivel != nullptr; }
    bool es_nivel1() const { return nivel_elegido == 1; }

    Level* get_nivel() const { return nivel; }
    player* get_jugador() const { return jugador; }

    void limpiar_menu();
    void mostrar_pantalla_principal();
    void volver();
    void reiniciar(QGraphicsScene* escena);
    void mostrar_opciones_principal();
    void entrar_submenu();
    void salir_submenu();
    void detener_musica_menu();

    void transferir_ownership() { nivel = nullptr; jugador = nullptr; }

private:

    float timer_intro = 0.0f;
    QMediaPlayer* musica_menu   = nullptr;
    QAudioOutput* audio_menu    = nullptr;

    QGraphicsScene* escena;
    float volumen;

    EstadoMenu estado_menu = EstadoMenu::PRINCIPAL;

    Level*  nivel   = nullptr;
    player* jugador = nullptr;

    int opcion_seleccionada = 0;
    int nivel_elegido       = 1;
    bool dificil            = false;

    // Elementos gráficos del menú
    QGraphicsPixmapItem* fondo_menu  = nullptr;
    QGraphicsPixmapItem* logo        = nullptr;
    std::vector<QGraphicsTextItem*> opciones;
    std::vector<QGraphicsTextItem*> textos_info;
    int num_opciones = 0;

    QFont fuente_doom;

    void mostrar_seleccion_nivel();
    void mostrar_dificultad();
    void mostrar_controles();
    void resaltar_opcion();
    void limpiar_opciones();
    void crear_nivel();

};

#endif // JUEGO_H
