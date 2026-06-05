#ifndef BALL_H
#define BALL_H

#include "config.h"

class character;

enum class modoBall : unsigned char
{
    basketball,
    projectile
};

enum class ComportamientoColision : unsigned char
{
<<<<<<< HEAD
    Rebote,
    Roto
=======

    Rebote, //rebota y pierde energia hasta romperse
    Roto    //se rompe al primer impacto con el borde

>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec
};

enum class sprite_ball : unsigned char
{
    Normal,
    Agrietada,
    Daniada,
    Destruida
};

class Ball
{
public:
    float x, y;
    float vx, vy;
    bool en_suelo;
    bool activa;

    modoBall modo;
    ComportamientoColision al_colisionar;
    sprite_ball sprite_actual;

    unsigned short rebotes_restantes;
    float energia;

    character* portador;

    Ball(modoBall modo,
         ComportamientoColision comportamiento = ComportamientoColision::Roto,
         unsigned short rebotes_max = REBOTES_MAX_DEF);

    ~Ball() = default;

    Ball(const Ball& otro) = delete;
    Ball& operator=(const Ball&) = delete;

    void actualizar(float cambio);
    void lanzar(float fuerza, float angulo);
    void recoger(character* c);
    void soltar();

    void verificar_colision_paredes(float ancho, float alto);
    void verificar_colision_charater(character* c);

<<<<<<< HEAD
    bool Activa() const { return activa; }
    bool tiene_portador() const { return portador != nullptr; }
    sprite_ball get_sprite() const { return sprite_actual; }
=======
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
    static constexpr float Gravedad = config::FISICAS::GRAVEDAD;
    static constexpr float Coef_rebote = config::FISICAS::COEF_REBOTE;
    static constexpr float Perdida_energia = 0.65f;
    static constexpr float Vel_minima = config::FISICAS::VEL_MINIMA;
    static constexpr float Radio_colision = 60.0f;
    static constexpr float OFFSET_X = 20.0f;
    static constexpr float OFFSET_Y = 10.0f;
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

    static constexpr float Gravedad     = config::FISICAS::GRAVEDAD;
    static constexpr float Coef_rebote  = config::FISICAS::COEF_REBOTE;
    static constexpr float Vel_minima   = config::FISICAS::VEL_MINIMA;
    static constexpr float OFFSET_X     = 20.0f;
    static constexpr float OFFSET_Y     = 10.0f;
    static constexpr unsigned short REBOTES_MAX_DEF = 3;

    // variables de instancia — valores según modo
    float radio_colision;
    float perdida_energia;
    float danio;

private:
    void aplicar_gravedad(float cambio);
    void mover(float cambio);
    void rebotar_suelo();
    void aplicar_rebote_proyectil();
    void actualizar_sprite();

    float radio;
};

#endif // BALL_H
