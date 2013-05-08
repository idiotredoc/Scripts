#ifndef ORDER_H
#define ORDER_H

#include <string>

class Item;

// An order placed by a factory for some number of items

struct Order {
  Item* item; // the item being ordered
  int amount; // how many are needed

  Order (Item* it, int amt)
    : item(it), amount(amt)
    {}

  Order()
    : item(0), amount(0)
    {}

  bool operator== (const Order& right) const
  {
    return item == right.item
      && amount == right.amount;
  }


};

#endif

