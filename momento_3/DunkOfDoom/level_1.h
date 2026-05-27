#ifndef LEVEL_1_H
#define LEVEL_1_H
#include "level.h"
#include "ball.h"
#include "enemy.h"
#include "player.h"
#include "character.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>


enum class EstadoAnimacion : unsigned char
{
    CAMINANDO_CON_BALON,
    CORRIENDO_CON_BALON,
    LANZANDO,
    CORRIENDO_SIN_BALON
};

class Level_1 : public Level
{

    private:

        unsigned short puntos_player;
        unsigned short puntos_enemy;
        float tiempo_restante; // El nivel dura 90 segundos pero este atributo es para tener un contador
        float canasta_enemy_x;
        float canasta_enemy_y;
        float canasta_player_x;
        float canasta_player_y;
        Ball* balon;
        Enemy* enemigo;
        character* ganador;

        // Capa grafica

        QPixmap sheet_jugador;
        QPixmap sheet_enemigo;
        QGraphicsPixmapItem* sprite_jugador;
        QGraphicsPixmapItem* sprite_enemigo;
        EstadoAnimacion estado_jugador;
        EstadoAnimacion estado_enemigo;
        EstadoAnimacion estado_anterior_jugador;
        int frame_jugador;
        int frame_enemigo;
        float timer_frame_jugador;
        float timer_frame_enemigo;


    public:

        Level_1();

        ~Level_1() override;

        void inicializacion(player* p, QGraphicsScene* escena) override;
        void actualizar(float dt) override;
        void finalizar() override;

        void sumar_puntos_enemy();
        void sumar_puntos_player();
        void definir_ganador();

        character* getganador() const;
        float gettiemporestante()const;

};

#endif // LEVEL_1_H
