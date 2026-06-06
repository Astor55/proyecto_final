#include "juego.h"
#include "config.h"
#include "level_1.h"
#include <QApplication>
#include <QWidget>

Juego::Juego(QGraphicsScene* escena, float volumen)
    : escena(escena), volumen(volumen)
{
    int id = QFontDatabase::addApplicationFont(config::Assets::FUENTE_DOOM);
    QString familia = QFontDatabase::applicationFontFamilies(id).at(0);
    fuente_doom = QFont(familia, 36);

    mostrar_pantalla_principal();

}


Juego::~Juego()
{

    limpiar_menu();
    if(musica_menu) { musica_menu->stop(); delete musica_menu; musica_menu = nullptr; }
    if(audio_menu)  { delete audio_menu;  audio_menu  = nullptr; }

}


// mostrar el menu principal
void Juego::mostrar_pantalla_principal()
{

    limpiar_opciones();
    if(fondo_menu) { escena->removeItem(fondo_menu); delete fondo_menu; fondo_menu = nullptr; }
    if(logo)       { escena->removeItem(logo);       delete logo;       logo       = nullptr; }

    estado_menu = EstadoMenu::INTRO_LOGO;
    timer_intro = 2.0f; // segundos mostrando solo el logo

    // Fondo negro
    QPixmap negro(1280, 720);
    negro.fill(Qt::black);
    fondo_menu = escena->addPixmap(negro);
    fondo_menu->setZValue(0);

    // Logo centrado
    QPixmap px_logo(config::Assets::LOGO);
    logo = escena->addPixmap(
        px_logo.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setPos(440, 80);
    logo->setZValue(1);
    logo->setOpacity(0.0f);

    // Iniciar música del menú
    if(!musica_menu)
    {
        musica_menu = new QMediaPlayer();
        audio_menu  = new QAudioOutput();
        musica_menu->setAudioOutput(audio_menu);
        audio_menu->setVolume(volumen);
    }

    musica_menu->setSource(QUrl::fromLocalFile(config::Assets::MUSICA_MENU));
    musica_menu->setLoops(QMediaPlayer::Infinite);
    musica_menu->play();

}


// mostrar la seleccion de niveles
void Juego::mostrar_seleccion_nivel()
{
    limpiar_opciones();
    estado_menu = EstadoMenu::SELECCION_NIVEL;

    entrar_submenu();

    const char* textos[2] = {"NIVEL 1 - BASKETBALL", "NIVEL 2 - BOSS FIGHT"};
    num_opciones = 2;

    for(unsigned short i = 0; i < num_opciones; i++)
    {
        QGraphicsTextItem* txt = escena->addText(textos[i], fuente_doom);
        txt->setZValue(2);
        txt->setPos(640 - txt->boundingRect().width() / 2, 530 + i * 70);
        opciones.push_back(txt);
    }

    opcion_seleccionada = 0;
    resaltar_opcion();
}


// mostrar dificultad [facil - dificil]
void Juego::mostrar_dificultad()
{
    limpiar_opciones();
    estado_menu = EstadoMenu::DIFICULTAD;

    entrar_submenu();

    const char* textos[2] = {"FACIL", "DIFICIL"};
    num_opciones = 2;

    for(unsigned short i = 0; i < num_opciones; i++)
    {
        QGraphicsTextItem* txt = escena->addText(textos[i], fuente_doom);
        txt->setZValue(2);
        txt->setPos(640 - txt->boundingRect().width() / 2, 300 + i * 80);
        opciones.push_back(txt);
    }

    opcion_seleccionada = 0;
    resaltar_opcion();
}


// mostrar pantalla de controles (especie de tutorial)
void Juego::mostrar_controles()
{
    limpiar_opciones();
    estado_menu = EstadoMenu::CONTROLES;

    entrar_submenu();

    const char* info[5] = {
    "W A S D     -  MOVERSE",
    "E           -  ARREBATAR BALON",
    "ESPACIO     -  LANZAR (mantener para cargar)",
    "",
    ""
    };

    for(int i = 0; i < 5; i++)
    {
        QGraphicsTextItem* txt = escena->addText(info[i], QFont(fuente_doom.family(), 30));
        txt->setDefaultTextColor(Qt::white);
        txt->setZValue(2);
        txt->setPos(300, 200 + i * 70);
        textos_info.push_back(txt);
    }


    QGraphicsTextItem* volver = escena->addText("VOLVER", fuente_doom);
    volver->setZValue(2);
    volver->setPos(640 - volver->boundingRect().width() / 2, 500);
    opciones.push_back(volver);
    num_opciones = 1;

    opcion_seleccionada = 0;
    resaltar_opcion();
}


// navegacion por el menu
void Juego::confirmar()
{

    switch(estado_menu)
    {

    case EstadoMenu::PRINCIPAL:
        if(opcion_seleccionada == 0)      mostrar_seleccion_nivel();
        else if(opcion_seleccionada == 1) mostrar_controles();
        else if(opcion_seleccionada == 2)
        {

            if(QWidget* v_principal = QApplication::activeWindow())
            {
                v_principal->close();   // cerrar
            }

        }
        break;

    case EstadoMenu::SELECCION_NIVEL:
        nivel_elegido = opcion_seleccionada + 1;
        if(nivel_elegido == 2) mostrar_dificultad();
        else                   crear_nivel();
        break;

    case EstadoMenu::DIFICULTAD:
        dificil = (opcion_seleccionada == 1);
        crear_nivel();
        break;

    case EstadoMenu::CONTROLES:
        mostrar_opciones_principal();
        break;

    }
}


// click con mouse
void Juego::click(float x, float y)
{

    for(unsigned short i = 0; i < (int)opciones.size(); i++)
    {

        if(opciones[i] && opciones[i]->sceneBoundingRect().contains(x, y))
        {

            opcion_seleccionada = i;
            confirmar();
            return;

        }
    }
}


void Juego::volver()
{

    mostrar_opciones_principal();

}


void Juego::reiniciar(QGraphicsScene* escena)
{

    this->escena = escena;
    jugador = new player(640.0f, 360.0f, 0.0f, 150.0f, 80.0f, 40.0f, false);

    if(nivel_elegido == 1)
        nivel = new Level_1();

    else
        nivel = new Level_2(dificil);

}


void Juego::detener_musica_menu()
{

    if(musica_menu) musica_menu->stop();

}


//crear o instanciar el nivel seleccionado
void Juego::crear_nivel()
{

    jugador = new player(640.0f, 360.0f, 0.0f, 150.0f, 80.0f, 40.0f, false);

    if(nivel_elegido == 1)
        nivel = new Level_1();

    else
        nivel = new Level_2(dificil);

}


// resaltar opcion
void Juego::resaltar_opcion()
{

    for(unsigned short i = 0; i < (int)opciones.size(); i++)

        if(opciones[i])
            opciones[i]->setDefaultTextColor(i == opcion_seleccionada ? Qt::red : Qt::white);

}


// limpiar opciones del menu
void Juego::limpiar_opciones()
{
    for(auto* t : opciones)
        if(t) { escena->removeItem(t); delete t; }
    opciones.clear();

    for(auto* t : textos_info)
        if(t) { escena->removeItem(t); delete t; }
    textos_info.clear();

    num_opciones = 0;
}


// limpiar el menu
void Juego::limpiar_menu()
{
    if(!escena) return;

    limpiar_opciones();

    if(fondo_menu) { escena->removeItem(fondo_menu); delete fondo_menu; fondo_menu = nullptr; }

    if(logo)       { escena->removeItem(logo);       delete logo;       logo       = nullptr; }

}


// actualizar por frame
void Juego::actualizar_menu()
{

    if(estado_menu == EstadoMenu::INTRO_LOGO)
    {

        if(!logo) return;

        float opacidad = qMin(1.0f, (float)logo->opacity() + 0.02f);
        logo->setOpacity(opacidad);

        timer_intro -= config::DELTA_TIME;

        if(timer_intro <= 0.0f)
        {

            estado_menu = EstadoMenu::INTRO_FONDO;
            timer_intro = 1.5f;

            if(!fondo_menu) return;

            QPixmap px_fondo(config::Assets::FONDO_MENU);

            fondo_menu->setPixmap(
                px_fondo.scaled(1280, 720, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            fondo_menu->setOpacity(0.0f);
        }
    }
    else if(estado_menu == EstadoMenu::INTRO_FONDO)
    {

        if(!fondo_menu) return;

        float op = qMin(1.0f, (float)fondo_menu->opacity() + 0.02f);
        fondo_menu->setOpacity(op);

        timer_intro -= config::DELTA_TIME;

        if(timer_intro <= 0.0f)
        {

            // Mostrar opciones
            estado_menu = EstadoMenu::PRINCIPAL;

            const char* textos[3] = {"JUGAR", "CONTROLES", "SALIR"};

            num_opciones = 3;

            for(unsigned short i = 0; i < num_opciones; i++)
            {

                QGraphicsTextItem* txt = escena->addText(textos[i], fuente_doom);
                txt->setZValue(2);
                txt->setPos(640 - txt->boundingRect().width() / 2, 460 + i * 70);
                opciones.push_back(txt);

            }

            opcion_seleccionada = 0;
            resaltar_opcion();

        }
    }
}


void Juego::mostrar_opciones_principal()
{

    limpiar_opciones();
    estado_menu = EstadoMenu::PRINCIPAL;

    salir_submenu();

    const char* textos[3] = {"JUGAR", "CONTROLES", "SALIR"};
    num_opciones = 3;

    for(unsigned short i = 0; i < num_opciones; i++)
    {

        QGraphicsTextItem* txt = escena->addText(textos[i], fuente_doom);
        txt->setZValue(2);
        txt->setPos(640 - txt->boundingRect().width() / 2, 460 + i * 70);
        opciones.push_back(txt);

    }

    opcion_seleccionada = 0;
    resaltar_opcion();

}


// Al entrar a submenús, bajar opacidad del fondo y logo hacer invisible
void Juego::entrar_submenu()
{

    if(fondo_menu) fondo_menu->setOpacity(0.4f);

    if(logo)       logo->setVisible(false); // ocultar el logo

}


// Al volver al principal, restaurar
void Juego::salir_submenu()
{
    if(fondo_menu) fondo_menu->setOpacity(1.0f);

    if(logo)       logo->setVisible(true); // mostrar de nuevo el logo
}

