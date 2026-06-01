#ifndef POWERUPSPRITES_H
#define POWERUPSPRITES_H

#include "powerup.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

class PowerUpSprites
{

public:

    //constructor y destructor
    PowerUpSprites(QGraphicsScene* escena, float escala = 1.0f);
    ~PowerUpSprites();

    // prohibir copias
    PowerUpSprites(const PowerUpSprites&) = delete;
    PowerUpSprites& operator=(const PowerUpSprites&) = delete;

    // actualizar visibilidad y posición según el estado del PowerUp
    void actualizar(const PowerUp& pu, float dt);

    // acceso al item para removeItem al limpiar
    QGraphicsPixmapItem* get_item() const { return item; }

private:

    QGraphicsScene*      escena;
    QGraphicsPixmapItem* item;

    // pixmaps precargados para cada tipo
    QPixmap px_curacion;
    QPixmap px_velocidad;

    float escala;

    // parpadeo visual
    float parpadeo_timer;
    bool  visible;
    static constexpr float INTERVALO_PARPADEO = 0.15f;

};

#endif // POWERUPSPRITES_H
