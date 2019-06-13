#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <qdebug.h>
#include <iostream>
#include <time.h>
#include <rrt.h>
#include <QObject>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    grid = new Grid(size,cellSize,scale);
    addToScene(*scene,grid);

    rrt = new RRT(grid,scene);
    rrt->start();

    //srand(time(0));
/*
    int numObjects = rand() % 50;

    for(int i = 0; i < numObjects; i++)
    {
        int j0 = rand()%size;
        int k0 = rand()%size;
        int objectSize =rand()%20;

        for(int j = j0; j < (j0+objectSize); j++)
        {
            for(int k = k0; k <( k0+objectSize); k++)
            {
                if(j<size && k<size)
                {
                    grid->matriz[j][k]->status = 1;
                    grid->matriz[j][k]->update();
                }
            }
        }
    }

    pair <int,int> objectivePoint = make_pair(rand()%size,rand()%size);
    while(grid->matriz[objectivePoint.first][objectivePoint.second]->status==1)
            objectivePoint = make_pair(rand()%size,rand()%size);
    grid->matriz[objectivePoint.first][objectivePoint.second]->status=2;

    pair <int,int> startingPoint = make_pair(rand()%size,rand()%size);
    while(grid->matriz[startingPoint.first][startingPoint.second]->status==1)
        startingPoint = make_pair(rand()%size,rand()%size);
    grid->matriz[startingPoint.first][startingPoint.second]->status=3;

    scene->addEllipse(startingPoint.first*scale,startingPoint.second*scale,2,2);
    int validNodes = 0;

    adjaList.push_back(adjacentList());
    adjaList[0].id = 0;
    adjaList[0].father = -1;
    adjaList[0].coordinates = startingPoint;


    for(int i = 0; i < numNodes; i++)
    {
        pair <int,int> randPoint = make_pair(rand()%size,rand()%size);
        if(grid->matriz[randPoint.first][randPoint.second]->status==0)
        {
            pair <int,int> oldNode = adjaList[0].coordinates;
            int indexNode = 0;
            for(int j = 0; j < validNodes; j++)
            {
                pair <int,int> listedNode = adjaList[j].coordinates;
                if(dist(randPoint,listedNode) < dist(randPoint,oldNode))
                {
                    oldNode = listedNode;
                    indexNode = j;
                }
            }
            pair <int,int> newNode = stepFromTo(oldNode,randPoint);
            if(grid->matriz[newNode.first][newNode.second]->status==0 && toLine(newNode.first,newNode.second,oldNode.first,oldNode.second))
            {
                scene->addEllipse(newNode.first*scale,newNode.second*scale,2,2);
                scene->addLine(newNode.first*scale,newNode.second*scale,oldNode.first*scale,oldNode.second*scale);

                validNodes++;

                adjaList.push_back(adjacentList());
                adjaList[validNodes].id = validNodes;
                adjaList[validNodes].father = indexNode;
                adjaList[validNodes].coordinates = newNode;
                adjaList[indexNode].sons.push_back(validNodes);

                if(dist(newNode,objectivePoint)<=3)
                {
                    i=numNodes;
                    int count = validNodes;
                    do
                    {
                        scene->addEllipse(adjaList[count].coordinates.first*scale,adjaList[count].coordinates.second*scale,5,5,QPen(),QBrush(Qt::red));
                        count = adjaList[count].father;
                    }while(adjaList[count].father != -1);
                }

            }

        }
    }
*/

    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            connect(grid->matriz[i][j], SIGNAL(setPoints(QGraphicsSceneMouseEvent*,int,int)),grid,SLOT(setPoints(QGraphicsSceneMouseEvent*,int,int)));
        }
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),rrt,SLOT(addSquare()));
    connect(rrt,SIGNAL(stopTimer()),this,SLOT(stopTimer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::dist(pair <int,int> p1, pair <int,int> p2)
{
    return sqrt(pow(p2.first-p1.first,2)+pow(p2.second-p1.second,2));
}

pair <int,int> MainWindow::stepFromTo(pair <int,int> p1, pair <int,int> p2)
{
    if(dist(p1,p2) < epsilon)
    {
        return p2;
    }
    else
    {
        float theta = atan2((p2.second-p1.second),(p2.first-p1.first));
        return make_pair(p1.first+epsilon*cos(theta),p1.second+epsilon*sin(theta));
    }
}
/*
void MainWindow::mousePressEvent(QMouseEvent *event)
{
     qDebug() << "event->pos()";
}*/

void MainWindow::addToScene(QGraphicsScene &scene, Grid *matrix)
{
    /* Adiciona as células da Matriz na Scene */
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
           scene.addItem(matrix->matriz[i][j]);
        }
    }
}

bool MainWindow::toLine(int x1,	int y1,	int const x2,	int const y2)
{
    //Bresenham para HIMM
    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = std::abs(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = std::abs(delta_y) << 1;

    if(grid->matriz[x1][y1]->status==1)
        return false;
    //matrix->matriz[x1][y1]->update();

    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));

        while (x1 != x2)
        {
            // reduce error, while taking into account the corner case of error == 0
            if ((error > 0) || (!error && (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;

            if(grid->matriz[x1][y1]->status==1)
                return false;
            //matrix->matriz[x1][y1]->update();
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));

        while (y1 != y2)
        {
            // reduce error, while taking into account the corner case of error == 0
            if ((error > 0) || (!error && (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;
            if(grid->matriz[x1][y1]->status==1)
                return false;
            //matrix->matriz[x1][y1]->update();
        }
    }
    return true;
}

void MainWindow::on_pushButton_clicked()
{
    rrt->createMap();
}


void MainWindow::on_pushButton_2_clicked()
{
    rrt->startRRT();
    timer->start(500);
}



void MainWindow::on_spinBox_valueChanged(int arg1)
{
    rrt->numNodes = arg1;
}

void MainWindow::stopTimer()
{
    timer->stop();
}
