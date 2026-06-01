#ifndef TRAPSPRITES_H
#define TRAPSPRITES_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

class Trap;

class TrapSprites
{

public:

    //constructor y destructor
    TrapSprites(QGraphicsScene* escena, float escala = 0.06f);
    ~TrapSprites() = default;

    //prohibir copias
    TrapSprites(const TrapSprites& otro) = delete;
    TrapSprites& operator=(const TrapSprites& otro) = delete;

    void actualizar(float dt, float activada, bool en_parpadeo);
    void set_pos(float x, float y);
    void set_visible(bool v);

    QGraphicsPixmapItem* get_item() const { return item; }

private:

    QGraphicsPixmapItem* item;
    QPixmap              sprite; // un solo frame (pentagrama estático)

    float escala;

    // El asset es 1024x1024 — un solo frame estático
    static constexpr int SRC_W = 1024;
    static constexpr int SRC_H = 1024;

    float parpadeo_timer = 0.0f;
    bool  parpadeo_visible = true;

};

#endif // TRAPSPRITES_H
