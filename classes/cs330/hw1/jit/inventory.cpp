#include "inventory.h"
#include <algorithm>

using namespace std;


// The JIT Inventory tracks a single month's orders for a variety of
// objects from a variety of factories.




JITInventory::JITInventory()
{
}


  
  
// Set number of items ordered by all factories to zero.
void JITInventory::clearOrders()
{
  for (FactoryPosition p = beginFactory(); p != endFactory(); ++p)
    p->clearOrders();
  for (ItemPosition p = beginItem(); p != endItem(); ++p)
    p->amount = 0;
}


// Begin tracking a new item
void JITInventory::addItem (std::string itemName)
{
  list<Item>::iterator pos = items.begin();
  while (pos != items.end()
	 && pos->name < itemName)
    ++pos;
  items.insert (pos, Item(itemName, 0));
}

// Begin tracking a new factory
void JITInventory::addFactory (std::string factoryName)
{
  list<Factory>::iterator pos = factories.begin();
  while (pos != factories.end()
	 && pos->getName() < factoryName)
    ++pos;
  factories.insert (pos, Factory(factoryName));
}


// Access to items
JITInventory::ItemPosition JITInventory::beginItem() const
{
  return ((JITInventory*)this)->items.begin();
}

JITInventory::ItemPosition JITInventory::endItem() const
{
  return ((JITInventory*)this)->items.end();
}

JITInventory::ItemPosition JITInventory::findItem (std::string itemName) const
{
  ItemPosition pos = beginItem();
  while (pos != endItem() && pos->name < itemName)
    ++pos;
  if (pos == endItem())
    return endItem();
  else if (itemName == pos->name)
    return pos;
  else
    return endItem();
}


// Access to factories
JITInventory::FactoryPosition JITInventory::beginFactory() const
{
  return ((JITInventory*)this)->factories.begin();
}

JITInventory::FactoryPosition JITInventory::endFactory() const
{
  return ((JITInventory*)this)->factories.end();
}

JITInventory::FactoryPosition 
  JITInventory::findFactory (std::string factoryName) const
{
  FactoryPosition pos = beginFactory();
  while (pos != endFactory() && pos->getName() < factoryName)
    ++pos;
  if (pos == endFactory())
    return endFactory();
  else if (factoryName == pos->getName())
    return pos;
  else
    return endFactory();
}


bool JITInventory::operator== (const JITInventory& inv) const
{
  return items == inv.items
    // && factories == inv.factories
    ;
}
