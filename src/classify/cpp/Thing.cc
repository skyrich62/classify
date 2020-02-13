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

#include "Thing.h"
#include "Explainer.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>

using namespace std;


Thing::Knowledge Thing::_knowledge;

typedef vector<std::string> VStrTok;

Explainer explainer;

Thing::Thing(const string& article, const string& name) :
    _article(article),
    _name(name)
{

}

Thing::~Thing()
{

}

bool
Thing::isA( Thing* thing, Explainer& explain, bool &isOrIsNot) const
{
    bool found = false;
    IsaSet::const_iterator iter = _isa.find(thing);
    found = iter != _isa.end();
    if (found) {
        isOrIsNot = iter->second;
        explain.addFact(const_cast<Thing*>(this), thing, iter->second);
    } else {
        for (iter = _isa.begin(); iter != _isa.end(); ++iter) {
            Thing* thing2 = iter->first;
            if (!iter->second) {
                continue;
            }
            found = thing2->isA(thing, explain, isOrIsNot);
            if (found) {
                explain.addFact(const_cast<Thing*>(this), thing2, iter->second);
                //isOrIsNot = iter->second;
                break;
            }
        }
    }
    return found;
}

void
Thing::addIs(Thing* thing, bool isOrIsNot)
{
    _isa[thing] = isOrIsNot;
}

Thing* Thing::fromString(const string& article, const string& name)
{
    Thing *thing = _knowledge[name].get();
    if (!thing) {
        thing = new Thing(article, name);
        _knowledge[name] = std::unique_ptr<Thing>(thing);
    }
    return thing;
}

void
Thing::save(ostream& r)
{
    for (Knowledge::const_iterator iter = _knowledge.begin();
                                   iter != _knowledge.end();
                                   ++iter)
    {
        Thing* thing = iter->second.get();
        for (IsaSet::const_iterator iter2 = thing->_isa.begin();
                                    iter2 != thing->_isa.end();
                                    ++iter2)
        {
            Thing* thing2 = iter2->first;
            string isOrIsNot = iter2->second ? " is " : " is not ";
            r << thing->fullName() << isOrIsNot << thing2->fullName() << endl;
        }
    }
}

void
Thing::load(istream& r)
{
    bool parse(const string&);
    do {
        string line;
        getline(r, line);
        if (!line.empty()) {
            cout << line << " ...";
            parse(line);
        }
    } while (r);
}

void
Thing::forget()
{
    _knowledge.clear();
}
