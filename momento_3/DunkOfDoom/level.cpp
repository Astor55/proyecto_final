#include "level.h"

//constructor
Level::Level()
    : tiempo(0.0f),
    terminado(false),
    pausado(false),
    jugador(nullptr),
    escena(nullptr),
    musica(new QMediaPlayer()),
    audio_output(new QAudioOutput())
{

    musica->setAudioOutput(audio_output);
    audio_output->setVolume(0.7f);

}


//destructor
Level::~Level()
{

    detener_musica();
    delete musica;
    delete audio_output;

    //jugador y escena no se eliminan
    jugador = nullptr;
    escena = nullptr;

}


//iniciar la musica
void Level::iniciar_musica(const QString& ruta)
{

    if(!musica) return;

    musica->setSource(QUrl::fromLocalFile(ruta));
    musica->setLoops(QMediaPlayer::Infinite);
    musica->play();

}


//detener la musica
void Level::detener_musica()
{

    if(!musica) return;

    musica->stop();

}
