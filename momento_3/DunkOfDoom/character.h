#ifndef CHARACTER_H
#define CHARACTER_H

// Clase abstracta

class character
{

    protected: //Protected para poder ser heredados por sus clases derivadas

        // Atributos

        float x;
        float y;
        float velocidad;
        float vida;
        float fuerza;

        bool en_suelo;

    public:

        character(float _x, float _y, float _vida, float _fuerza, bool _en_suelo);

        // Metodos

        virtual void moverse() = 0; // Especificador para el compilador que obliga a las clases derivadas a implementarlo
        virtual void atacar() = 0;

        void modificar_vida(float cantidad);

        virtual ~character() = default; // Default, le dice al compilador que genere este metodo automaticamente


};

#endif // CHARACTER_H
