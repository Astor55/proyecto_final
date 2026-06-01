#ifndef LEVEL_H
#define LEVEL_H

#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QAudioOutput>

class player;

class Level
{
public:

    //constructor y destructor
    Level();
    virtual ~Level();

    //prohibir copias
    Level(const Level& otro) = delete;
    Level& operator=(const Level& otro) = delete;

    //Interfaz publica
    virtual void inicializacion(player* p, QGraphicsScene* scena) = 0;
    virtual void actualizar(float dt) = 0;
    virtual void finalizar() = 0;

    //Getters
    bool esta_terminado() const { return terminado; }
    bool esta_pausado() const { return pausado; }
    float get_tiempo() const { return tiempo; }

    //control
    void pausar() { pausado = true; }
    void reanudar() { pausado = false; }
    void bajar_volumen(float vol) { if(audio_output) audio_output->setVolume(vol); }
    void subir_volumen(float vol) { if(audio_output) audio_output->setVolume(vol); }
    void set_volumen(float vol) { if(audio_output) audio_output->setVolume(vol); }

protected:

    float tiempo; //tiempo transcurrido en segundos

    //condicion
    bool terminado;
    bool pausado;

    //level no es dueño de jugador
    player* jugador;
    QGraphicsScene* escena;

    //musica - cada nivel lo gestiona
    QMediaPlayer* musica;
    QAudioOutput* audio_output;

    //gestion musica
    void iniciar_musica(const QString& ruta);
    void detener_musica();

};

#endif // LEVEL_H
