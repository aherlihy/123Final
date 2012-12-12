#ifndef LSYSTEM_H
#define LSYSTEM_H

#include <vector>
#include <map>
#include <string>
#include <iostream>

enum Letter  // all of the letters of the "alphabet"
{
    //constants
    C_PLUS, C_MINUS,     // add / subtract constants (for turtle graphics)
    C__PLUS, C__MINUS,   // more add / subtract for 3D
    C_BRACK__, __C_BRACK,  // open bracket ([) / close bracket (]) (for state)
    C_F, C_G,                   // draw forward, go (turtle graphics)
    C_0, C_1, C_2, C_3, C_4,        // more constants
    V_0, V_1, V_2, V_3, V_4, V_5, V_6, V_7, V_8, V_9// variables
};

//using namespace std;
typedef std::vector< Letter > LetterList;




class LSystem
{
public:
    LSystem();
    ~LSystem();

    int iterate(void);
    void setAxiom(LetterList axiom);
    // map methods
    int setRule(Letter variable, LetterList rule);
    LetterList getRule(Letter variable);
    // list methods
    LetterList getList(void);

    static void printList(LetterList list);
    static int isVariable(Letter l);





private:
    LetterList m_list;
    LetterList m_axiom;
    std::vector<LetterList > m_rules;
    int m_depth;

};



#endif // LSYSTEM_H
