#include <iostream>
#include <sstream>
#include <string>
#include "inventory.h"


using namespace std;

/*
 * Move forward one month. Print prior month's report.
 * Start a fesh month, carrying over all known items and
 * factories, but clearing the orders.
 */
void monthCommand (JITInventory* months, int& numMonths);

/*
 * Begin tracking orders for a new item.
 */
void itemCommand (JITInventory& monthlyOrders, string newItemName);

/*
 * Begin tracking orders from a new factory
 */
void factoryCommand (JITInventory& monthlyOrders, string newFactoryName);


/*
 * A factory has requested delivery of some # of an item.
 */
void orderCommand (JITInventory& monthlyOrders, string details);



/*
 * Print the monthly report, showing first the total number of each 
 * item ordered, the nthe details as to what should be delivered 
 * to each factory.
 */
void printMonthlyReport (const JITInventory& ordersForThisMonth,
			 int monthNum);



/*
 * Print the summary report, showing the number of factories and # of
 * items ordered, per month.
 */
void printSummaryReport (const JITInventory* inventory,
			 int numMonths);



int main ()
{
  // Reads inventory instructions from standard input,
  // generating monthly order summaries.
  //
  // Inventory instructions:
  //
  // month         - print report for prior month (if any) and start a 
  //                    new month. A maximum of one year's worth of
  //                    monthly reports can be managed.
  // item:name    - begin tracking orders for items with the given name
  // factory:name - begin tracking orders for a factory with the given name
  // order:factory:item:# - A factory has determined that it will 
  //                        need # additional items for next month
  //                        If the same factor orders the same item more
  //                        than once, the orders are cumulative.
  
  JITInventory monthlyInventory[12];
  int numMonths = 1;

  string line;
  getline (cin, line);
  while (cin) {
    string::size_type loc =  line.find(":");
    string operatorName = line.substr(0, loc);
    string details;
    if (loc != string::npos) 
      details = line.substr(loc+1);
    if (operatorName == "month")
      monthCommand(monthlyInventory, numMonths);
    else if (operatorName == "item")
      itemCommand (monthlyInventory[numMonths-1],details);
    else if (operatorName == "factory")
      factoryCommand (monthlyInventory[numMonths-1],details);
    else if (operatorName == "order")
      orderCommand (monthlyInventory[numMonths-1],details);
    else if (operatorName != "")
      cout << "Illegal input: " << line << endl;
    getline (cin, line);
  }
  printMonthlyReport (monthlyInventory[numMonths-1], numMonths);
  printSummaryReport(monthlyInventory, numMonths);

  return 0;
}


/*
 * Move forward one month. Print prior month's report.
 * Start a fesh month, carrying over all known items and
 * factories, but clearing the orders.
 */
void monthCommand (JITInventory* months, int& numMonths)
{
  if (numMonths > 0)
    printMonthlyReport (months[numMonths-1], numMonths);
  if (numMonths < 12)
    {
      months[numMonths] = months[numMonths - 1];
      months[numMonths].clearOrders();
    }
  ++numMonths;
}

/*
 * Begin tracking orders for a new item.
 */
void itemCommand (JITInventory& monthlyOrders, string newItemName)
{
  JITInventory::ItemPosition pos = monthlyOrders.findItem(newItemName);
  if (pos == monthlyOrders.endItem())
    monthlyOrders.addItem (newItemName);
}

/*
 * Begin tracking orders from a new factory
 */
void factoryCommand (JITInventory& monthlyOrders, string newFactoryName)
{
  JITInventory::FactoryPosition pos
    = monthlyOrders.findFactory(newFactoryName);
  if (pos == monthlyOrders.endFactory())
    monthlyOrders.addFactory (newFactoryName);
}


/*
 * A factory has requested delivery of some # of an item.
 */
