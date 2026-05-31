#ifndef BOSS_SPRITES_H
#define BOSS_SPRITES_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <vector>

enum class EstadoBoss
{

    idle,
    Cargando,
    Disparando

};

class BossSprites
{

public:

    //constructor y destructor
    BossSprites(QGraphicsScene* escena, float escala = 0.8f);
    ~BossSprites() = default;

    //evitar copias
    BossSprites(const BossSprites& otro) = delete;
    BossSprites& operator=(const BossSprites& otro) = delete;

    //setters
    void set_Estado(EstadoBoss e);
    void set_pos(float x, float y);
    //void set_rotacion(float angulo_rad);
    void set_flip(bool jugador_a_la_derecha);
    void set_visible(bool v);

    QGraphicsPixmapItem* get_item() const { return item; }

private:

    QGraphicsPixmapItem * item;
    std::vector<QPixmap> frames;

    float escala;

    static constexpr unsigned short COLS   = 3;
    static constexpr unsigned short FY     = 25;
    static constexpr unsigned short FH     = 188;

    static constexpr unsigned short FRAME_X[3] = {192, 502, 815};
    static constexpr unsigned short FRAME_W[3] = {177, 166, 206};
    static constexpr unsigned short FW_MAX     = 206;

    //funciones internas
    void precargar_frames();
    void aplicar_frame(unsigned short indice);

};

#endif // BOSS_SPRITES_H
