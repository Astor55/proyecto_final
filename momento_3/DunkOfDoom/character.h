#ifndef CHARACTER_H
#define CHARACTER_H

// Clase abstracta
class Ball;

class character
{

    protected: //Protected para poder ser heredados por sus clases derivadas

        // Atributos

        float x;
        float y;
        float velocidad;
        float vida;
        float fuerza;
        float rango_ataque; // Se refiere al rango que tiene cada personaje para atacar
        float dx_actual;
        float dy_actual;
        float timer_inmovilizacion = 0.0f;
        float velocidad_base       = 200.0f;
        float boost_timer          = 0.0f;

        Ball* balon;

        bool en_suelo;
        bool inmovilizado = false;
        bool flash_verde_activo = false;

    public:

        character(float _x, float _y, float _vida, float _fuerza,float _rango_ataque, bool _en_suelo);

        // Metodos

        virtual void moverse(float dx, float dy) = 0; // Especificador para el compilador que obliga a las clases derivadas a implementarlo
        virtual void atacar(character& objetivo, Ball& balon) = 0;

        void modificar_vida(float cantidad);
        float get_x()const {return x;}
        float get_y()const {return y;}

        virtual ~character() = default; // Default, le dice al compilador que genere este metodo automaticamente

        // Getters

        float getx() const;
        float gety() const;
        float getvelocidad() const;
        float getvida() const;
        float getfuerza() const;
        bool getensuelo() const;
        float getdx_actual() const;
        float getdy_actual() const;

        // Setters

        void setx(float num);
        void sety(float num);



};

#endif // CHARACTER_H
