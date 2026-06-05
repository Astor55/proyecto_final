#ifndef CONFIG_H
#define CONFIG_H


//--------------
//  PANTALLA
//--------------
namespace config
{

static constexpr unsigned short ANCHO_VENTANA  = 1280;
static constexpr unsigned short ALTO_VENTANA = 720;
static constexpr unsigned short FPS_TARGET = 60;
static constexpr float DELTA_TIME = 1.0F / FPS_TARGET;

// -----------
// Dificultad
// -----------
namespace FACIL
{
static constexpr float VIDA_JUGADOR       = 150.0f;
static constexpr float DAÑO_PROYECTIL     = 20.0f;
static constexpr float CURACION_POWERUP   = 20.0f;
static constexpr float PRECISION_IA       = 0.40f;
static constexpr int   HISTORIAL_IA       = 2;
static constexpr float INTERVALO_ATAQUE   = 3.0f;
static constexpr float VEL_PROYECTIL      = 3.0f;
static constexpr float TELEGRAPH          = 0.5f;
static constexpr float DAÑO_LAVA          = 10.0f;
}

<<<<<<< HEAD
        static constexpr float VIDA_JUGADOR       = 150.0f;
        static constexpr float DAÑO_PROYECTIL     = 20.0f;
        static constexpr float CURACION_POWERUP   = 20.0f;
        static constexpr float PRECISION_IA       = 0.40f;
        static constexpr int   HISTORIAL_IA       = 2;
        static constexpr float INTERVALO_ATAQUE   = 3.0f;
        static constexpr float VEL_PROYECTIL      = 300.0f;
        static constexpr float TELEGRAPH          = 0.5f;
        static constexpr float DAÑO_LAVA          = 10.0f;
=======
namespace DIFICIL
{
static constexpr float VIDA_JUGADOR       = 100.0f;
static constexpr float DAÑO_PROYECTIL     = 25.0f;
static constexpr float CURACION_POWERUP   = 18.75f;
static constexpr float PRECISION_IA       = 0.80f;
static constexpr int   HISTORIAL_IA       = 8;
static constexpr float INTERVALO_ATAQUE   = 1.5f;
static constexpr float VEL_PROYECTIL      = 6.0f;
static constexpr float TELEGRAPH          = 0.1f;
static constexpr float DAÑO_LAVA          = 10.0f;
}
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec


// --------
// Nivel 1
// --------
namespace NIVEL1
{

<<<<<<< HEAD
        static constexpr float VIDA_JUGADOR       = 100.0f;
        static constexpr float DAÑO_PROYECTIL     = 25.0f;
        static constexpr float CURACION_POWERUP   = 18.75f;
        static constexpr float PRECISION_IA       = 0.80f;
        static constexpr int   HISTORIAL_IA       = 8;
        static constexpr float INTERVALO_ATAQUE   = 1.5f;
        static constexpr float VEL_PROYECTIL      = 400.0f;
        static constexpr float TELEGRAPH          = 0.1f;
        static constexpr float DAÑO_LAVA          = 15.0f;
=======
static constexpr float DURACION           = 90.0f;
static constexpr int   PUNTOS_POR_CANASTA = 2;
static constexpr float ALTO_SUELO         = 650.0f;
static constexpr float ALTO_CESTA         = 282.0f;
static constexpr float RADIO_CESTA        = 40.0f;
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

}

// --------
// Nivel 2
// --------
namespace NIVEL2
{

<<<<<<< HEAD
        static constexpr float DURACION           = 90.0f;
        static constexpr int   PUNTOS_POR_CANASTA = 2;
        static constexpr float ALTO_SUELO         = 650.0f;
        static constexpr float ALTO_CESTA         = 282.0f;
        static constexpr float RADIO_CESTA        = 40.0f;
=======
static constexpr float DURACION           = 90.0f;
static constexpr float ANCHO_LAVA         = 180.0f;
static constexpr float X_LAVA_IZQ         = 0.0f;
static constexpr float X_LAVA_DER         = 1280.0f - 180.0f;
static constexpr float DESFASE_LAVA       = 7.5f;
static constexpr int   NUM_TRAMPAS        = 4;
static constexpr int   MAX_PROYECTILES    = 6;
static constexpr float DAÑO_LAVA_SEG      = 10.0f;
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

}

// -------
// Física
// -------
namespace FISICAS
{

<<<<<<< HEAD
        static constexpr float DURACION           = 90.0f;
        static constexpr float ANCHO_LAVA         = 150.0f;
        static constexpr float X_LAVA_IZQ         = 200.0f;
        static constexpr float X_LAVA_DER         = 916.0f;
        static constexpr float DESFASE_LAVA       = 7.5f;
        static constexpr int   NUM_TRAMPAS        = 4;
        static constexpr int   MAX_PROYECTILES    = 6;
        static constexpr float DAÑO_LAVA_SEG      = 10.0f;
=======
static constexpr float GRAVEDAD           = 980.0f;
static constexpr float COEF_REBOTE        = 0.35f;
static constexpr float FRICCION           = 0.98f;
static constexpr float VEL_MINIMA         = 20.0f;
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

}

// ----------------
// Rutas de assets
// ----------------
namespace Assets
{

<<<<<<< HEAD
        static constexpr float GRAVEDAD           = 980.0f;
        static constexpr float COEF_REBOTE        = 0.65f;
        static constexpr float COEF_REBOTE2       = 0.35;
        static constexpr float FRICCION           = 0.98f;
        static constexpr float VEL_MINIMA         = 20.0f;
=======
static constexpr const char* FONDO_MENU      = "assets/sprites/ui/fondo_logo.png";
static constexpr const char* LOGO            = "assets/sprites/ui/logo.png";
static constexpr const char* BALL_SHEET      = "assets/sprites/ball/ball_doom.png";
static constexpr const char* MUSICA_MENU     = "assets/sounds/music/menu.mp3";
static constexpr const char* MUSICA_NIVEL1   = "assets/sounds/music/level1.mp3";
static constexpr const char* MUSICA_NIVEL2   = "assets/sounds/music/level2.mp3";
static constexpr const char* SFX_BOUNCE      = "assets/sounds/sfx/bounce.wav";
static constexpr const char* SFX_SCORE       = "assets/sounds/sfx/score.wav";
static constexpr const char* SFX_HIT         = "assets/sounds/sfx/hit.wav";
static constexpr const char* SFX_POWERUP     = "assets/sounds/sfx/powerup.wav";
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec

static constexpr const char* MUSIC_LEVEL1    = "assets/music_level1.mp3";

<<<<<<< HEAD
    // ----------------
    // Rutas de assets
    // ----------------
    namespace Assets
    {