void orderCommand (JITInventory& monthlyOrders, string details)
{
  bool ok = true;

  string::size_type endFactoryPos = details.find(":");
  string::size_type endItemPos = details.find(":",endFactoryPos+1);

  string factoryName = details.substr(0, endFactoryPos);
  JITInventory::FactoryPosition fpos
    = monthlyOrders.findFactory(factoryName);
  if (fpos == monthlyOrders.endFactory())
    {
      cerr << "Unknown factory in order:" << details << endl; 
      ok = false;
    }


  string itemName = details.substr(endFactoryPos+1, 
				   endItemPos - endFactoryPos - 1);
  JITInventory::ItemPosition ipos = monthlyOrders.findItem(itemName);
  if (ipos == monthlyOrders.endItem())
    {
      cerr << "Unknown item in order:" << details << endl; 
      ok = false;
    }

  if (ok)
    {
      string numStr = details.substr(endItemPos+1);
      istringstream numIn (numStr);
      int numOrdered;
      numIn >> numOrdered;

      fpos -> orderItems (&(*ipos), numOrdered);
    }
}



/*
 * Print the monthly report, showing first the total number of each 
 * item ordered, the nthe details as to what should be delivered 
 * to each factory.
 */
void printMonthlyReport (const JITInventory& ordersForThisMonth,
			 int monthNum)
{
  cout << "\n\nTotal items ordered in month " << monthNum << "\n";
  for (JITInventory::ItemPosition itemp = ordersForThisMonth.beginItem();
       itemp != ordersForThisMonth.endItem(); ++itemp)
    {
      cout << itemp->name << "\t" << itemp->amount << endl;
    }

  cout << "\n\nItems ordered by factory\n";
  for (JITInventory::FactoryPosition factp = ordersForThisMonth.beginFactory();
       factp != ordersForThisMonth.endFactory(); ++factp)
    {
      Factory& factory = *factp;
      cout << factory.getName() << endl;
      for (Factory::OrderPosition pos = factory.begin();
       pos != factory.end(); ++pos)
	{
	  cout << "   " << pos->item->name << "\t" << pos->amount << endl;
	}
    }
}



/*
 * Print the summary report, showing the number of factories and # of
 * items ordered, per month.
 */
void printSummaryReport (const JITInventory* inventory,
			 int numMonths)
{
  JITInventory annual;
  cout << "\n\nSummary 0\n";
  for (int month = 1; month <= numMonths; ++month)
    {
      for (JITInventory::FactoryPosition factp 
	     = inventory[month-1].beginFactory();
	   factp != inventory[month-1].endFactory(); ++factp) {
	Factory& f = *factp;
	annual.addFactory (f.getName());
	Factory& f2 = *(annual.findFactory(f.getName()));
	for (Factory::OrderPosition ordp = f.begin(); 
	     ordp != f.end(); ++ordp) {
	  annual.addItem (ordp->item->name);
	  Item& it = *(annual.findItem(ordp->item->name));
	  f2.orderItems (&it, ordp->amount);
	}
      }


      int numFactories = 0;
      int numDistinctItems = 0;
      int numItems = 0;

      for (JITInventory::FactoryPosition factp = annual.beginFactory();
	   factp != annual.endFactory(); ++factp)
	++numFactories;

      for (JITInventory::ItemPosition itemp = annual.beginItem();
       itemp != annual.endItem(); ++itemp)
	{
	  ++numDistinctItems;
	  numItems += itemp->amount;
	}
      
      
      cout << "\n\nItems ordered by factory\n";
      for (JITInventory::FactoryPosition factp = annual.beginFactory();
	   factp != annual.endFactory(); ++factp) {
	Factory& factory = *factp;
	cout << factory.getName() << endl;
	for (Factory::OrderPosition pos = factory.begin();
	     pos != factory.end(); ++pos)
	  {
	    cout << "   " << pos->item->name << "\t" << pos->amount << endl;
	  }
      }

      cout << numFactories << " factories ordered "
	   << numItems << " items of "
	   << numDistinctItems << " different kinds." << endl;
    }
}
