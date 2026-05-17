#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include "ball.h"
#include "character.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void actualizar();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* escena;
    QGraphicsEllipseItem* circulo;
    QTimer* timer;
    Ball* balon;
};
#endif // MAINWINDOW_H
