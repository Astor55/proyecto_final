#ifndef LEVEL_1_H
#define LEVEL_1_H
#include "level.h"
#include "ball.h"
#include "enemy.h"
#include "player.h"
#include "character.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>



enum class EstadoAnimacion : unsigned char
{
    CAMINANDO_CON_BALON,  // fila 0
    CORRIENDO_CON_BALON,  // fila 1
    LANZANDO,             // fila 2
    CORRIENDO_SIN_BALON,  // fila 3

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
        bool balon_en_aire = false;
        float timer_invulnerabilidad = 0.0f; // Invulnerabilidad del balon cuando se lanza (Esta en el suelo)
        float timer_invulnerabilidad_enemigo = 0.0f;
        QGraphicsTextItem* texto_canasta;
        float timer_texto_canasta = 0.0f;
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

        QGraphicsTextItem* texto_puntos_player;
        QGraphicsTextItem* texto_puntos_enemy;
        QGraphicsTextItem* texto_tiempo;

        QGraphicsRectItem* rect_player;
        QGraphicsRectItem* rect_enemy;
        QGraphicsTextItem* texto_label_player;
        QGraphicsTextItem* texto_label_enemy;

        QPixmap sheet_balon;
        QGraphicsPixmapItem* sprite_balon;

        QGraphicsPixmapItem* pantalla_final;


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
        Ball* getBalon() const;
        Enemy* getEnemigo() const;
        float getCanasta_enemy_x() const { return canasta_enemy_x; }
        float getCanasta_enemy_y() const { return canasta_enemy_y; }
        void setBalonEnAire(bool valor);
        void lanzar_balon_jugador(float fuerza);

};

#endif // LEVEL_1_H
