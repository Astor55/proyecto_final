#include "trapsprites.h"
#include "config.h"

TrapSprites::TrapSprites(QGraphicsScene* escena, float escala)
    : item(nullptr),
    escala(escala)
{
    // Cargar y escalar el pentagrama
    QPixmap raw(config::Assets::TRAP_DOOM);

    sprite = raw.scaled(
        static_cast<int>(SRC_W * escala),
        static_cast<int>(SRC_H * escala),
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

    item = new QGraphicsPixmapItem(sprite);
    item->setZValue(5); // por encima del fondo, por debajo del player
    escena->addItem(item);
}


// actualizar
void TrapSprites::actualizar(float dt, float activada, bool en_parpadeo)
{

    if(!item) return;

    if(activada)
    {
        item->setVisible(false);
        return;
    }

    // parpadeo — alterna visibilidad rápidamente
    if(en_parpadeo)
    {
        parpadeo_timer += dt;
        if(parpadeo_timer >= 0.1f)  // cambia cada 0.1 segundos
        {
            parpadeo_timer = 0.0f;
            parpadeo_visible = !parpadeo_visible;
        }

        // si está en parpadeo (recién reapareció) usa parpadeo_visible, si no siempre visible
        item->setVisible(parpadeo_visible);
    }

    else
    {

        parpadeo_timer = 0.0f;
        parpadeo_visible = true;
        item->setVisible(true);

    }

}


// cambio de posicion
void TrapSprites::set_pos(float x, float y)
{

    if(!item) return;

    item->setPos(x , y);

}


// se hace visible
void TrapSprites::set_visible(bool v)
{

    if(item) item->setVisible(v);

}


