#ifndef BRANCHFACTORY_H
#define BRANCHFACTORY_H

#include "Branch.h"
#include "LSystem.h"

#define RAND_V0 .2
#define RAND_V1 RAND_V0+.25
#define RAND_V2 RAND_V1+.25
#define RAND_V3 RAND_V2+.3


class BranchFactory
{
public:
    BranchFactory();
    ~BranchFactory();
    Branch *generateBranch(int n);

private:
    Letter randVar(void);

    LSystem *m_lsys;

    LetterList var0rule(void);
    LetterList var1rule(void);
    LetterList var2rule(void);
    LetterList var3rule(void);

};

#endif // BRANCHFACTORY_H
