#include "lavasprites.h"

LavaSprites::LavaSprites(QGraphicsScene* escena, float x, float ancho)
    : item(nullptr), x(x), ancho(ancho)
{
    item = new QGraphicsRectItem();
    item->setZValue(8);
    item->setBrush(QBrush(QColor(220, 80, 0, 180)));  // naranja semitransparente
    item->setPen(Qt::NoPen);
    item->setVisible(false);
    escena->addItem(item);
}


// actualizar cada frame
void LavaSprites::actualizar(const Lava& lava)
{

    if(!item) return;

    float alto = lava.get_alto();

    if(!lava.esta_activo() && lava.get_estado() == LavaEstado::ESPERANDO)
    {

        item->setVisible(false);
        return;

    }

    item->setRect(x, 0.0f, ancho, alto);
    item->setVisible(true);

    //color segun el estaod
    switch (lava.get_estado())
    {

    case LavaEstado::CAYENDO: item->setBrush(QBrush(QColor(220, 80, 0, 150)));
        break;

    case LavaEstado::ACTIVA: item->setBrush(QBrush(QColor(255, 50, 0, 200)));
        break;

    case LavaEstado::RETIRANDO: item->setBrush(QBrush(QColor(180, 60, 0, 120)));
        break;

    default: item->setVisible(false);
        break;
    }

}















