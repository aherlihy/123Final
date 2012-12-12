#include "LSystem.h"

// LetterList m_list;
// LetterList m_axiom;
// RuleMap m_map;
// int m_depth;

LSystem::LSystem()
{
    m_list = LetterList(0);
    m_axiom = LetterList(0);

    m_rules = std::vector<LetterList >(10);

    for (int i=0; i<10; i++) m_rules[i] = LetterList(1);

    m_rules[0][0] = V_0;
    m_rules[1][0] = V_1;
    m_rules[2][0] = V_2;
    m_rules[3][0] = V_3;
    m_rules[4][0] = V_4;
    m_rules[5][0] = V_5;
    m_rules[6][0] = V_6;
    m_rules[7][0] = V_7;
    m_rules[8][0] = V_8;
    m_rules[9][0] = V_9;

    m_depth = 0;
}


LSystem::~LSystem()
{

}



int LSystem::iterate(void)
{
    LetterList newlist;

    for (int i=0; i < m_list.size(); i++) {
        if (isVariable(m_list[i])) {
            LetterList rule = getRule(m_list[i]);
            newlist.insert(newlist.end(), rule.begin(), rule.end());
        } else {
            newlist.push_back(m_list[i]);
        }
    }

    m_list = newlist;
    m_depth++;
    return m_depth;
}

void LSystem::setAxiom(LetterList axiom)
{
    m_axiom = axiom;
    m_list = m_axiom;
}


int LSystem::setRule(Letter l, LetterList rule)
{
    if (isVariable(l)) {
        int i = l - V_0;
        m_rules[i] = rule;
        return 1;
    } else {  // not acceptable
        return 0;
    }
}


LetterList LSystem::getRule(Letter l)
{
    if (isVariable(l)) {
        int i = l - V_0;
        return m_rules.at(i);
    } else {
        LetterList ll;
        ll.push_back(l);
        return ll;
    }
}


/* List methods */


LetterList LSystem::getList(void)
{
    return m_list;
}


int LSystem::isVariable(Letter l)
{
    if (l==V_0 || l==V_1 || l==V_2 || l==V_3 || l==V_4 || l==V_5 || l==V_6 || l==V_7 || l==V_8 || l==V_9) { // l is a variable
        return 1;
    } else { // l is a constant
        return 0;
    }
}


void LSystem::printList(LetterList list)
{
    using namespace std;
    cout << "size:" << list.size() << "| ";
    for (int i=0; i<list.size(); i++) {
        switch (list[i]) {
        case V_0:
            cout << "V0";
            break;
        case V_1:
            cout << "V1";
            break;
        case V_2:
            cout << "V2";
            break;
        case V_3:
            cout << "V3";
            break;
        case V_4:
            cout << "V4";
            break;
        case V_5:
            cout << "V5";
            break;
        case V_6:
            cout << "V6";
            break;
        case V_7:
            cout << "V7";
            break;
        case V_8:
            cout << "V8";
            break;
        case V_9:
            cout << "V9";
            break;
        case C_PLUS:
            cout << "+";
            break;
        case C_MINUS:
            cout << "-";
            break;
        case C__PLUS:
            cout << "++";
            break;
        case C__MINUS:
            cout << "--";
            break;
        case C_F:
            cout << "F";
            break;
        case C_G:
            cout << "G";
            break;
        case C_BRACK__:
            cout << "[";
            break;
        case __C_BRACK:
            cout << "]";
            break;
        case C_0:
            cout << "C0";
            break;
        case C_1:
            cout << "C1";
            break;
        case C_2:
            cout << "C2";
            break;
        case C_3:
            cout << "C3";
            break;
        case C_4:
            cout << "C4";
            break;
        default:
            cout << "ERROR";
            break;
        }
        cout << " ";
    }
    cout << "\n";
}




