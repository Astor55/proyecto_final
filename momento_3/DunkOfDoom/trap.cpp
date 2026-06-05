#include "trap.h"
#include "player.h"
#include <QRandomGenerator>
#include "QDebug"

using namespace std;

Trap::Trap(const zona& zona, ValidadorZona validador)
    : Obstacle(0.0f, 0.0f, ANCHO, ALTO_TRAP, ObstacleType::Trap),
    zona_asignada(zona),
    validador(validador),
    activada(false),
    timer_reactivacion(0.0f)
{
    reposicionar();
}



//reposisionamiento luego de actuvar
void Trap::reposicionar()
{

    float rango_x = zona_asignada.x_max - zona_asignada.x_min - ANCHO;
    float rango_y = zona_asignada.y_max - zona_asignada.y_min - ALTO_TRAP;

    int intentos = 0;
    do {

        x = zona_asignada.x_min +
            static_cast<float>(QRandomGenerator::global()->generateDouble() * rango_x);
        y = zona_asignada.y_min +
            static_cast<float>(QRandomGenerator::global()->generateDouble() * rango_y);
        intentos++;

    } while(!validador(x, y) && intentos < 50);

}




//aplicar trampa al jugador cuando colisionan
void Trap::aplicar(player* p)
{

    if(!p || activada || !colisionable) return;

    p->inmovilizar(DURACION_TRAMPA);

    activada = true;
    activo = false;
    colisionable = false;
    timer_reactivacion = COOLDOWN;

}



//actualizar trampa
void Trap::actualizar(float dt)
{

    actualizar_parpadeo(dt);

    if(!activada) return;

    timer_reactivacion -= dt;

    if(timer_reactivacion <= 0.0f)
    {


        reposicionar();

        activada = false;
        activo = true;

        colisionable = true;
        timer_reactivacion = 0.0f;

        iniciar_parpadeo();

    }

}


