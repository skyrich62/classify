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

#ifndef EXPLAINER_H_
#define EXPLAINER_H_
#include <list>


class Thing;

class Explainer {
public:
    Explainer( );
    void addFact(Thing* thing1, Thing* thing2, bool isOrIsNot);
    void explain(void);
    void reset(void);
    bool empty() const { return _chain.empty(); }
private:
    class Fact {
    public:
        Fact(Thing*, Thing*, bool);
        Thing* thing1() const { return _thing1; }
        Thing* thing2() const { return _thing2; }
        bool   is() const     { return _assertion; }
    private:
        Thing* _thing1;
        Thing* _thing2;
        bool   _assertion;
    };
private:
    typedef std::list<Fact> FactList;
    FactList _chain;
};

#endif /* EXPLAINER_H_ */
