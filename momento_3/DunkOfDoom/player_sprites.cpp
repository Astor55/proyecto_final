#include "player_sprites.h"
#include "config.h"

// Constructor
PlayerSprites::PlayerSprites(QGraphicsScene* escena, float escala)
    : item(nullptr),
    estado_actual(EstadoAnim::idleFrente),
    escala(escala),
    frame_actual(0),
    timer_frame(0.0f),
    fps(8.0f)   // 8 fps para las animaciones
{

    // Crear el ítem gráfico y añadirlo a la escena
    item = new QGraphicsPixmapItem();
    item->setZValue(10);   // por encima del fondo y la plataforma
    item->setTransformOriginPoint(FW_PRINCIPAL * escala / 2.0f,
                                  FILA_H[0] * escala / 2.0f);
    escena->addItem(item);

    // mostrar el primer frame de inmediato
    precargar_frames();
    aplicar_frames();

}



void PlayerSprites::actualizar(float dt, float dx, float dy, bool atrapado)
{

    elegir_estado(dx, dy, atrapado);
    avanzar_frame(dt);
    aplicar_frames();

}


// set posicion
void PlayerSprites::set_pos(float x, float y)
{

    if(!item) return;

    float alto_visual = (estado_actual == EstadoAnim::Trampa)
                            ? FH_TRAMPA * escala * ESCALA_TRAMPA
                            : FILA_H[fila_de(estado_actual)] * escala;

    item->setPos(x - FW_PRINCIPAL * escala / 2.0f,
                 y - alto_visual / 2.0f);

}



// set visible
void PlayerSprites::set_visible(bool v)
{

    if(item) item->setVisible(v);

}


//elegir estado
void PlayerSprites::elegir_estado(float dx, float dy, bool atrapado)
{

    EstadoAnim nuevo;

    //atrapado
    if(atrapado)
    {

        nuevo = EstadoAnim::Trampa;

    }

    //quieto o estado en reposo
    else if(dx == 0.0f && dy == 0.0f)
    {

        nuevo = EstadoAnim::idleFrente;

    }

    //bajando
    else if(dy > 0.0f)
    {

        nuevo = EstadoAnim::walkAbajo;

    }

    //subiendo
    else if(dy < 0.0f)
    {

        nuevo = EstadoAnim::walkArriba;

    }

    //hacia la derecha
    else if(dx > 0.0f)
    {

        nuevo = EstadoAnim::walkDerecha;

    }

    //hacia la izquierda
    else
    {

        nuevo = EstadoAnim::walkIzquierda;

    }

    //reiniciar anim al cambiar de estado
    if(nuevo != estado_actual)
    {

        estado_actual = nuevo;
        frame_actual = 0;
        timer_frame = 0.0f;

    }
}


//avanzar frame
void PlayerSprites::avanzar_frame(float dt)
{


    timer_frame += dt;

    float intervalo = 1.0f / fps;

    if(timer_frame >= intervalo)
    {

        timer_frame -= intervalo;

        int total_frames = (estado_actual == EstadoAnim::Trampa) ? COLS_TRAMPA
                                                                 : COLS_PRINCIPAL;

        frame_actual = (frame_actual + 1) % total_frames;

    }

}


int PlayerSprites::fila_de(EstadoAnim e) const
{

    switch (e) {
    case EstadoAnim::idleFrente: return 0;

    case EstadoAnim::walkArriba: return 1;

    case EstadoAnim::walkAbajo: return 2;

    case EstadoAnim::walkDerecha: return 3;

    case EstadoAnim::walkIzquierda: return 4;

    default: return 0;

    }
}


//prueba con funciones para los sprites
void PlayerSprites::precargar_frames()
{

    if(!sheet_principal.empty()) return;

    QPixmap sheet_p(config::Assets::PLAYER_SHEET_2);
    QPixmap sheet_t(config::Assets::PLAYER_SHEET_T);

    // principal: 5 filas x 11 columnas
    for(int fila = 0; fila < FILAS_PRINCIPAL; fila++) {
        for(int col = 0; col < COLS_PRINCIPAL; col++) {
            QImage img = sheet_p.copy(FILA_COL_X[col],
                                      FILA_Y[fila],
                                      FW_PRINCIPAL, FILA_H[fila]).toImage()
                             .convertToFormat(QImage::Format_ARGB32);

            for(int y = 0; y < img.height(); y++)
                for(int x = 0; x < img.width(); x++) {
                    QRgb pixel = img.pixel(x, y);
                    if(qRed(pixel) < 15 && qGreen(pixel) < 15 && qBlue(pixel) < 15)
                        img.setPixel(x, y, qRgba(0, 0, 0, 0));
                }

            QPixmap frame = QPixmap::fromImage(img).scaled(
                static_cast<int>(FW_PRINCIPAL * escala),
                static_cast<int>(FILA_H[fila] * escala),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation);

            sheet_principal.push_back(frame);
        }
    }

    for(int col = 0; col < COLS_TRAMPA; col++) {
        QImage img = sheet_t.copy(col * FW_TRAMPA, 0,
                                  FW_TRAMPA, FH_TRAMPA).toImage()
                         .convertToFormat(QImage::Format_ARGB32);

        for(int y = 0; y < img.height(); y++)
            for(int x = 0; x < img.width(); x++) {
                QRgb pixel = img.pixel(x, y);
                if(qRed(pixel) < 15 && qGreen(pixel) < 15 && qBlue(pixel) < 15)
                    img.setPixel(x, y, qRgba(0, 0, 0, 0));
            }

        QPixmap frame = QPixmap::fromImage(img).scaled(
            static_cast<int>(FW_TRAMPA * escala * ESCALA_TRAMPA),
            static_cast<int>(FH_TRAMPA * escala * ESCALA_TRAMPA),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation);

        sheet_trampa.push_back(frame);
    }

}


void PlayerSprites::aplicar_frames()
{
    if(!item) return;

    if(estado_actual == EstadoAnim::Trampa)
    {

        item->setPixmap(sheet_trampa[frame_actual]);

    }

    else
    {
        item->setPixmap(sheet_principal[fila_de(estado_actual) * COLS_PRINCIPAL + frame_actual]);
    }
}

