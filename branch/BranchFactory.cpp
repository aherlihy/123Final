#include "BranchFactory.h"

BranchFactory::BranchFactory(GLUquadric *quadric)
{
    m_quadric = quadric;
    m_lsys = new LSystem();
}

BranchFactory::~BranchFactory()
{
    if (m_lsys) delete m_lsys;
}


Branch *BranchFactory::generateBranch(int n)
{
    LetterList axiom;
    axiom.push_back(C_F);
    axiom.push_back(randVar());
    m_lsys->setAxiom(axiom);
    m_lsys->setRule(V_0, var0rule());
    m_lsys->setRule(V_1, var1rule());
    m_lsys->setRule(V_2, var2rule());
    m_lsys->setRule(V_3, var3rule());

    for (int i=0; i<n; i++) m_lsys->iterate();

    Branch *branch = new Branch(m_quadric, m_lsys->getList());
    branch->parseList();
    return branch;
}

Letter BranchFactory::randVar(void)
{
    double r = (double)rand()/(double)RAND_MAX;
    if (r <= RAND_V0) {
        return V_0;
    } else if (r <= RAND_V1) {
        return V_1;
    } else if (r <= RAND_V2) {
        return V_2;
    } else {
        return V_3;
    }
}


LetterList BranchFactory::var0rule(void)
{
    // .[-FX][+FX]
    LetterList rule;
    rule.push_back(C__MINUS);
    rule.push_back(C_BRACK__);
    rule.push_back(C_MINUS);
    rule.push_back(C_F);
    rule.push_back(randVar());
    rule.push_back(__C_BRACK);
    rule.push_back(C_BRACK__);
    rule.push_back(C_PLUS);
    rule.push_back(C_F);
    rule.push_back(randVar());
    rule.push_back(__C_BRACK);
    return rule;
}

LetterList BranchFactory::var1rule(void)
{
    //.[+FX]~FX
    LetterList rule;
    rule.push_back(C__MINUS);
    rule.push_back(C_BRACK__);
    rule.push_back(C_PLUS);
    rule.push_back(C_F);
    rule.push_back(randVar());
    rule.push_back(__C_BRACK);
    rule.push_back(C_0);
    rule.push_back(C_F);
    rule.push_back(randVar());
    return rule;
}

LetterList BranchFactory::var2rule(void)
{
    // .[-FX]~FX
    LetterList rule;
    rule.push_back(C__MINUS);
    rule.push_back(C_BRACK__);
    rule.push_back(C_MINUS);
    rule.push_back(C_F);
    rule.push_back(randVar());
    rule.push_back(__C_BRACK);
    rule.push_back(C_0);
    rule.push_back(C_F);
    rule.push_back(randVar());
    return rule;
}

LetterList BranchFactory::var3rule(void)
{
    // .~FX
    LetterList rule;
    rule.push_back(C__MINUS);
    rule.push_back(C_0);
    rule.push_back(C_F);
    Letter var;
    while ((var = randVar()) == V_3);
    rule.push_back(var);
    return rule;
}
