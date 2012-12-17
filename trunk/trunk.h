#ifndef TRUNK_H
#define TRUNK_H

#include "common.h"
#include <QtOpenGL>
#include <vector>

typedef struct {
    float base;
    float top;
    float len;
   // float height;
} TreeSeg;


class Trunk
{
public:
    Trunk();
    ~Trunk();
    void drawTrunk(GLUquadric *quadric);
private:
    std::vector<TreeSeg> *m_trunk;

};

#endif // TRUNK_H
