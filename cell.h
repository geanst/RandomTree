#ifndef CELL_H
#define CELL_H

#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>


class Cell: public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Cell(int x, int y, int cellSize, int scale);
        QColor color;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    int x;
    int y;
    int scale;
    int status; // 0 = empty, 1 = obstacle, 3 = goal
    int cellSize;
    bool visited;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void setPoints(QGraphicsSceneMouseEvent *event, int x, int y);




};

#endif // CELL_H
