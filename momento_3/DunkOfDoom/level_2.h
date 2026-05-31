#ifndef LEVEL_2_H
#define LEVEL_2_H

#include "level.h"
#include "lava.h"
#include "player_sprites.h"
#include "boss_sprites.h"
#include "trap.h"
#include "powerup.h"
#include "config.h"
#include <array>
#include <vector>
#include <QGraphicsPixmapItem>

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

    //sprite del player
    PlayerSprites* sprite_jugador = nullptr;
    BossSprites * sprite_boss = nullptr;

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
    float curacion_powerup;

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

    //limites o zonas validas
    bool en_zona_valida(float px, float py) const;

    //constantes para la zona
    //centro
    static constexpr float C_IZQ  = 455.0f;
    static constexpr float C_DER  = 809.0f;
    static constexpr float C_ARR  = 295.0f;
    static constexpr float C_ABA  = 568.0f;

    //plataforma izquierda
    static constexpr float PI_IZQ = 244.0f;
    static constexpr float PI_DER = 349.0f;
    static constexpr float PI_ARR = 302.0f;
    static constexpr float PI_ABA = 559.0f;

    //plataforma derecha
    static constexpr float PD_IZQ = 932.0f;
    static constexpr float PD_DER = 1023.0f;
    static constexpr float PD_ARR = 303.0f;
    static constexpr float PD_ABA = 566.0f;

    //puente izquierdo
    static constexpr float PUI_IZQ = 332.0f;
    static constexpr float PUI_DER = 455.0f;
    static constexpr float PUI_ARR = 382.0f;
    static constexpr float PUI_ABA = 436.0f;

    //puente derecho
    static constexpr float PUD_IZQ = 803.0f;
    static constexpr float PUD_DER = 932.0f;
    static constexpr float PUD_ARR = 384.0f;
    static constexpr float PUD_ABA = 436.0f;

    float ultima_x_valida = 640.0f;
    float ultima_y_valida = 430.0f;

};

#endif // LEVEL_2_H
