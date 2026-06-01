#include "boss_sprites.h"
#include "config.h"

BossSprites::BossSprites(QGraphicsScene* escena, float escala)
    : item(nullptr), escala(escala)
{

    item = new QGraphicsPixmapItem();
    item->setZValue(10);

    escena->addItem(item);

    precargar_frames();
    aplicar_frame(0);

}


//precargar los frames del sprite
void BossSprites::precargar_frames()
{

    QPixmap sheet(config::Assets::BOSS_SHEET);

    for(unsigned short i = 0; i < COLS; i++)
    {

        QImage img = sheet.copy(FRAME_X[i], FY, FRAME_W[i], FH)
                         .toImage()
                         .convertToFormat(QImage::Format_ARGB32);

        for(unsigned short y = 0; y < img.height(); y++)
            for(unsigned short x = 0; x < img.width(); x++)
            {

                QRgb pixel = img.pixel(x, y);

                if(qRed(pixel) < 15 && qGreen(pixel) < 15 && qBlue(pixel) < 15)
                    img.setPixel(x, y , qRgba(0, 0, 0, 0));

            }

        frames.push_back(
            QPixmap::fromImage(img).scaled(
                static_cast<unsigned short>(FRAME_W[i] * escala),
                static_cast<unsigned short>(FH * escala),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation));

    }

}


//aplicar frames al sprite
void BossSprites::aplicar_frame(unsigned short indice)
{

    if(!item || frames.empty()) return;
    item->setPixmap(frames[indice]);

    // centrar el punto de transformacion para que la rotacion sea sobre el centro
    float cx = frames[indice].width() / 2.0f;
    float cy = frames[indice].height() / 2.0f;

    item->setTransformOriginPoint(cx, cy);

}


// cambios de estado
void BossSprites::set_Estado(EstadoBoss e)
{

    switch (e)
    {
    case EstadoBoss::idle: aplicar_frame(0); break;

    case EstadoBoss::Cargando: aplicar_frame(1); break;

    case EstadoBoss::Disparando: aplicar_frame(2); break;

    }

}


// cambio se posicion
void BossSprites::set_pos(float x, float y)
{

    if(!item) return;

    float ancho = item->pixmap().width();
    float alto = item->pixmap().height();

    item->setPos(x - ancho / 2.0f, y - alto / 2.0f);

}


void BossSprites::set_flip(bool jugador_a_la_derecha)
{
    if(!item) return;

    QTransform transform;

    if(!jugador_a_la_derecha)
    {
        float w = item->pixmap().width();
        transform.translate(w, 0).scale(-1, 1);
    }

    item->setTransform(transform);

}


void BossSprites::set_visible(bool v)
{

    if(item) item->setVisible(v);

}

