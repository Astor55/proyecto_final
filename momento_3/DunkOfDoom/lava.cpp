#include "lava.h"

Lava::Lava(float x, float ancho_lateral,
           float alto_pantalla, float timer_inicial)
    : Obstacle(x, 0.0f,
               ancho_lateral, 0.0f,
               ObstacleType::Lava),
    estado(LavaEstado::ESPERANDO),
    alto_pantalla(alto_pantalla),
    altura_actual(0.0f),
    generar_powerup(false)

{
    timer        = timer_inicial; // desfase entre izquierda y derecha
    activo       = false;
    colisionable = false;
}



// actualizar estados
void Lava::actualizar(float dt)
{

    switch (estado)
    {

    case LavaEstado::ESPERANDO: // primer caso. La lava está esperando para caer
        timer -= dt; //cuenta regresiva

        if(timer <= 0.0f)
        {

            estado = LavaEstado::CAYENDO; //empieza a caer
            altura_actual = 0.0f;
            activo = true;
            timer = 0.0f;

        }
        break;

    case LavaEstado::CAYENDO: // segundo caso. La lava está cayendo

        altura_actual += VELOCIDAD_CAIDA * dt;
        alto = altura_actual;

        if(altura_actual >= alto_pantalla)
        {

            altura_actual = alto_pantalla;
            alto = alto_pantalla;
            colisionable = true;
            estado = LavaEstado::ACTIVA;
            timer = DURACION_ACTIVA;

        }
        break;

    case LavaEstado::ACTIVA: // tercer caso. La lava ya está activa
        timer -= dt;

        if(timer <= 0.0f)
        {

            colisionable = false;
            generar_powerup = true;
            estado = LavaEstado::RETIRANDO;
            timer = 0.0f;

        }
        break;

    case LavaEstado::RETIRANDO:


        altura_actual -= VELOCIDAD_CAIDA * dt;
        alto = altura_actual;

        if(altura_actual <= 0.0f)
        {

            altura_actual = 0.0f;
            alto = 0.0f;
            activo = false;
            estado = LavaEstado::ESPERANDO;
            timer = INTERVALO;

        }
        break;

    }
}


