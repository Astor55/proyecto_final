#ifndef POWERUP_H
#define POWERUP_H

#include "obstacle.h"

class player;

// tipos de PowerUp
enum class PowerUpTipo : unsigned char
{

    VELOCIDAD,
    CURACION

};


class PowerUp: public Obstacle
{

public:

    //------------------------
    //constructor y destructor
    //------------------------
    PowerUp(float x, float y, float vida_jugador,
            float vida_max_jugador);

    ~PowerUp() override = default;

    // evitar copias
    PowerUp(const PowerUp& otro) = delete;
    PowerUp& operator=(const PowerUp& otro) = delete;

    void aplicar(player* p) override;
    void actualizar(float dt) override;

    // level 2 consultará para saber si fué recogido
    bool fue_recogido() const { return recogido; }

    // level 2 llama esto para cuando la lava vuelva a caer
    void forzar_desaparicion() { activo = false; recogido = true; }

    PowerUpTipo get_tipo() const { return tipo_PU; }

    //esto se configura desde level 2 segun la dificultad
    float curacion_cantidad;

private:

    PowerUpTipo tipo_PU;

    bool recogido;

    static PowerUpTipo elegir_tipo(float vida_actual,
                                   float vida_max);

    static constexpr float ANCHO_PU           = 30.0f;
    static constexpr float ALTO_PU            = 30.0f;
    static constexpr float MULT_VELOCIDAD     = 1.5f;
    static constexpr float DURACION_VELOCIDAD = 5.0f;
    static constexpr float CURACION_DEFAULT   = 20.0f;

};

#endif // POWERUP_H
