#ifndef PLAYER_SPRITES_H
#define PLAYER_SPRITES_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <vector>

enum class EstadoAnim
{

    idleFrente,     // fila(0)
    walkArriba,     // fila(1)
    walkAbajo,      // fila(2)
    walkDerecha,    // fila(3)
    walkIzquierda,  // fila(4)
    Trampa          // fila(5)

};



class PlayerSprites
{

public:

    //constructor y destructor
    PlayerSprites(QGraphicsScene* escena, float escala = 0.4f);
    ~PlayerSprites() = default;

    //prohibir copias
    PlayerSprites(const PlayerSprites& otro) = delete;
    PlayerSprites& operator=(const PlayerSprites& otro) = delete;

    //cada frame se llama desde el game loop
    void actualizar(float dt, float dx, float dy, bool atrapado);

    //sincroniza la posicion visual del sprite con la logica del jugador
    void set_pos(float x, float y);

    void set_visible(bool v);

    QGraphicsPixmapItem* get_item() const { return item; }


private:

    // Spritesheets
    std::vector<QPixmap> sheet_principal;   // doom_comportamiento_lvl2.png
    std::vector<QPixmap> sheet_trampa;      // trap_player.png

    // Dimensiones de cada frame
    static constexpr int FILAS_PRINCIPAL = 5;
    static constexpr int COLS_PRINCIPAL  = 11;

    // frame width
    static constexpr int FILA_COL_X[11] = {0, 154, 308, 463, 617, 771, 926, 1080, 1234, 1389, 1543};
    static constexpr int FW_PRINCIPAL    = 154;

    // frame height
    static constexpr int FILA_Y[5]  = {14,  197, 392, 576, 754};
    static constexpr int FILA_H[5]  = {169, 170, 161, 167, 171};

    static constexpr int COLS_TRAMPA     = 8;
    static constexpr int FW_TRAMPA       = 81;
    static constexpr int FH_TRAMPA       = 385;

    //escala player en trampa
    static constexpr float ESCALA_TRAMPA = 2.0f;

    // Estado
    QGraphicsPixmapItem* item;
    EstadoAnim estado_actual;
    float escala;

    int   frame_actual;
    float timer_frame;

    // frames por segundo de la animación
    float fps;

    // Métodos privados
    void elegir_estado(float dx, float dy, bool atrapado);
    void avanzar_frame(float dt);
    //void aplicar_frame();
    void precargar_frames();
    void aplicar_frames();

    int fila_de(EstadoAnim e) const;

};

#endif // PLAYER_SPRITES_H