        //juego
        static constexpr const char* FONDO_MENU      = "assets/sprites/ui/fondo_logo.png";
        static constexpr const char* LOGO            = "assets/sprites/ui/logo.png";
        static constexpr const char* MUSICA_MENU     = "assets/sounds/music/menu.mp3";

        //level 1 y 2
        static constexpr const char* BALL_SHEET      = "assets/sprites/Ball_Doom.png";
        static constexpr const char* SFX_BOUNCE      = "assets/sounds/sfx/bounce.wav";
        static constexpr const char* SFX_HIT         = "assets/sounds/sfx/hit.wav";

        //level_2
        static constexpr const char* FONDO_NIVEL2    = "assets/sprites/level2.png";
        static constexpr const char* MUSICA_NIVEL2   = "assets/sounds/music/level2.mp3";
        static constexpr const char* SFX_POWERUP     = "assets/sounds/sfx/powerup.wav";
        static constexpr const char* PLAYER_SHEET_2  = "assets/player/doom_comportamiento_level2.png";
        static constexpr const char* PLAYER_SHEET_T  = "assets/player/trap_player.png";
        static constexpr const char* BOSS_SHEET      = "assets/enemy/boss_comportamiento_lvl2.png";
        static constexpr const char* BARRA_PLAYER    = "assets/sprites/ui/healthbar_player.png";
        static constexpr const char* BARRA_BOSS      = "assets/sprites/ui/healthbar_boss.png";
        static constexpr const char* FUENTE_DOOM     = "assets/fonts/doom.ttf";
        static constexpr const char* TRAP_DOOM       = "assets/sprites/trap_doom.png";
        static constexpr const char* HEALING_DOOM    = "assets/sprites/powerup_curacion.png";
        static constexpr const char* SPEED_DOOM      = "assets/sprites/powerup_velocidad.png";
        static constexpr const char* VICTORY2        = "assets/sprites/ui/victory2.png";
        static constexpr const char* GAME_OVER2      = "assets/sprites/ui/game_over2";
        static constexpr const char* MUSICA_VICTORY   = "assets/sounds/music/victory.mp3";
        static constexpr const char* MUSICA_GAME_OVER = "assets/sounds/music/game_over.mp3";

        //level_1
        static constexpr const char* SFX_SCORE       = "assets/sounds/sfx/score.wav";
        static constexpr const char* MUSICA_NIVEL1   = "assets/sounds/music/level1.mp3";
        static constexpr const char* PLAYER_SHEET_1  = "assets/player/doom_comportamiento_lvl1.png";

    }
=======
}
>>>>>>> 1c20730c189bd461ab8b636d73684ffd0512b9ec
}
#endif // CONFIG_H
