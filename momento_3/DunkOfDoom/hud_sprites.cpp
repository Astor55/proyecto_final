#include "hud_sprites.h"
#include "config.h"
#include <QFile>

HudSprites::HudSprites(QGraphicsScene* escena,
                       float vida_max_jugador,
                       float vida_max_boss)
{

    cargar_sprites();

    // ── Barra jugador ──
    marco_jugador = new QGraphicsPixmapItem();
    marco_jugador->setZValue(20);
    marco_jugador->setPos(JUG_X, BARRA_Y_POS);
    escena->addItem(marco_jugador);

    relleno_jugador = escena->addRect(
        JUG_X + RELLENO_X_OFFSET,
        BARRA_Y_POS + RELLENO_Y_OFFSET,
        RELLENO_ANCHO_MAX, RELLENO_ALTO,
        QPen(Qt::NoPen),
        QBrush(Qt::green));
    relleno_jugador->setZValue(19);

    // ── Barra boss ──
    marco_boss = new QGraphicsPixmapItem();
    marco_boss->setZValue(20);
    marco_boss->setPos(BOSS_X, BARRA_Y_POS);
    escena->addItem(marco_boss);

    relleno_boss = escena->addRect(
        BOSS_X + BOSS_RELLENO_X_OFFSET,
        BARRA_Y_POS + BOSS_RELLENO_Y_OFFSET,
        BOSS_RELLENO_ANCHO_MAX, BOSS_RELLENO_ALTO,
        QPen(Qt::NoPen), QBrush(Qt::red));
    relleno_boss->setZValue(19);

    // ── Timer ──
    // Cargar fuente custom
    int id = QFontDatabase::addApplicationFont(config::Assets::FUENTE_DOOM);
    QFont fuente;
    if(id != -1 && !QFontDatabase::applicationFontFamilies(id).isEmpty()) {
        QString familia = QFontDatabase::applicationFontFamilies(id).at(0);
        fuente = QFont(familia, 36, QFont::Bold);
    } else {
        fuente = QFont("Arial", 36, QFont::Bold);
    }

    texto_timer = escena->addText("90", fuente);
    texto_timer->setDefaultTextColor(Qt::white);
    texto_timer->setZValue(20);
    texto_timer->setPos(TIMER_X, TIMER_Y);

    // Mostrar primer frame — protegido contra sprites vacíos
    if(!marcos_jugador[0].isNull())
        marco_jugador->setPixmap(marcos_jugador[0].scaled(
            BARRA_ANCHO, BARRA_ALTO,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation));

    if(!marcos_boss[0].isNull())
        marco_boss->setPixmap(marcos_boss[0].scaled(
            BARRA_ANCHO, BARRA_ALTO,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation));
}


// Cargar sprites
void HudSprites::cargar_sprites() {
    QPixmap sheet_jug(config::Assets::BARRA_PLAYER);
    QPixmap sheet_boss(config::Assets::BARRA_BOSS);

    for(unsigned short i = 0; i < 3; i++)
    {

        marcos_jugador[i] = sheet_jug.copy(0, FRAME_Y[i], 1408, FRAME_ALTO);

        marcos_boss[i]    = sheet_boss.copy(0, BOSS_FRAME_Y[i], 1536, BOSS_FRAME_ALTOS[i]);

    }
}


// Actualizar HUD completo
void HudSprites::actualizar(float vida_jugador, float vida_max_jugador,
                            float vida_boss,    float vida_max_boss,
                            float tiempo_restante) {
    // Barras
    actualizar_barra(marco_jugador, relleno_jugador,
                     vida_jugador, vida_max_jugador,
                     marcos_jugador, RELLENO_ANCHO_MAX);

    actualizar_barra(marco_boss, relleno_boss,
                     vida_boss, vida_max_boss,
                     marcos_boss, BOSS_RELLENO_ANCHO_MAX,
                     BOSS_RELLENO_Y_OFFSET);

    // Timer
    int segundos = static_cast<int>(tiempo_restante);
    texto_timer->setPlainText(QString::number(segundos));

    // Timer rojo cuando quedan menos de 10s
    if(segundos <= 10)
        texto_timer->setDefaultTextColor(Qt::red);
    else
        texto_timer->setDefaultTextColor(Qt::white);
}


// Actualizar barra individual
void HudSprites::actualizar_barra(QGraphicsPixmapItem* marco,
                                  QGraphicsRectItem*   relleno,
                                  float vida, float vida_max,
                                  const std::array<QPixmap, 3>& marcos,
                                  unsigned short ancho_max,
                                  unsigned short y_offset) {
    float porcentaje = vida / vida_max;
    if(porcentaje < 0.0f) porcentaje = 0.0f;
    if(porcentaje > 1.0f) porcentaje = 1.0f;

    // ── Elegir marco según estado ──
    unsigned short idx = 0;
    if(porcentaje <= 0.30f)      idx = 2; // crítico
    else if(porcentaje <= 0.50f) idx = 1; // sangre
    else                          idx = 0; // normal

    marco->setPixmap(marcos[idx].scaled(
        BARRA_ANCHO, BARRA_ALTO,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation));

    // ── Actualizar relleno ──
    QRectF rect = relleno->rect();

    rect.setWidth(ancho_max * porcentaje);
    relleno->setRect(rect);

    // ── Color del relleno ──
    QColor color;
    if(porcentaje > 0.50f)
        color = Qt::green;
    else if(porcentaje > 0.30f)
        color = QColor(200, 100, 0); // naranja
    else
        color = Qt::red;

    relleno->setBrush(QBrush(color));
}
