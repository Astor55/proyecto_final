#include "ballsprites.h"

BallSprites::BallSprites(QGraphicsScene* escena, float escala)
    : item(nullptr),
    escala(escala),
    estado_anterior(sprite_ball::Normal)
{

    item = new QGraphicsPixmapItem();
    item->setZValue(15);
    escena->addItem(item);
    precargar_frames();
    aplicar_frame(sprite_ball::Normal);

}


// precargar frames del sprite
void BallSprites::precargar_frames()
{
    QPixmap sheet(config::Assets::BALL_SHEET);

    for(int i = 0; i < COLS; i++)
    {
        QImage img = sheet.copy(BALL_FRAME_X[i], BALL_FY,
                                BALL_FRAME_W[i], BALL_FH)
                         .toImage()
                         .convertToFormat(QImage::Format_ARGB32);

        for(int y = 0; y < img.height(); y++)
            for(int x = 0; x < img.width(); x++)
            {
                QRgb pixel = img.pixel(x, y);
                if(qRed(pixel) < 15 && qGreen(pixel) < 15 && qBlue(pixel) < 15)
                    img.setPixel(x, y, qRgba(0, 0, 0, 0));
            }

        frames.push_back(
            QPixmap::fromImage(img).scaled(
                static_cast<int>(BALL_FRAME_W[i] * escala),
                static_cast<int>(BALL_FH * escala),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation));
    }

    qDebug() << "frames cargados:" << frames.size();
    qDebug() << "frame 0 isNull:" << frames[0].isNull();

}


//aplicar frames del sprite
void BallSprites::aplicar_frame(sprite_ball estado)
{

    if(!item || frames.empty()) return;

    int indice = 0;
    switch(estado)
    {
    case sprite_ball::Normal:    indice = 0; break;
    case sprite_ball::Agrietada: indice = 1; break;
    case sprite_ball::Daniada:   indice = 2; break;
    case sprite_ball::Destruida: indice = 3; break;
    }

    item->setPixmap(frames[indice]);

    float cx = frames[indice].width()  / 2.0f;
    float cy = frames[indice].height() / 2.0f;
    item->setTransformOriginPoint(cx, cy);

}


// atualizar
void BallSprites::actualizar(const Ball& ball, float dt)
{

    if(!item) return;

    qDebug() << "ball activa:" << ball.Activa() << "x:" << ball.x << "y:" << ball.y;

    if(destruida)
    {
        timer_destruccion -= dt;
        if(timer_destruccion <= 0.0f)
            item->setVisible(false);
        return;
    }

    if(!ball.Activa())
    {

        destruida = true;
        timer_destruccion = 0.3;
        aplicar_frame(sprite_ball::Destruida);
        item->setVisible(true);
        return;

    }

    // actualizar frame solo si cambio de estado
    if(ball.get_sprite() != estado_anterior)
    {
        estado_anterior = ball.get_sprite();
        aplicar_frame(estado_anterior);
    }

    //posicion
    float ancho = item->pixmap().width();
    float alto = item->pixmap().height();

    item->setPos(ball.x - ancho / 2.0f, ball.y - alto / 2.0f);
    item->setVisible(true);

}



//hacer visible el sprite
void BallSprites::set_visible(bool v)
{

    if(item) item->setVisible(v);

}























