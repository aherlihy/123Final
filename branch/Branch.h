#ifndef BRANCH_H
#define BRANCH_H


#include "math.h"
#include "LSystem.h"
#include <vector>
#include <stack>
#include <qgl.h>
#include "CS123Algebra.h"

#define FORK_EVEN   V_0
#define FORK_RIGHT  V_1
#define FORK_LEFT   V_2
#define FORK_BEND   V_3
#define BEND_LEFT   C_PLUS
#define BEND_RIGHT  C_MINUS
#define FORWARD     C_F
#define DEC_SIZE    C__MINUS // decrement size
#define PUSH        C_BRACK__
#define POP         __C_BRACK
#define JITTER      C_0

#define RAND_V      M_PI*0.2
#define RAND_H      M_PI*0.2
#define RAND_H_MIN  -M_PI*0.05
#define RAND_L      3.0
#define RAND_L_MIN  0.2
#define RADIUS      0.14
#define SCALE       0.7


#define P1 10
#define P2 10
#define RAD(depth) (1.0f/((double)depth+1))


typedef struct {
    double len;
    double base;
    double top;
    Matrix4x4 trans;
} Seg;

class Branch
{
public:
    Branch(LetterList list);
    ~Branch();

    void drawBranch(GLUquadric *quadric);
    void drawSeg(Seg seg);
    void setList(LetterList list);
    void parseList(void);

    void setPosition(Vector3 pos);
    void setDirection(double dir);


private:
    LetterList m_list;
    std::vector<Seg> *m_segs;
    double randDouble(double lo, double hi);
    Vector3 m_pos;
    double m_dir;
};

#endif // BRANCH_H
