#ifndef CHARACTER_H
#define CHARACTER_H

class Ball;
class character
{
protected:
    float x;
    float y;
    float velocidad;
    float vida;
    float vida_max;
    float fuerza;
    float rango_ataque;
    float dx_actual;
    float dy_actual;
    float timer_inmovilizacion = 0.0f;
    float velocidad_base       = 200.0f;
    float boost_timer          = 0.0f;
<<<<<<< HEAD

    Ball* balon = nullptr;

    bool en_suelo;
    bool inmovilizado = false;
    bool flash_verde_activo = false;

public:
    character(float _x, float _y, float _velocidad, float _vida, float _fuerza, float _rango_ataque, bool _en_suelo);

    virtual void moverse(float dx, float dy) = 0;
    virtual void atacar(character& objetivo, Ball& bal) {}

    void modificar_vida(float cantidad);

=======
    bool en_suelo;
    bool inmovilizado = false;
    bool flash_verde_activo = false;

public:
    character(float _x, float _y, float _velocidad, float _vida, float _fuerza, float _rango_ataque, bool _en_suelo);
    virtual void moverse(float dx, float dy) = 0;
    virtual void atacar(character& objetivo, Ball& bal) {}
    void modificar_vida(float cantidad);
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec
    virtual ~character() = default;

    // Getters
    float get_x() const { return x; }
    float get_y() const { return y; }
    float getx() const;
    float gety() const;
    float getvelocidad() const;
    float getvida() const;
    float getfuerza() const;
<<<<<<< HEAD
    bool  getensuelo() const;
    float getdx_actual() const;
    float getdy_actual() const;
    float getrango_ataque() const;
    float get_inmovilizado() const { return inmovilizado; }
=======
    bool getensuelo() const;
    float getdx_actual() const;
    float getdy_actual() const;
    float getrango_ataque() const;
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

    // Setters
    void setx(float num);
    void sety(float num);
    void setdx(float num);
    void setdy(float num);
};
#endif // CHARACTER_H
