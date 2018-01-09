/*
 * $RCSfile:  $
 * $Revision:  $
 *
 * Comments:
 *
 * $Author: $          $Locker:  $
 * $Date:  $
 * $Log:  $
 *
 */

#include "Explainer.h"
#include "Thing.h"
#include <iostream>

using namespace std;

Explainer::Explainer()
{  }

void explainFact(Thing* thing1, Thing* thing2, bool is)
{
    string isOrIsNot = is ? " is ": " is not ";
    cout << thing1->fullName() << isOrIsNot
         << thing2->fullName();
}

void Explainer::explain()
{
    if (_chain.size() == 0) {
        cout << "I have no facts to support such a position." << endl;
        return;
    }
    if (_chain.size() == 1) {
        FactList::const_iterator iter = _chain.begin();
        if (iter->thing1() == iter->thing2()) {
            cout << "You're kidding, right?" << endl;

        } else {
            cout << "Because you said, \"";
            explainFact(iter->thing1(), iter->thing2(), iter->is());
            cout << "\"." << endl;
        }
        return;
    }
    for (FactList::const_iterator iter = _chain.begin();
                              iter != _chain.end();
                              ++iter) {
        if (iter == _chain.begin()) {
            cout << "Because ";
        } else {
            cout << "    and ";
        }
        explainFact(iter->thing1(), iter->thing2(), iter->is());
        cout << endl;
    }

}

void Explainer::addFact(Thing* thing1, Thing* thing2, bool isOrIsNot)
{
    Fact fact(thing1, thing2, isOrIsNot);
    _chain.push_front(fact);
}

void Explainer::reset()
{
    _chain.clear();
}

Explainer::Fact::Fact(Thing* thing1, Thing* thing2, bool isOrIsNot) :
        _thing1(thing1),
        _thing2(thing2),
        _assertion(isOrIsNot)
{
}

