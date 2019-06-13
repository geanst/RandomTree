#include "rrt.h"
#include <qdebug.h>
#include <math.h>
#include <time.h>
#include <QTimer>

RRT::RRT(Grid * grid, QGraphicsScene * scene)
{
    this->grid = grid;
    this->scene = scene;
    this->countingSteps = 0;
}

void RRT::startRRT()
{
    cleanLists();
    //emit stopTimer();

    for(int i = 0; i < grid->size; i++)
         for(int j = 0; j < grid->size; j++)
             if(grid->matriz[i][j]->status == 2 || grid->matriz[i][j]->status == 3)
                grid->matriz[i][j]->status = 0;

    if(grid->startPoint.first>=0)
        startingPoint = grid->startPoint;
    else
    {
        startingPoint = make_pair(rand()%grid->size,rand()%grid->size);
        while(grid->matriz[startingPoint.first][startingPoint.second]->status==1)
            startingPoint = make_pair(rand()%grid->size,rand()%grid->size);
    }
    grid->matriz[startingPoint.first][startingPoint.second]->status=3;
    grid->matriz[startingPoint.first][startingPoint.second]->update();

    if(grid->endPoint.first>=0)
        objectivePoint = grid->endPoint;
    else
    {
        objectivePoint = make_pair(rand()%grid->size,rand()%grid->size);
        while(grid->matriz[objectivePoint.first][objectivePoint.second]->status==1)
            objectivePoint = make_pair(rand()%grid->size,rand()%grid->size);
    }
    grid->matriz[objectivePoint.first][objectivePoint.second]->status=2;
    grid->matriz[objectivePoint.first][objectivePoint.second]->update();




    adjaList.push_back(adjacentList());
    adjaList[0].id = 0;
    adjaList[0].father = -1;
    adjaList[0].point = new QGraphicsEllipseItem(startingPoint.first*grid->scale,startingPoint.second*grid->scale,2,2);
    scene->addItem(adjaList[0].point);
    adjaList[0].coordinates = startingPoint;

    countingSteps = 0;
    validNodes = 0;


}

void RRT::constructRRT()
{
    for(int i = countingSteps; i < countingSteps+step; i++)
    {

        pair <int,int> randPoint = make_pair(rand()%grid->size,rand()%grid->size);
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
                //scene->addEllipse(newNode.first*grid->scale,newNode.second*grid->scale,2,2);
                edgesList.push_back(edge());
                edgesList[validNodes].src = oldNode;
                edgesList[validNodes].dest = newNode;
                edgesList[validNodes].line = new QGraphicsLineItem(newNode.first*grid->scale,newNode.second*grid->scale,oldNode.first*grid->scale,oldNode.second*grid->scale);
                //scene->addLine(newNode.first*grid->scale,newNode.second*grid->scale,oldNode.first*grid->scale,oldNode.second*grid->scale);
                scene->addItem(edgesList[validNodes].line);

                validNodes++;

                adjaList.push_back(adjacentList());
                adjaList[validNodes].id = validNodes;
                adjaList[validNodes].father = indexNode;
                adjaList[validNodes].coordinates = newNode;
                adjaList[validNodes].point = new QGraphicsEllipseItem(newNode.first*grid->scale,newNode.second*grid->scale,2,2);
                scene->addItem(adjaList[validNodes].point);
                adjaList[indexNode].sons.push_back(validNodes);

                if(dist(newNode,objectivePoint)<=3)
                {
                    int count = validNodes;
                    do
                    {
                        //scene->addEllipse(adjaList[count].coordinates.first*grid->scale,adjaList[count].coordinates.second*grid->scale,5,5,QPen(),QBrush(Qt::red));
                        //adjaList[count].point->setScale(5);
                        scene->removeItem(adjaList[count].point);
                        adjaList[count].point =  new QGraphicsEllipseItem(adjaList[count].coordinates.first*grid->scale,adjaList[count].coordinates.second*grid->scale,5,5);
                        adjaList[count].point->setBrush(Qt::red);
                        scene->addItem(adjaList[count].point);
                        count = adjaList[count].father;
                    }while(adjaList[count].father != -1);
                    emit stopTimer();
                    return;
                }

            }
        }

    }
    if(countingSteps==numNodes)
    {
        emit stopTimer();
        return;
    }
}

void RRT::cleanLists()
{
    for(int i = 0; i < edgesList.size(); i++)
        scene->removeItem(edgesList[i].line);
    edgesList.clear();
    for(int i = 0; i < adjaList.size(); i++)
        scene->removeItem(adjaList[i].point);
    adjaList.clear();
    for(int i = 0; i < grid->size; i++)
         for(int j = 0; j < grid->size; j++)
             grid->matriz[i][j]->update();

}

void RRT::addSquare()
{
    constructRRT();
}


void RRT::createMap()
{
    cleanLists();

    grid->startPoint = make_pair(-1,-1);
    grid->endPoint = make_pair(-1,-1);

    int numObjects = rand() % 50;

    for(int j = 0; j < grid->size; j++)
    {
        for(int k = 0; k <grid->size; k++)
        {
            grid->matriz[j][k]->status = 0;
            grid->matriz[j][k]->update();
        }
    }

    for(int i = 0; i < numObjects; i++)
    {
        int j0 = rand()%grid->size;
        int k0 = rand()%grid->size;
        int objectSize =rand()%20;

        for(int j = j0; j < (j0+objectSize); j++)
        {
            for(int k = k0; k <( k0+objectSize); k++)
            {
                if(j<grid->size && k<grid->size)
                {
                    grid->matriz[j][k]->status = 1;
                    grid->matriz[j][k]->update();
                }
            }
        }
    }
}

void RRT::run()
{
    srand(time(0));
    createMap();
    grid->startPoint = make_pair(-1,-1);
    grid->endPoint = make_pair(-1,-1);

}

float RRT::dist(pair <int,int> p1, pair <int,int> p2)
{
    return sqrt(pow(p2.first-p1.first,2)+pow(p2.second-p1.second,2));
}

pair <int,int> RRT::stepFromTo(pair <int,int> p1, pair <int,int> p2)
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

bool RRT::toLine(int x1,	int y1,	int const x2,	int const y2)
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
