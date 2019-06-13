#ifndef RRT_H
#define RRT_H
#include <QThread>
#include <time.h>
#include <grid.h>
#include <utility>
#include <vector>
#include <QGraphicsScene>

using namespace std;

class RRT: public QThread
{
        Q_OBJECT
public:
    RRT(Grid * grid, QGraphicsScene * scene);

    Grid * grid;
    QGraphicsScene * scene;
    QGraphicsLineItem * edges;

    struct edge  {
       pair <int,int> src;
       pair <int,int> dest;
       QGraphicsLineItem * line;
    };

    struct adjacentList {
        int id;
        int father;
        pair <int,int> coordinates;
        QGraphicsEllipseItem * point;
        vector <int> sons;
    };

    vector < adjacentList > adjaList;
    vector < edge > edgesList;
    pair <int,int> startingPoint;
    pair <int,int> objectivePoint;
    int numNodes = 5000;
    int epsilon = 5;
    int step = 50;
    int countingSteps;
    int validNodes;


    float dist(pair <int,int> p1, pair <int,int> p2);
    bool toLine(int x1,	int y1,	int const x2,	int const y2);
    void addToScene(QGraphicsScene &scene, Grid *matrix);
    pair <int,int> stepFromTo(pair <int,int> p1, pair <int,int> p2);
    void startRRT();
    void constructRRT();
    void cleanLists();
    void createMap();

public slots:
    void addSquare();
protected:
    void run();
signals:
    void stopTimer();
};

#endif // RRT_H
