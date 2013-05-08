#ifndef ITEM_H
#define ITEM_H

#include <string>


// An item tracked by the JIT inventory system.

// Although this is currently a very simple structure, we anticipate
// that it may become considerably more complicated and considerably
// larger in later releases of the system. 

struct Item {
  std::string name;
  int amount;

  Item (std::string nm, int amt)
    : name(nm), amount(amt)
    {}

  Item()
    : amount(0)
    {}

  bool operator== (const Item& right) const
  {
    return name == right.name
      && amount == right.amount;
  }

};

#endif

