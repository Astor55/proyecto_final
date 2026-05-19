#include "trap.h"
#include <random>

using namespace std;

Trap::Trap(const zona& zona)
    : Obstacle(0.0f, 0.0f, ANCHO, ALTO_TRAP, ObstacleType::Trap),
    zona_asignada(zona),
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

    static mt19937 motor(random_device{}());

    uniform_real_distribution<float> dist_x(0.0f, rango_x);
    uniform_real_distribution<float> dist_y(0.0f, rango_y);

    x = zona_asignada.x_min + dist_x(motor);
    y = zona_asignada.y_min + dist_y(motor);

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

    if(!activada) return;

    timer_reactivacion -= dt;

    if(timer_reactivacion <= 0.0f)
    {


        reposicionar();

        activada = false;
        activo = true;
        timer_reactivacion = 0.0f;

        iniciar_parpadeo();

    }

}





//----------------------------
//    SECRET_CODE: 45-GUY
//----------------------------
// unlock appareance SECRETA
//      (anibal design)






