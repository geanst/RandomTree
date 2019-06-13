#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <grid.h>
#include <utility>
#include <vector>
#include <rrt.h>
#include <QMouseEvent>
#include <QTimer>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QGraphicsScene * scene;
    Grid * grid;
    RRT * rrt;
    QTimer * timer;



    struct edge  {
       pair <int,int> src;
       pair <int,int> dest;
       QGraphicsLineItem * line;
    };

    struct adjacentList {
        int id;
        int father;
        pair <int,int> coordinates;
        vector <int> sons;
    };

    int size = 100;
    int cellSize = 9;
    int scale = 10;
    int numNodes = 10000;
    int epsilon = 5;

    vector < adjacentList > adjaList;

    float dist(pair <int,int> p1, pair <int,int> p2);
    bool toLine(int x1,	int y1,	int const x2,	int const y2);
    void addToScene(QGraphicsScene &scene, Grid *matrix);
    pair <int,int> stepFromTo(pair <int,int> p1, pair <int,int> p2);
 //   void mousePressEvent(QMouseEvent *event) override;

    Ui::MainWindow *ui;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);
    void stopTimer();
};

#endif // MAINWINDOW_H
