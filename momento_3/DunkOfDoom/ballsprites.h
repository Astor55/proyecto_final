#ifndef BALLSPRITES_H
#define BALLSPRITES_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <vector>
#include "ball.h"


class BallSprites
{

public:

    //constructor y destructor
    BallSprites(QGraphicsScene* escena, float escala = 0.5f);
    ~BallSprites() = default;

    //prohibir copias
    BallSprites(const BallSprites& otro) = delete;
    BallSprites& operator=(const BallSprites& otro) = delete;

    //actualizar
    void actualizar(const Ball& ball, float dt);

    //setter
    void set_visible(bool v);

    bool lista_para_limpiar() const { return destruida && timer_destruccion <= 0.0f; }

    QGraphicsPixmapItem* get_item() const { return item; }

private:

    QGraphicsPixmapItem* item;
    std::vector<QPixmap> frames;

    float escala;

    sprite_ball estado_anterior;

    static constexpr int COLS   = 4;
    static constexpr int BALL_FY = 97;
    static constexpr int BALL_FH = 174;
    static constexpr int BALL_FRAME_X[4] = {31,  184, 338, 502};
    static constexpr int BALL_FRAME_W[4] = {115, 120, 133, 153};

    float timer_destruccion = 0.0f;
    bool destruida = false;

    void precargar_frames();
    void aplicar_frame(sprite_ball estado);

};

#endif // BALLSPRITES_H
