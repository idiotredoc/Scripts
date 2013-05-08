#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"
#include "factory.h"
#include <list>

// The JIT Inventory tracks a single month's orders for a variety of
// objects from a variety of factories.


class JITInventory {
public:
  typedef std::list<Item>::iterator ItemPosition;
  typedef std::list<Factory>::iterator FactoryPosition;


  JITInventory();  
  
  
  // Set number of items ordered by all factories to zero.
  void clearOrders();

  // Begin tracking a new item
  void addItem (std::string itemName);

  // Begin tracking a new factory
  void addFactory (std::string factoryName);


  // Access to items (in alphabetic order by item name)
  ItemPosition beginItem() const;
  ItemPosition endItem() const;

  // Find the position of the Item with the given name.
  // If no such factory exists, return endItem();
  ItemPosition findItem (std::string itemName) const;


  // Access to factories (in alphabetic order by factory name)
  FactoryPosition beginFactory() const;
  FactoryPosition endFactory() const;

  // Find the position of the factory with the given name.
  // If no such factory exists, return endFactory();
  FactoryPosition findFactory (std::string factoryName) const;

  bool operator== (const JITInventory& inv) const;

private:
  std::list<Item> items;

  std::list<Factory> factories;
};

#endif
