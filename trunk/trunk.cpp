#include "trunk.h"

#define CIRCLE_RAD 30.0
#define TRUNK_RAD 10.0




Trunk::Trunk()
{
    m_trunk = new std::vector<TreeSeg>();
    float cyl_rad = 2.5;
    float circle_rad = 4.0;
    float tess = 10.0;
    float base = cyl_rad + circle_rad;
    for (float h=.00001; h<circle_rad; h += circle_rad/tess) {
        TreeSeg newseg;
        newseg.base = base;
        base = circle_rad - sqrt(circle_rad*circle_rad - (h + circle_rad)*(h + circle_rad));
        //newseg.height = h;
        newseg.len = circle_rad/tess;
        newseg.top = base;
        m_trunk->push_back(newseg);
    }
}

Trunk::~Trunk()
{
    m_trunk->clear();
    delete m_trunk;
}

void Trunk::drawTrunk(GLUquadric *quadric)
{
    glPushMatrix();
    TreeSeg curseg;
    for(int i=0; i<m_trunk->size(); i++) {
        curseg = m_trunk->at(i);
        gluCylinder(quadric, curseg.base, curseg.top, curseg.len, 20, 5);
        glTranslatef(0, 0, curseg.len);
    }
    gluCylinder(quadric, curseg.top, curseg.top/5.0, 500.0f,20, 50);
    gluCylinder(quadric, 1.0, 1.0, 1150.0f, 100, 100);



    glPopMatrix();
}
