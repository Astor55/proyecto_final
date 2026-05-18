#ifndef BALL_H
#define BALL_H

#include "character.h"

enum class modoBall : unsigned char
{

    basketball, //modo para el nivel 1
    projectile  //modo para el nivel 2

};

enum class ComportamientoColision : unsigned char
{

  Rebote, //rebota y pierde energia hasta romperse
  Roto    //se rompe al primer impacto con el borde

};

enum class sprite_ball : unsigned char
{

    Normal,    // sin dueño
    Agrietada, // 2 rebotes restantes
    Daniada,   // 1 rebote restante
    Destruida  // activa = false

};



class Ball
{

public:

    //fisicas

    float x, y;
    float vx, vy;

    bool en_suelo;
    bool activa;

    // comportamientos

    modoBall modo;
    ComportamientoColision al_colisionar;
    sprite_ball sprite_actual;

    // estado de daño

    unsigned short rebotes_restantes;

    float energia;

    //posesiones

    character* portador;

    //contructor, destructor

    Ball(modoBall modo,
         ComportamientoColision comportamiento = ComportamientoColision::Roto,
         unsigned short rebotes_max = REBOTES_MAX_DEF);

    ~Ball() = default;

    // prohibir copia

    Ball(const Ball& otro) = delete;
    Ball& operator=(const Ball&) = delete;

    // interfaz publica

    void actualizar(float Cambio);
    void lanzar(float fuerza, float angulo);
    void recoger(character* c);
    void soltar();

    // colisiones
    void verificar_colision_paredes(float ancho, float alto);
    void verificar_colision_charater(character* c);

    // getters
    bool Activa() const
    {
        return activa;
    }

    bool tiene_portador() const
    {
        return portador != nullptr;
    }


    sprite_ball get_sprite() const
    {
        return sprite_actual;
    }

    // constantes fisicas igual que en el la tierra
    static constexpr float Gravedad = 980.0f;
    static constexpr float Coef_rebote = 0.65f;
    static constexpr float Perdida_energia = 0.65f;
    static constexpr float Vel_minima = 20.0f;
    static constexpr float Radio_colision = 24.0f;
    static constexpr float OFFSET_X = 20.0f;
    static constexpr float OFFSET_Y = 10.0f;

    static constexpr unsigned short REBOTES_MAX_DEF = 3;

    float DANIO_PROYECTIL = 25.0f;

private:

    //fisicas internas
    void aplicar_gravedad(float cambio);
    void mover(float cambio);
    void rebotar_suelo();
    void aplicar_rebote_proyectil();

    //visual
    void actualizar_sprite();

    float radio;


};

#endif // BALL_H
