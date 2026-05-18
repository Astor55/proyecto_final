#ifndef OBSTACLE_H
#define OBSTACLE_H

class Character;

class player;


enum class ObstacleType : unsigned char
{

    Trap,
    Lava,
    PowerUp

};

// -----------------
//  CLASE ABSTRACTA
// -----------------
class Obstacle
{

public:
    //constructor
    Obstacle(float x, float y, float ancho, float alto, ObstacleType tipo);

    //destructor
    virtual ~Obstacle() = default;

    // prohibir copias
    Obstacle(const Obstacle& otro) = delete;
    Obstacle& operator=(const Obstacle& otro) = delete;

    // interfaz publica
    virtual void aplicar(player* p) = 0;
    virtual void actualizar(float dt) = 0;

    // colision AABB simple
    bool colisiona_con(float px, float py, float pradio) const;

    //getters
    float get_x() const { return x; }
    float get_y() const { return y; }
    float get_ancho() const { return ancho; }
    float get_alto() const { return alto; }
    bool  esta_activo() const { return activo; }
    bool  es_colisionable() const { return colisionable; }
    ObstacleType get_tipo() const { return tipo; }

protected:

    float x, y;
    float ancho, alto;
    bool activo;
    bool colisionable;

    ObstacleType tipo;

    //timer general y reutilizable
    float timer;

    //parpadeo
    float parpadeo_timer;
    bool parpadeo;

    static constexpr float Duracion_parpadeo = 1.0f;

    void iniciar_parpadeo();
    void actualizar_parpadeo(float dt);

};

#endif // OBSTACLE_H
