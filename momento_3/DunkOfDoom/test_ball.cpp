#include "ball.h"
#include "character.h"
#include <iostream>

void test_ball() {

    // Prueba 1: Basketball
    Ball* balon = new Ball(modoBall::basketball);
    Character* jugador = new Character(100.0f, 300.0f);

    balon->recoger(jugador);
    std::cout << "Tiene portador: "
              << balon->tiene_portador() << "\n";

    balon->lanzar(500.0f, 0.785f);
    std::cout << "Portador tras lanzar: "
              << balon->tiene_portador() << "\n";

    for(unsigned short i = 0; i < 60; i++) {
        balon->actualizar(0.016f);
        balon->verificar_colision_paredes(1280.0f, 720.0f);
    }
    std::cout << "x: " << balon->x << " y: " << balon->y << "\n";

    // Prueba 2: Proyectil BREAK
    Ball* p1 = new Ball(modoBall::projectile, ComportamientoColision::Roto);
    p1->DANIO_PROYECTIL = 20.0f;
    p1->lanzar(400.0f, 0.0f);
    p1->x = 1279.0f;
    p1->actualizar(0.016f);
    p1->verificar_colision_paredes(1280.0f, 720.0f);
    std::cout << "BREAK activo tras borde: "
              << p1->Activa() << "\n";

    // Prueba 3: Proyectil BOUNCE
    Ball* p2 = new Ball(modoBall::projectile, ComportamientoColision::Rebote, 3);
    p2->DANIO_PROYECTIL = 20.0f;
    p2->lanzar(400.0f, 0.0f);
    p2->x = 1279.0f;
    for(unsigned short i = 0; i < 10; i++) {
        p2->actualizar(0.016f);
        p2->verificar_colision_paredes(1280.0f, 720.0f);
        std::cout << "Rebotes restantes: "
                  << p2->rebotes_restantes << "\n";
        std::cout << "Esta activa: " << p2->Activa() << "\n"; // ← agregar

        if(!p2->Activa())
        {

            std::cout << "Proyectil destruido en iteracion: "
                      << i << "\n";

            break;

        }
    }

    // Prueba 4: Daño
    Ball* p3 = new Ball(modoBall::projectile, ComportamientoColision::Roto);
    p3->DANIO_PROYECTIL = 20.0f;
    p3->x    = jugador->x;
    p3->y    = jugador->y;
    p3->verificar_colision_charater(jugador);
    std::cout << "Vida tras impacto: " << jugador->vida << "\n";

    // Liberar
    delete balon;
    delete jugador;
    delete p1;
    delete p2;
    delete p3;
}
