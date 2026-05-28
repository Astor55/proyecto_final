#include "level_2.h"
#include "ball.h"
#include "boss.h"
#include "player.h"
#include "config.h"
#include <cmath>

// Constructor
Level_2::Level_2(bool es_dificil)
    : Level(),
    jefe(nullptr),
    lavas({nullptr, nullptr}),
    trampas({nullptr, nullptr, nullptr, nullptr}),
    dificil(es_dificil),
    sobrevivio(false),
    timer_ataque(0.0f),
    timer_nivel(config::NIVEL2::DURACION)
{
    aplicar_config_dificultad();
}


// Destructor
Level_2::~Level_2() { Level_2::finalizar(); }


//Aplicar congifuracciones de dificultad
void Level_2::aplicar_config_dificultad()
{
    //dificultad en dificil
    if(dificil)
    {

        vida_jugador = config::DIFICIL::VIDA_JUGADOR;
        daño_proyectil = config::DIFICIL::DAÑO_PROYECTIL;
        vel_ataque_boss = config::DIFICIL::VEL_PROYECTIL;
        curaion_powerup = config::DIFICIL::CURACION_POWERUP;
        timer_ataque = config::DIFICIL::INTERVALO_ATAQUE;

    }
    //dificultad en facil
    else
    {

        vida_jugador = config::FACIL::VIDA_JUGADOR;
        daño_proyectil = config::FACIL::DAÑO_PROYECTIL;
        vel_ataque_boss = config::FACIL::VEL_PROYECTIL;
        curaion_powerup = config::FACIL::CURACION_POWERUP;
        timer_ataque = config::FACIL::INTERVALO_ATAQUE;

    }

}



//inicializar las lavas
void Level_2::inicializar_lavas()
{

    lavas[0] = new Lava(X_LAVA_IZQ, ANCHO_LAVA,
                        ALTO_PLATAFORMA, DESFASE_LAVA);

    lavas[1] = new Lava(X_LAVA_DER, ANCHO_LAVA,
                        ALTO_PLATAFORMA, 0.0f);

}


//Inicializacion tramapas - zonas automatica
void Level_2::inicializar_trampas()
{

    float x_inicio = X_LAVA_IZQ + ANCHO_LAVA;
    float ancho_util = ANCHO_PLATAFORMA - (ANCHO_LAVA * 2);
    float ancho_zona = ancho_util / 4.0f;

    for(unsigned short i = 0; i < 4; i++)
    {

        zona zona(x_inicio + (ancho_zona * i),       // x_min
                  x_inicio + (ancho_zona * (i + 1)), // x_max

                  400.0f,  //y_min
                  680.0f); //y_max

        trampas[i] = new Trap(zona);

    }
}


//Inicializacion del nivel
void Level_2::inicializacion(player* p, QGraphicsScene* escena)
{

    jugador = p;
    this->escena;

    //configurar vida del jugador según dificultad
    jugador->modificar_vida(vida_jugador - jugador->getvida());

    //crear boss centrado arriba (fijo)
    jefe = new Boss(ANCHO_PLATAFORMA / 2.0F, // x centrado
                    80.0f,  //Y arriba
                    300.0f, //vida
                    100.0f, //fuerza
                    500.0f, //rango ataque
                    false,  //en_suelo
                    vel_ataque_boss);   //velociadd_ataque

    inicializar_lavas();
    inicializar_trampas();

    iniciar_musica(config::Assets::MUSICA_NIVEL2);

}


//actualizar game_loop
void Level_2::actualizar(float dt)
{

    if(pausado || terminado) return;

    //Timer del nivel
    timer_nivel -= dt;
    if(timer_nivel <= 0.0f)
    {

        timer_nivel = 0.0f;
        sobrevivio = true;
        terminado = true;
        return;

    }

    //el boss ataca
    timer_ataque -= dt;

    if(timer_ataque <= 0.0f && proyectiles.size() < MAX_PROYECTILES)
    {
        //predecir posicion del jugador
        jefe->calcular_posible_nueva_posicion(*jugador);

        //crear nuevo proyectil
        Ball* p = new Ball(modoBall::projectile,
                           ComportamientoColision::Rebote, 3);

        p->DANIO = daño_proyectil;
        p->x = jefe->get_x();
        p->y = jefe->get_y();
        jefe->lanzar_proyectil(*p);
        proyectiles.push_back(p);

        //resetear timer según dificultad
        timer_ataque = dificil ? config::DIFICIL::INTERVALO_ATAQUE
                               : config::FACIL::INTERVALO_ATAQUE;

    }
}


//actualizar proyectiles
void Level_2::actualizar_proyectiles(float dt)
{

    for(Ball* p : proyectiles)
    {

        if(!p->Activa()) continue;

        p->actualizar(dt);
        p->verificar_colision_paredes(ANCHO_PLATAFORMA, ALTO_PLATAFORMA);

    }
}
























