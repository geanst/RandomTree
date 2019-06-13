#include "grid.h"
#include "cell.h"
#include <QObject>
#include "qdebug.h"

Grid::Grid(int size, int cell_size, int scale)
{
    this->size = size;
    this->cell_size = cell_size;
    this->scale = scale;
    this->startPoint = make_pair(-1,-1);
    this->endPoint = make_pair(-1,-1);

    matriz = new Cell **[size];

    int xx = 0;
    for(int i = 0; i < size*scale; i += scale)
    {
        matriz[xx] = new Cell *[size];
        int yy = 0;
        for(int j = 0; j < size*scale; j += scale)
        {
            matriz[xx][yy] = new Cell(i, j, cell_size, scale);
            yy++;
        }
        xx++;
    }


}

void Grid::setPoints(QGraphicsSceneMouseEvent *event, int x, int y)
{
    x = x/scale;
    y = y/scale;
    if (event->button() == Qt::LeftButton)
    {
        for(int j = 0; j < size; j++)
        {
            for(int k = 0; k <size; k++)
            {
                if(matriz[j][k]->status == 3)
                {
                    matriz[j][k]->status = 0;
                    matriz[j][k]->update();
                }
            }
        }
        this->startPoint = make_pair(x,y);
        matriz[x][y]->status = 3;
        matriz[x][y]->update();
    }
    else if (event->button() == Qt::RightButton)
    {
        for(int j = 0; j < size; j++)
        {
            for(int k = 0; k <size; k++)
            {
                if(matriz[j][k]->status == 2)
                {
                    matriz[j][k]->status = 0;
                    matriz[j][k]->update();
                }
            }
        }
        this->endPoint = make_pair(x,y);
        matriz[x][y]->status = 2;
        matriz[x][y]->update();
    }
}
