#ifndef PLAYER_H
#define PLAYER_H

// STUB TEMPORAL — reemplazar con Player real de tu compañero
class player {
public:
    float x, y;
    float vida;
    float vida_max;
    bool  inmovilizado;
    float timer_inmovilizacion;
    bool  flash_verde_activo;
    float velocidad;
    float velocidad_base;
    float boost_timer;

    player(float x, float y)
        : x(x), y(y),
        vida(150.0f),
        vida_max(150.0f),
        inmovilizado(false),
        timer_inmovilizacion(0.0f),
        flash_verde_activo(false),
        velocidad(200.0f),
        velocidad_base(200.0f),
        boost_timer(0.0f)
    {}

    void modificar_vida(float cantidad) {
        vida += cantidad;
        if (vida > vida_max) vida = vida_max;
        if (vida < 0.0f)     vida = 0.0f;
    }

    void inmovilizar(float duracion) {
        inmovilizado          = true;
        timer_inmovilizacion  = duracion;
    }

    void aplicar_boost_velocidad(float multiplicador,
                                 float duracion) {
        velocidad   = velocidad_base * multiplicador;
        boost_timer = duracion;
    }

    void activar_flash_verde() {
        flash_verde_activo = true;
    }
};

#endif // PLAYER_H
