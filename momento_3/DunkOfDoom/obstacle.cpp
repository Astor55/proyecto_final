#include "obstacle.h"

Obstacle::Obstacle(float x, float y,
                   float ancho, float alto,
                   ObstacleType tipo)
    : x(x), y(y),
      ancho(ancho),
      alto(alto),
      activo(true),
      colisionable(true),
      tipo(tipo),
      timer(0.0f),
      parpadeo_timer(0.0f),
      parpadeo(false)
{}



//colision AABB
bool Obstacle::colisiona_con(float px, float py,
                             float pradio) const
{

    if(!activo || !colisionable) return false;

    return px + pradio > x &&
           px - pradio < x + ancho &&
           py + pradio > y &&
           py - pradio < pradio + alto;

}




//parpadeo
void Obstacle::iniciar_parpadeo()
{

    parpadeo = true;
    colisionable = false;
    parpadeo_timer = Duracion_parpadeo;

}



void Obstacle::actualizar_parpadeo(float dt)
{

    if(!parpadeo) return;

    parpadeo_timer -= dt;

    if(parpadeo_timer <= 0.0f)
    {

        parpadeo = false;
        colisionable = true;
        parpadeo_timer = 0.0f;

    }
}


