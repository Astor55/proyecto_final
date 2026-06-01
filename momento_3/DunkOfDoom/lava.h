#ifndef LAVA_H
#define LAVA_H

#include "obstacle.h"
#include "config.h"

class player;

enum class LavaEstado: unsigned char
{

    ESPERANDO,
    CAYENDO,
    ACTIVA,
    RETIRANDO

};


class Lava: public Obstacle
{

public:

    //------------------------
    //constructor y destructor
    //------------------------
    Lava(float x, float ancho_lateral,
         float alto_pantalla, float timer_inicial);
    ~Lava() override = default;

    //evitar copias
    Lava(const Lava& otro) = delete;
    Lava& operator=(const Lava& otro) = delete;

    //aplicacion y actualizacion
    void aplicar(player* p) override;
    void actualizar(float dt) override;

    // consultas para level 2(generar el power-up)
    bool debe_generar_powerup() const { return generar_powerup; }
    void confirmar_powerup_generado() { generar_powerup = false; }

    LavaEstado get_estado() const { return estado; }
    float get_alto() const { return altura_actual; }

private:

    LavaEstado estado;

    float alto_pantalla;
    float altura_actual; // crece al caer y decrece al retirarse

    bool generar_powerup;

    static constexpr float ANCHO_LATERAL    = config::NIVEL2::ANCHO_LAVA;
    static constexpr float INTERVALO        = 15.0f;  // cada 15s cae
    static constexpr float DURACION_ACTIVA  = 8.0f;   // dura 8s activa
    static constexpr float VELOCIDAD_CAIDA  = 200.0f; // px/s
    static constexpr float DAÑO_POR_SEGUNDO = config::NIVEL2::DAÑO_LAVA_SEG;  // daño continuo

};

#endif // LAVA_H
