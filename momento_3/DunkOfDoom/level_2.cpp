
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
    zona zonas[4] = {
        // 2 trampas en la central (mitad izquierda y derecha)
        { C_IZQ  + 20.0f, C_IZQ + (C_DER - C_IZQ) / 2.0f - 10.0f, C_ARR + 15.0f, C_ABA - 15.0f },
        { C_IZQ  + (C_DER - C_IZQ) / 2.0f + 10.0f, C_DER - 20.0f, C_ARR + 15.0f, C_ABA - 15.0f },
        // 1 trampa en plataforma izquierda
        { PI_IZQ + 10.0f, PI_DER - 10.0f, PI_ARR + 15.0f, PI_ABA - 15.0f },
        // 1 trampa en plataforma derecha
        { PD_IZQ + 10.0f, PD_DER - 10.0f, PD_ARR + 15.0f, PD_ABA - 15.0f }
    };

    for(unsigned short i = 0; i < 4; i++)
    {
        trampas[i] = new Trap(zonas[i], [this](float x, float y)
                              {
                                  return en_zona_valida(x, y);
                              });
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

    HUD = new HudSprites(escena, vida_jugador, 300.0f);

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

    sprites_lavas[0] = new LavaSprites(escena, X_LAVA_IZQ, ANCHO_LAVA);
    sprites_lavas[1] = new LavaSprites(escena, X_LAVA_DER, ANCHO_LAVA);

    for(unsigned short i = 0; i < 4; i++)
    {

        sprites_trampas[i] = new TrapSprites(escena, 0.06f);
        sprites_trampas[i]->set_pos(trampas[i]->get_x(), trampas[i]->get_y());

    }

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

    //modificar vida del boss segun el timer
    float vida_por_segundo = VIDA_MAX_BOSS / config::NIVEL2::DURACION;
    jefe->modificar_vida(-(vida_por_segundo * dt));

    if(timer_ataque <= 0.0f && proyectiles.size() < MAX_PROYECTILES)
    {

        sprite_boss->set_Estado(EstadoBoss::Disparando);
        sprite_boss->set_flip(jugador->get_x() > jefe->get_x());

        //predecir posicion del jugador
        jefe->calcular_posicion_jugador(*jugador);

        //crear nuevo proyectil

        Ball* proyectil = new Ball(modoBall::projectile, ComportamientoColision::Rebote, 3);

        proyectil->DANIO = daño_proyectil;
        proyectil->x = jefe->get_x();
        proyectil->y = jefe->get_y();
        jefe->lanzar_proyectil(*proyectil);

        proyectiles.push_back(proyectil);

        BallSprites* BS = new BallSprites(escena, 0.3f);
        sprites_proyectiles.push_back(BS);

        //resetear timer según dificultad
        timer_ataque = dificil ? config::DIFICIL::INTERVALO_ATAQUE
                               : config::FACIL::INTERVALO_ATAQUE;

    }

    else if(timer_ataque <= 1.5f)
        sprite_boss->set_Estado(EstadoBoss::Cargando);

    else
        sprite_boss->set_Estado(EstadoBoss::idle);

    actualizar_proyectiles(dt);
    limpiar_proyectiles_inactivos();

    for(size_t i = 0; i < proyectiles.size(); i++)
        sprites_proyectiles[i]->actualizar(*proyectiles[i], dt);

    for(unsigned short i = 0; i < 2; i++)
        sprites_lavas[i]->actualizar(*lavas[i]);

    for(unsigned short i = 0; i < 4; i++)
    {

        sprites_trampas[i]->actualizar(dt, !trampas[i]->esta_activo(), !trampas[i]->es_colisionable());
        sprites_trampas[i]->set_pos(trampas[i]->get_x(), trampas[i]->get_y());

    }

    actualizar_obstaculos(dt);

    sprite_boss->set_flip(jugador->get_x() > jefe->get_x());

    sprite_jugador->actualizar(dt,
                               jugador->getdx_actual(),
                               jugador->getdy_actual(),
                               jugador->get_inmovilizado()
                               );
    sprite_jugador->set_pos(jugador->get_x(), jugador->get_y());

    HUD->actualizar(jugador->getvida(), vida_jugador,
                    jefe->getvida(),   300.0f,
                    timer_nivel);

    jugador->actualizar(dt);

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

        if(lavas[i]->get_estado() == LavaEstado::CAYENDO)
        {
            float x_lateral = (i == 0)
            ? X_LAVA_IZQ + ANCHO_LAVA / 2.0f
            : X_LAVA_DER + ANCHO_LAVA / 2.0f;

            for(PowerUp* pu : powerups)
                if(pu->esta_activo() && pu->get_x() == x_lateral)
                    pu->forzar_desaparicion();
        }

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
    for(size_t i = 0; i < powerups.size(); i++)
    {

        powerups[i]->actualizar(dt);
        sprites_powerups[i]->actualizar(*powerups[i], dt);

    }
}


