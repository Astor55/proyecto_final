#include "powerupsprites.h"
#include "config.h"

PowerUpSprites::PowerUpSprites(QGraphicsScene* escena, float escala)
    : escena(escena),
    item(nullptr),
    escala(escala),
    parpadeo_timer(0.0f),
    visible(true)

{

    // Cargar los dos pixmaps al construir (una sola vez)
    QPixmap raw_curacion(config::Assets::HEALING_DOOM);
    QPixmap raw_velocidad(config::Assets::SPEED_DOOM);

    int sz = static_cast<int>(64 * escala);

    px_curacion  = raw_curacion.scaled(sz, sz,
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);

    px_velocidad = raw_velocidad.scaled(sz, sz,
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);

    // Crear el item en escena (vacío al inicio)
    item = escena->addPixmap(QPixmap());
    item->setZValue(2);
    item->setVisible(false);

}



PowerUpSprites::~PowerUpSprites()
{

    if(item && item->scene())
        item->scene()->removeItem(item);

    delete item;

}


void PowerUpSprites::actualizar(const PowerUp& pu, float dt)
{
    if(!item) return;

    // Si el power-up ya fue recogido o inactivo -> ocultar
    if(!pu.esta_activo() || pu.fue_recogido())
    {
        item->setVisible(false);
        return;
    }

    // Seleccionar el pixmap correcto según tipo
    const QPixmap& px = (pu.get_tipo() == PowerUpTipo::CURACION)
                            ? px_curacion
                            : px_velocidad;

    item->setPixmap(px);

    // Posicionar centrado en la posición del obstáculo
    float offset = (64.0f * escala) / 2.0f;
    item->setPos(pu.get_x() - offset, pu.get_y() - offset);

    // Parpadeo mientras es_colisionable() == false (primer segundo)
    if(!pu.es_colisionable())
    {
        parpadeo_timer -= dt;
        if(parpadeo_timer <= 0.0f)
        {
            visible = !visible;
            parpadeo_timer = INTERVALO_PARPADEO;
        }
        item->setVisible(visible);
    }
    else
    {
        // Ya colisionable: siempre visible, leve pulso de opacidad
        item->setVisible(true);
        item->setOpacity(1.0f);
    }
}




























