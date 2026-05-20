#include "powerup.h"
#include <QRandomGenerator>

using namespace std;

// ─────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────
PowerUp::PowerUp(float x, float y,
                 float vida_jugador,
                 float vida_max_jugador)
    : Obstacle(x, y, ANCHO_PU, ALTO_PU, ObstacleType::PowerUp),
    curacion_cantidad(CURACION_DEFAULT),
    tipo_PU(elegir_tipo(vida_jugador, vida_max_jugador)),
    recogido(false)
{
    iniciar_parpadeo(); // aparece parpadeando 1s
}



// Elegir tipo según vida del jugador
PowerUpTipo PowerUp::elegir_tipo(float vida_actual,
                                 float vida_max)
{

    const float porcentaje = vida_actual / vida_max;
    const double prob = QRandomGenerator::global()->generateDouble();

    // Menos del 50% vida -> 70% de prob de que salga curacion
    if(porcentaje < 0.5f)
        return (prob < 0.70f) ? PowerUpTipo::CURACION
                             : PowerUpTipo::VELOCIDAD;

    // mas del 50% vida -> 70% de prob de que salga velocidad
    return (prob < 0.70f) ? PowerUpTipo::VELOCIDAD
                         : PowerUpTipo::CURACION;

}


//aplicar efecto al jugador
void PowerUp::aplicar(player* p)
{

    if(!p || recogido || !colisionable) return;

    switch (tipo_PU)
    {

    case PowerUpTipo::VELOCIDAD:

        p->aplicar_boost_velocidad(MULT_VELOCIDAD,
                                   DURACION_VELOCIDAD);

        break;


    case PowerUpTipo::CURACION:

        p->modificar_vida(curacion_cantidad);
        p->activar_flash_verde();

        break;

    }

    recogido = true;
    activo = false;

}



// actualizar
void PowerUp::actualizar(float dt)
{

    if(recogido || !activo) return;

    actualizar_parpadeo(dt); //heredado de obstacle

}