//verificar colisiones
void Level_2::verificar_colisiones()
{

    if(!jugador) return;

    const float px = jugador->get_x();
    const float py = jugador->get_y();
    const float pr = 12.0f;

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

    // Posición en el lateral correspondiente
    float x_lateral = (indice_lava == 0)
                          ? X_LAVA_IZQ + ANCHO_LAVA / 2.0f
                          : X_LAVA_DER + ANCHO_LAVA / 2.0f;

    // Limpiar powerups inactivos (lógica + sprites)
    size_t i = 0;
    while(i < powerups.size())
    {
        if(powerups[i]->get_x() == x_lateral)
        {
            delete powerups[i];
            powerups.erase(powerups.begin() + i);

            if(sprites_powerups[i]->get_item() &&
                sprites_powerups[i]->get_item()->scene())
                sprites_powerups[i]->get_item()->scene()
                    ->removeItem(sprites_powerups[i]->get_item());
            delete sprites_powerups[i];
            sprites_powerups.erase(sprites_powerups.begin() + i);
        }
        else ++i;
    }

    PowerUp* pu = new PowerUp(x_lateral, 400.0f,
                              jugador->getvida(),
                              vida_jugador);
    pu->curacion_cantidad = curacion_powerup;
    powerups.push_back(pu);

    PowerUpSprites* ps = new PowerUpSprites(escena, 0.6f);
    sprites_powerups.push_back(ps);

}


//limpiar proyectiles inactivos
void Level_2::limpiar_proyectiles_inactivos()
{

    size_t i = 0;
    while(i < proyectiles.size())
    {

        if(!proyectiles[i]->Activa() && sprites_proyectiles[i]->lista_para_limpiar())
        {

            if(sprites_proyectiles[i]->get_item() && sprites_proyectiles[i]->get_item()->scene())
                sprites_proyectiles[i]->get_item()->scene()->removeItem(sprites_proyectiles[i]->get_item());

            delete proyectiles[i];
            proyectiles.erase(proyectiles.begin() + i);
            delete sprites_proyectiles[i];
            sprites_proyectiles.erase(sprites_proyectiles.begin() + i);
        }
        else ++i;
    }
}


//finalizar nivel y liberacion de la memoria
void Level_2::finalizar()
{

    detener_musica();

    //liberar sprite del jugador
    if(sprite_jugador)
    {
        if(sprite_jugador->get_item() && sprite_jugador->get_item()->scene())
            sprite_jugador->get_item()->scene()->removeItem(sprite_jugador->get_item());
        delete sprite_jugador;
        sprite_jugador = nullptr;
    }

    //liberar sprite del boss
    if(sprite_boss)
    {
        if(sprite_boss->get_item() && sprite_boss->get_item()->scene())
            sprite_boss->get_item()->scene()->removeItem(sprite_boss->get_item());
        delete sprite_boss;
        sprite_boss = nullptr;
    }

    // liberar sprites trampas
    for(unsigned short i = 0; i < 4; i++)
    {
        if(sprites_trampas[i])
        {
            if(sprites_trampas[i]->get_item() && sprites_trampas[i]->get_item()->scene())
                sprites_trampas[i]->get_item()->scene()->removeItem(sprites_trampas[i]->get_item());
            delete sprites_trampas[i];
            sprites_trampas[i] = nullptr;
        }
    }

    //liberar sprites de los proyectiles
    for(BallSprites* BS : sprites_proyectiles) delete BS;
    sprites_proyectiles.clear();

    //liberar al boss
    delete jefe;
    jefe = nullptr;

    //liberar lavas
    for(unsigned short i = 0; i < 2; i++)
    {

        delete lavas[i];
        lavas[i] = nullptr;

    }

    //liberar sprite de lavas
    for(unsigned short i = 0; i < 2; i++)
    {
        delete sprites_lavas[i];
        sprites_lavas[i] = nullptr;
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

    //liberar los powerups
    for(size_t i = 0; i < powerups.size(); i++)
    {
        if(sprites_powerups[i]->get_item() &&
            sprites_powerups[i]->get_item()->scene())
            sprites_powerups[i]->get_item()->scene()
                ->removeItem(sprites_powerups[i]->get_item());
        delete sprites_powerups[i];
        delete powerups[i];
    }
    sprites_powerups.clear();
    powerups.clear();

    // liberar HUD
    delete HUD;
    HUD = nullptr;

    //liberar al player
    jugador = nullptr;

    //liberar la escena
    escena = nullptr;

}








