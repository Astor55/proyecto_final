#ifndef TRAP_H
#define TRAP_H

#include "obstacle.h"
#include "player.h"
#include "zona.h"

class player;

class Trap : public Obstacle
{

public:

    Trap();

    Trap(const zona& otro);
    ~Trap() override = default;

    Trap(const Trap&) = delete;
    Trap& operator=(const Trap& otro) = delete;

    void aplicar(player* p) override;
    void actualizar(float dt) override;

private:

    zona zona_asignada;

    bool activada;

    float timer_reactivacion;

    void reposicionar();

    static constexpr float ANCHO           = 40.0f;
    static constexpr float ALTO_TRAP       = 20.0f;
    static constexpr float DURACION_TRAMPA = 2.0f;
    static constexpr float COOLDOWN        = 6.0f;

};

#endif // TRAP_H
