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

    sprite_jugador = new PlayerSprites(escena, 0.4f);
    sprite_jugador->set_pos(jugador->get_x(), jugador->get_y());

    //configurar vida del jugador según dificultad
    jugador->modificar_vida(vida_jugador - jugador->getvida());

    //crear boss centrado arriba (fijo)
    jefe = new Boss(ANCHO_PLATAFORMA / 2.0F, // x centrado
                    150.0f,  //Y arriba
                    300.0f, //vida
                    100.0f, //fuerza
                    500.0f, //rango ataque
                    false,  //en_suelo
                    vel_ataque_boss);   //velociadd_ataque

    sprite_boss = new BossSprites(escena, 0.4f);
    sprite_boss->set_pos(jefe->get_x(), jefe->get_y());

    inicializar_lavas();
    inicializar_trampas();

    iniciar_musica(config::Assets::MUSICA_NIVEL2);

}


//validar limites o zonas permitidas de movimiento para el jugador
bool Level_2::en_zona_valida(float px, float py) const
{

    //plataforma central
    if(px >= C_IZQ && px <= C_DER &&
        py >= C_ARR && py <= C_ABA)
        return true;

    //plataforma izquierda
    if(px >= PI_IZQ && px <= PI_DER &&
        py >= PI_ARR && py <= PI_ABA)
        return true;

    // Plataforma derecha
    if(px >= PD_IZQ && px <= PD_DER &&
        py >= PD_ARR && py <= PD_ABA)
        return true;

    // Puente izquierdo
    if(px >= PUI_IZQ && px <= PUI_DER &&
        py >= PUI_ARR && py <= PUI_ABA)
        return true;

    // Puente derecho
    if(px >= PUD_IZQ && px <= PUD_DER &&
        py >= PUD_ARR && py <= PUD_ABA)
        return true;

    return false;

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

        sprite_boss->set_Estado(EstadoBoss::Disparando);

        //float dx = jugador->get_x() - jefe->get_x();
        //float dy = jugador->get_y() - jefe->get_y();
        sprite_boss->set_Estado(EstadoBoss::Disparando);
        sprite_boss->set_flip(jugador->get_x() > jefe->get_x());

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

    else if(timer_ataque <= 1.5f)
        sprite_boss->set_Estado(EstadoBoss::Cargando);

    else
        sprite_boss->set_Estado(EstadoBoss::idle);

    sprite_boss->set_flip(jugador->get_x() > jefe->get_x());

    sprite_jugador->actualizar(dt,
                               jugador->getdx_actual(),
                               jugador->getdy_actual(),
                               jugador->get_inmovilizado()
                               );
    sprite_jugador->set_pos(jugador->get_x(), jugador->get_y());

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

    if(!en_zona_valida(jugador->get_x(), jugador->get_y()))
    {

        //revertir a ultima posicion valida
        jugador->setx(ultima_x_valida);
        jugador->sety(ultima_y_valida);

    }

    //guardar ultima pos valida
    if(en_zona_valida(jugador->get_x(), jugador->get_y()))
    {

        ultima_x_valida = jugador->get_x();
        ultima_y_valida = jugador->get_y();

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

    if(sprite_jugador)
    {
        if(sprite_jugador->get_item() && sprite_jugador->get_item()->scene())
            sprite_jugador->get_item()->scene()->removeItem(sprite_jugador->get_item());
        delete sprite_jugador;
        sprite_jugador = nullptr;
    }

    if(sprite_boss)
    {
        if(sprite_boss->get_item() && sprite_boss->get_item()->scene())
            sprite_boss->get_item()->scene()->removeItem(sprite_boss->get_item());
        delete sprite_boss;
        sprite_boss = nullptr;
    }

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








