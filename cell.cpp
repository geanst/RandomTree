#include "cell.h"
#include <QPainter>
#include "qdebug.h"
#include "math.h"
#include <sstream>
#include <QObject>
#include<bits/stdc++.h>
using namespace std;


Cell::Cell(int x, int y, int cellSize, int scale)
{
    this->visited = false;
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->cellSize = cellSize;
    this->status = 0;
    this->color = Qt::gray;
    setZValue(0);
}

QRectF Cell::boundingRect() const
{
    return QRectF(x-cellSize/2, y-cellSize/2, cellSize, cellSize);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QRectF rect = boundingRect();

    if(status==1)
        this->color = Qt::green;
    else if(status==2)
        this->color = Qt::red;
    else if(status==3)
        this->color = Qt::blue;
    else
        this->color = Qt::gray;
    painter->fillRect(rect, color);

}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  /*  if (event->button() == Qt::LeftButton && this->status!=1)
    {
        this->status=3;
        update();
    }
    else if (event->button() == Qt::RightButton && this->status!=1)
    {
        this->status=2;
        update();
    }*/
    if (this->status!=1)
        setPoints(event, this->x, this->y);
}


