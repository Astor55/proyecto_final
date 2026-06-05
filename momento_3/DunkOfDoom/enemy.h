#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Ball;

enum Decision : unsigned char
{
    IDLE,
    ALEJARSE,
    LANZAR,
    ARREBATAR,
    RECOGER,
    AVANZAR_CON_BALON
};

class Enemy : public character
{
private:
    float distancia_al_jugador;
    bool balon_en_mano;
    bool balon_con_jugador;
    Decision decision;

    const float rango_maximo = 90.0f;
    const float rango_minimo = 30.0f;

    bool acaba_de_lanzar = false;
    float timer_acaba_lanzar = 0.0f;
    float cooldown_robo = 0.0f;
    float distancia_a_canasta = 0.0f;

    static constexpr float DELAY_ROBO = 1.5f; // segundos entre robos

public:
    Enemy(float _x, float _y, float _velocidad, float _vida,
          float _fuerza, float _rango_ataque, bool _en_suelo)
        : character(_x, _y, _velocidad, _vida, _fuerza,
                    _rango_ataque, _en_suelo),
        distancia_al_jugador(0),
        balon_en_mano(false),
        balon_con_jugador(false),
        decision(IDLE)
    {}

    void moverse(float dx, float dy) override;
    void atacar(character& objetivo, Ball& balon) override {}

    // level_1
    void quitar(character& objetivo, Ball& balon);
    void lanzar_balon(Ball& balon, float canasta_x, float canasta_y);
    void percepcion(character& jugador, Ball& balon, float dt,
                    float canasta_x, float canasta_y);
    void razonamiento();
    void accion(character& jugador, Ball& balon,
                float canasta_x, float canasta_y, float timer_inv);
    void aprendizaje(unsigned short puntos_player,
                     unsigned short puntos_enemy);

    // level_2
    void percepcion(character& jugador, Ball& balon);
    void accion(character& jugador, Ball& balon,
                float canasta_x, float canasta_y);
    void aprendizaje();

    ~Enemy() override = default;
};

#endif // ENEMY_H
