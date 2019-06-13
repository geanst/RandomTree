#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include <QColor>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <utility>
using namespace std;

class Grid: public QObject
{
            Q_OBJECT
    public:
        Cell *** matriz;
        Grid(int size, int cell_size, int scale);


        void updateCell(int x, int y, QColor new_color);
        int size;
        int cell_size;
        int scale;
        pair <int,int> startPoint;
        pair <int,int> endPoint;


public slots:
        void setPoints(QGraphicsSceneMouseEvent *event, int x, int y);


};

#endif // GRID_H
