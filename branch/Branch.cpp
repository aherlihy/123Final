#include "Branch.h"


Branch::Branch(GLUquadric *quad, LetterList list)
{
    m_quad = quad;
    m_list = list;
    m_segs = new vector<Seg >();

    m_dir = 0;
    m_pos = Vector3(0, 0, 0);
}

Branch::~Branch() {
    m_segs->clear();
    if (m_segs) delete m_segs;
}

void Branch::drawBranch(void)
{
    for (int i=0; i<m_segs->size(); i++) {
        glPushMatrix();
        Seg cur = m_segs->at(i);

//        double phi = acos(m_dir.dot(Vector3(0.0, 0.0, 1.0)));
//        glRotatef(phi, 1.0, 0.0, 0.0);
//        Vector3 a = m_dir;
//        a.z = 0;
//        a.normalize();
//        Vector3 b = Vector3(m_dir.x, m_dir.y, 0.0);
//        b.normalize();
//        double theta = acos(a.dot(b));
//        glRotatef(theta, 0.0, 0.0, 1.0);
        glTranslatef(m_pos.x, m_pos.y, m_pos.z);

        glRotatef(60.0, 0.0, 1.0, 0.0);
        glRotatef(m_dir, 0.0, 0.0, 1.0);


        glMultMatrixd(cur.trans.data);
        if (cur.len > EPSILON) {
            glTranslated(0, 0, -cur.len);
            gluCylinder(m_quad, cur.base, cur.top, cur.len, 10, 1);
        }
        gluSphere(m_quad, cur.base, 10, 5);
        glPopMatrix();
    }
}

void Branch::drawSeg(Seg seg)
{
    for (int i=0; i<m_list.size(); i++) {

    }
}

void Branch::setList(LetterList list)
{
    m_list = list;
}

void Branch::parseList(void)
{
    if (m_segs) {
        delete m_segs;
    }
    m_segs = new std::vector<Seg >();
    std::stack<Matrix4x4 > transstack;
    transstack.push(Matrix4x4::identity());
    //Matrix4x4 curtrans = transstack.top();

    for (int i=0; i<m_list.size(); i++) {
        if (LSystem::isVariable(m_list[i])) { // make leaf
        } else if (m_list[i] == BEND_LEFT) {
            Matrix4x4 trans = getRotXMat(randDouble(-RAND_V, RAND_V)) * getRotYMat(-randDouble(RAND_H_MIN, RAND_H));
            transstack.top() = transstack.top() * trans;
        } else if (m_list[i] == BEND_RIGHT) {
            Matrix4x4 trans = getRotXMat(randDouble(-M_PI/10.0, M_PI/10.0)) * getRotYMat(randDouble(RAND_H_MIN, RAND_H));
            transstack.top() = transstack.top() * trans;
        } else if (m_list[i] == FORWARD) {
            double l = randDouble(RAND_L_MIN, RAND_L);
            Matrix4x4 trans = getTransMat(Vector4(0, 0, l, 0));
            transstack.top() = transstack.top() * trans;
            m_segs->push_back((Seg){l, RADIUS, RADIUS*SCALE, (transstack.top()).getTranspose()});
        } else if (m_list[i] == PUSH) {
            transstack.push(transstack.top());
        } else if (m_list[i] == POP) {
//            curtrans = transstack.top();
            transstack.pop();
        } else if (m_list[i] == DEC_SIZE) {
            Matrix4x4 trans = getScaleMat(Vector4(SCALE, SCALE, SCALE, 0));
            transstack.top() = transstack.top() * trans;
        } else if (m_list[i] == JITTER) {
            transstack.top() = transstack.top() * getRotXMat(randDouble(-RAND_V, RAND_V)) * getRotYMat(randDouble(-RAND_V, RAND_V));
        } else if (m_list[i] == V_0 || m_list[i] == V_1 || m_list[i] == V_2 || m_list[i] == V_3) {
            m_segs->push_back((Seg){0, RADIUS, RADIUS, transstack.top().getTranspose()});
        }
    }
}


double Branch::randDouble(double lo, double hi)
{
    double r = (double)rand()/(double)RAND_MAX;
    return lo + (hi-lo)*r;
}

void Branch::setPosition(Vector3 pos)
{
    m_pos = pos;
}

void Branch::setDirection(double dir)
{
    m_dir = dir;
}


// FORK_EVEN    V_0
// FORK_RIGHT   V_1
// FORK_LEFT    V_2
// FORK_BEND    V_3
// BEND_LEFT    C_PLUS
// BEND_RIGHT   C_MINUS
// FORWARD      F
// DEC_SIZE     C__MINUS // decrement size
