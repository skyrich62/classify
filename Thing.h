/*
 * $RCSfile:  $
 * $Revision:  $
 *
 * Comments:
 *
 * (C) Copyright Parasoft Corporation 211.  All rights reserved.
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF Parasoft
 * The copyright notice above does not evidence any
 * actual or intended publication of such source code.
 *
 * $Author: $          $Locker:  $
 * $Date:  $
 * $Log:  $
 *
 */

#ifndef THING_H_
#define THING_H_
#include <string>
#include <map>
#include <memory>

class Explainer;

class Thing {
public:
    Thing(const std::string& article, const std::string& name);
    ~Thing();
    bool isA(Thing* thing, Explainer& explain, bool &isOrIsNot) const;
    void addIs(Thing* thing, bool isOrIsNot);
    const std::string& name(void) const { return _name; }
    const std::string& article(void) const { return _article; }
    const std::string fullName() const { return article() + " " + name(); }
public:
    static Thing* fromString(const std::string& article, const std::string& name);
    static void save(std::ostream& ) ;
    static void load(std::istream&) ;
    static void forget();
private:
    const std::string _article;
    const std::string _name;

    typedef std::map<Thing*, bool> IsaSet;
    IsaSet _isa;

    typedef std::map<std::string, std::unique_ptr<Thing>> Knowledge;
    static Knowledge _knowledge;
};

#endif /* THING_H_ */
