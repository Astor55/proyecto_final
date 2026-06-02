#include "enemy.h"
#include "ball.h"
#include <cmath>
#include <QDebug>


using namespace std;


void Enemy::moverse(float dx, float dy)
{
    dx_actual = dx;
    dy_actual = dy;
    x += dx * velocidad;
    y += dy * velocidad;

    // limitar dentro de la pantalla
    if(x < 150)  x = 150;
    if(x > 1045) x = 1045;
    if(y < 350) y = 350;
    if(y > 600)  y = 600;

}


void Enemy :: quitar(character& objetivo, Ball& balon){

    float dx = objetivo.getx() - x;

    float dy = objetivo.gety() - y;

    float distancia = sqrt((dx*dx) + (dy*dy));

    if(distancia <= rango_ataque && balon.portador == &objetivo){

        balon.recoger(this);

    }


}

void Enemy::percepcion(character& jugador, Ball& balon, float dt)
{
    if(timer_acaba_lanzar > 0)
    {
        timer_acaba_lanzar -= dt;
        if(timer_acaba_lanzar <= 0)
            acaba_de_lanzar = false;
    }

    float dx = jugador.getx() - x;
    float dy = jugador.gety() - y;
    distancia_al_jugador = sqrt((dx*dx) + (dy*dy));
    balon_en_mano = (balon.portador == this);
    balon_con_jugador = (balon.portador == &jugador);
}


void Enemy::razonamiento()
{
    if(balon_en_mano)
    {
        acaba_de_lanzar = false;
        decision = LANZAR;
    }
    else if(balon_con_jugador)
    {
        acaba_de_lanzar = false;
        decision = ARREBATAR;
    }
    else if(acaba_de_lanzar)
    {
        decision = IDLE; // esperar, no perseguir el balon
    }
    else
    {
        decision = RECOGER;
    }
}

void Enemy::accion(character& jugador, Ball& balon, float canasta_x, float canasta_y, float timer_inv)
{
    if(decision == ARREBATAR)
    {
        float dx = jugador.getx() - x;
        float dy = jugador.gety() - y;
        float distancia = sqrt(dx*dx + dy*dy);
        if(distancia > 0)
        {
            dx /= distancia;
            dy /= distancia;
        }
        moverse(dx, dy);
        quitar(jugador, balon);
    }

    else if(decision == LANZAR)
    {
        lanzar_balon(balon, canasta_x, canasta_y);
    }

    else if(decision == RECOGER)
    {
        float dx = balon.x - x;
        float dy = balon.y - y;
        float distancia = sqrt(dx*dx + dy*dy);
        if(distancia > 0)
        {
            dx /= distancia;
            dy /= distancia;
        }
        moverse(dx, dy);
        if(timer_inv <= 0)
            balon.recoger(this);
    }
}

void Enemy::aprendizaje(unsigned short puntos_player, unsigned short puntos_enemy)
{
    if(puntos_enemy < puntos_player)
    {
        if(rango_ataque < rango_maximo)
            rango_ataque *= 1.1f;
    }
    else if(puntos_enemy > puntos_player)
    {
        if(rango_ataque > rango_minimo)
            rango_ataque *= 0.9f;
    }
}


void Enemy::lanzar_balon(Ball& balon, float canasta_x, float canasta_y)
{
    if(balon.portador == this)
    {
        float dx = canasta_x - x;
        float dy = canasta_y - y;
        float angulo = atan2(dy, dx);
        balon.lanzar(fuerza, angulo);
        acaba_de_lanzar = true;
        timer_acaba_lanzar = 0.5;
    }
}
