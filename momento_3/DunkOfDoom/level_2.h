#ifndef LEVEL_2_H
#define LEVEL_2_H

#include "level.h"
#include "lava.h"
#include "trap.h"
#include "powerup.h"
#include "zona.h"
#include "config.h"
#include <array>
#include <vector>

using namespace std;

class Ball;
class Boss;
class player;

class Level_2 : public Level
{

public:

    //constructor/destructor
    Level_2(bool Dificultad);
    ~Level_2() override;

    //prohibir copias
    Level_2(const Level_2& otro) = delete;
    Level_2& operator=(const Level_2& otro) = delete;

    //Herencia de level
    void inicializacion(player* p, QGraphicsScene* escena) override;
    void actualizar(float dt) override;
    void finalizar() override;

    //El jugador sobrevivió hasta el final?
    bool jugador_sobrevivio() const { return sobrevivio; }

private:

    //personajes
    Boss* jefe;

    //obstaculos fijos(no cambian durante el tiempo de ejecucion)
    array<Lava*, 2> lavas;
    array<Trap*, 4> trampas;

    //obstaculos dinamicos(pueden cambiar durante el tiempo de ejecucion)
    vector<Ball*> proyectiles; //max 6 en pantalla
    vector<PowerUp*> powerups; //max 6 en pantalla

    // estado del nivel
    bool dificil;
    bool sobrevivio;
    float timer_ataque; // time entre ataques del boss
    float timer_nivel; // cuenta regresiva de 90s

    //configuracion de dificultad
    float vida_jugador;
    float daño_proyectil;
    float vel_ataque_boss;
    float curaion_powerup;

    //metodos privados de la clase
    void inicializar_lavas();
    void inicializar_trampas();
    void actualizar_proyectiles(float dt);
    void actualizar_obstaculos(float dt);
    void verificar_colisiones();
    void generar_powerup(unsigned short indice_lava);
    void limpiar_proyectiles_inactivos();
    void aplicar_config_dificultad();

    //consttantes
    static constexpr float ANCHO_PLATAFORMA = config::ANCHO_VENTANA;
    static constexpr float ALTO_PLATAFORMA  = config::ALTO_VENTANA;
    static constexpr float ANCHO_LAVA       = 180.0f;
    static constexpr float X_LAVA_IZQ       = 0.0f;
    static constexpr float X_LAVA_DER       = ANCHO_PLATAFORMA - ANCHO_LAVA;
    static constexpr float DESFASE_LAVA     = 7.5f;
    static constexpr unsigned short MAX_PROYECTILES = 6;

};

#endif // LEVEL_2_H
