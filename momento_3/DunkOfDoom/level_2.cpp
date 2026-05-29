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
        curacion_powerup = config::DIFICIL::CURACION_POWERUP;
        timer_ataque = config::DIFICIL::INTERVALO_ATAQUE;

    }
    //dificultad en facil
    else
    {

        vida_jugador = config::FACIL::VIDA_JUGADOR;
        daño_proyectil = config::FACIL::DAÑO_PROYECTIL;
        vel_ataque_boss = config::FACIL::VEL_PROYECTIL;
        curacion_powerup = config::FACIL::CURACION_POWERUP;
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
    this->escena = escena;

    //fondo
    QPixmap fondo(config::Assets::FONDO_NIVEL2);
    fondo = fondo.scaled(config::ANCHO_VENTANA,
                         config::ALTO_VENTANA,
                         Qt::IgnoreAspectRatio,
                         Qt::FastTransformation);

    QGraphicsPixmapItem* bg = escena->addPixmap(fondo);
    bg->setZValue(-1);

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

    inicializar_plataformas();

}


void Level_2::inicializar_plataformas()
{

    plataformas.clear();

    plataformas.push_back(QRectF(
        446.0f,
        301.0f,
        357.0f,
        267.0f
        ));

    plataformas.push_back(QRectF(
        837.0f,
        393.0f,
        46.0f,
        43.0f
        ));

    plataformas.push_back(QRectF(
        917.0f,
        303.0f,
        106.0f,
        263.0f
        ));

    plataformas.push_back(QRectF(
        370.0f,
        393.0f,
        43.0f,
        43.0f
        ));

    plataformas.push_back(QRectF(
        230.0f,
        303.0f,
        103.0f,
        256.0f
        ));

}


//actualizar game_loop
void Level_2::actualizar(float dt)
{

    if(pausado || terminado) return;

    verificar_colisiones();

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


//actualizar obstaculos
void Level_2::actualizar_obstaculos(float dt)
{

    //lavas
    for(unsigned short i = 0; i < 2; i++)
    {

        lavas[i]->actualizar(dt);

        //generar powerup cuando lava se retira
        if(lavas[i]->debe_generar_powerup())
        {

            generar_powerup(i);
            lavas[i]->confirmar_powerup_generado();

        }
    }

    //trampas
    for (unsigned short i = 0; i < 4; i++)
        trampas[i]->actualizar(dt);

    //PowerUps
    for(PowerUp* pu : powerups)
        pu->actualizar(dt);

}


//verificar colisiones
void Level_2::verificar_colisiones()
{

    if(!jugador) return;

    const float px = jugador->get_x();
    const float py = jugador->get_y();
    const float pr = 24.0f;

    QRectF hitboxJugador(
        px - pr,
        py - pr,
        pr * 2.0f,
        pr * 2.0f
        );

    bool enPlataforma = false;

    for(const QRectF& plataforma : plataformas)
    {
        if(plataforma.contains(hitboxJugador))
        {
            enPlataforma = true;
            break;
        }
    }

    if(!enPlataforma)
    {
        jugador->setx(jugador->get_x_anterior());
        jugador->sety(jugador->get_y_anterior());
    }

    //proyectiles vs jugador
    for(Ball* p: proyectiles)
    {

        if(!p->Activa()) continue;
        p->verificar_colision_charater(jugador);

    }

    //trampas vs jugador
    for(unsigned short i = 0; i < 4; i++)
    {

        if(trampas[i]->colisiona_con(px, py, pr))
            trampas[i]->aplicar(jugador);

    }

    float daño_lava = dificil ? config::DIFICIL::DAÑO_LAVA
                              : config::FACIL::DAÑO_LAVA;

    //lava vs jugador (daño continuo)
    for(unsigned short i = 0; i < 2; i++)
    {

        if(lavas[i]->colisiona_con(px, py, pr))
            jugador->modificar_vida(-daño_lava * 0.016f);

    }

    //Power Up vs jugador
    for(PowerUp* pu : powerups)
    {

        if(!pu->esta_activo()) continue;

        if(pu->colisiona_con(px, py, pr))
            pu->aplicar(jugador);

    }

    //verificar muerte del jugador
    if(jugador->getvida() <= 0.0f)
    {

        sobrevivio = false;
        terminado = true;

    }

}


//Generar Power Up cuando la lava se retira
void Level_2::generar_powerup(unsigned short indice_lava)
{

    //eliminar el PowerUp anterior
    for(auto it = powerups.begin(); it != powerups.end();)
    {

        PowerUp* pu = *it;

        if(!pu->esta_activo())
        {

            delete pu;
            it = powerups.erase(it);

        }

        else
        {

            ++it;

        }
    }

    //posicion aleatoria dentro de los laterales
    float x_lateral = (indice_lava == 0)
                          ? X_LAVA_IZQ + ANCHO_LAVA / 2.0f
                          : X_LAVA_DER + ANCHO_LAVA / 2.0f;

    PowerUp* pu = new PowerUp(x_lateral, 400.0f,
                              jugador->getvida(),
                              vida_jugador);

    pu->curacion_cantidad = curacion_powerup;
    powerups.push_back(pu);

}


//limpiar proyectiles inactivos
void Level_2::limpiar_proyectiles_inactivos()
{

    for(auto it = proyectiles.begin(); it != proyectiles.end();)
    {

        Ball* p = *it;

        if(!p->Activa())
        {

            delete p;

            it = proyectiles.erase(it);

        }

        else
        {

            ++it;

        }
    }
}


//finalizar nivel y liberacion de la memoria
void Level_2::finalizar()
{

    detener_musica();

    delete jefe;
    jefe = nullptr;

    //liberar lavas
    for(unsigned short i = 0; i < 2; i++)
    {

        delete lavas[i];
        lavas[i] = nullptr;

    }

    //liberar trampas
    for(unsigned short i = 0; i < 4; i++)
    {

        delete trampas[i];
        trampas[i] = nullptr;

    }

    //liberar proyectiles
    for(Ball* p : proyectiles) delete p;
    proyectiles.clear();

    for(PowerUp* pu : powerups) delete pu;
    powerups.clear();

    jugador = nullptr;
    escena = nullptr;

}








