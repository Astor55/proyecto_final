#ifndef TRAP_H
#define TRAP_H

#include "obstacle.h"
#include "zona.h"
#include <functional>

using ValidadorZona = std::function<bool(float, float)>;

class player;

class Trap : public Obstacle
{

public:

    Trap(const zona& otro, ValidadorZona validador);
    ~Trap() override = default;

    // evitar copias
    Trap(const Trap&) = delete;
    Trap& operator=(const Trap& otro) = delete;

    //heredados de obstacle
    void aplicar(player* p) override;
    void actualizar(float dt) override;

private:

    zona zona_asignada; //donde puede reaparecer

    ValidadorZona validador;

    bool activada; // true = si fue pisada

    float timer_reactivacion; // cuenta regresiva

    void reposicionar(); // posicion aleatoria para reaparecer

    static constexpr float ANCHO           = 40.0f;
    static constexpr float ALTO_TRAP       = 20.0f;
    static constexpr float DURACION_TRAMPA = 2.0f;
    static constexpr float COOLDOWN        = 6.0f;

};

#endif // TRAP_H
