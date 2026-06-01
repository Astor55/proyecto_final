#ifndef LAVASPRITES_H
#define LAVASPRITES_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include "lava.h"

class LavaSprites
{

public:

    //cosntructor y destructor
    LavaSprites(QGraphicsScene* escena, float x, float ancho);
    ~LavaSprites() = default;

    //prohibir copias
    LavaSprites(const LavaSprites& otro) = delete;
    LavaSprites& operator=(const LavaSprites& otro) = delete;

    void actualizar(const Lava& lava);

private:

    QGraphicsRectItem* item;

    float x;
    float ancho;

};

#endif // LAVASPRITES_H
