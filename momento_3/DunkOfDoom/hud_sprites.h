#ifndef HUD_SPRITES_H
#define HUD_SPRITES_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPixmap>
#include <QFontDatabase>
#include <QFont>
#include <array>

class HudSprites {
public:
    HudSprites(QGraphicsScene* escena,
               float vida_max_jugador,
               float vida_max_boss);
    ~HudSprites() = default;

    HudSprites(const HudSprites&)            = delete;
    HudSprites& operator=(const HudSprites&) = delete;

    void actualizar(float vida_jugador, float vida_max_jugador,
                    float vida_boss,    float vida_max_boss,
                    float tiempo_restante);

private:
    // ── Barra jugador ──
    QGraphicsPixmapItem* marco_jugador;
    QGraphicsRectItem*   relleno_jugador;

    // ── Barra boss ──
    QGraphicsPixmapItem* marco_boss;
    QGraphicsRectItem*   relleno_boss;

    // ── Timer ──
    QGraphicsTextItem*   texto_timer;

    // ── Frames de marcos ──
    std::array<QPixmap, 3> marcos_jugador; // normal, sangre, critico
    std::array<QPixmap, 3> marcos_boss;

    // ── Métodos privados ──
    void cargar_sprites();
    void actualizar_barra(QGraphicsPixmapItem* marco,
                          QGraphicsRectItem*   relleno,
                          float vida, float vida_max,
                          const std::array<QPixmap, 3>& marcos,
                          unsigned short ancho_max,
                          unsigned short y_offset = RELLENO_Y_OFFSET);

    // ── Constantes layout ──
    static constexpr unsigned short BARRA_ANCHO  = 380;
    static constexpr unsigned short BARRA_ALTO   = 70;
    static constexpr unsigned short BARRA_Y_POS  = 10;

    // Posición jugador
    static constexpr unsigned short JUG_X        = 10;

    // Posición boss (derecha)
    static constexpr unsigned short BOSS_X       = 840;

    // Timer centro
    static constexpr unsigned short TIMER_X      = 590;
    static constexpr unsigned short TIMER_Y      = 10;

    // Interior relleno - jugador
    static constexpr unsigned short RELLENO_X_OFFSET = 100;
    static constexpr unsigned short RELLENO_Y_OFFSET = 25;
    static constexpr unsigned short RELLENO_ALTO      = 35;
    static constexpr unsigned short RELLENO_ANCHO_MAX = 240;

    // Interior relleno — boss
    static constexpr unsigned short BOSS_RELLENO_X_OFFSET  = 108;
    static constexpr unsigned short BOSS_RELLENO_Y_OFFSET  = 31;
    static constexpr unsigned short BOSS_RELLENO_ALTO       = 36;
    static constexpr unsigned short BOSS_RELLENO_ANCHO_MAX  = 210;

    // Frames boss
    static constexpr unsigned short BOSS_FRAME_Y[3]  = {65, 342, 633};
    static constexpr unsigned short BOSS_FRAME_ALTOS[3] = {213, 219, 244};

    // Frames jugador
    static constexpr unsigned short FRAME_Y[3]   = {30, 270, 520};
    static constexpr unsigned short FRAME_ALTO   = 220;
};

#endif // HUD_SPRITES_H
