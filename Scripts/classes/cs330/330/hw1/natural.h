#ifndef NATURAL_H
#define NATURAL_H

#include <iostream>
#include <utility>

// The natural numbers are the non-negative integers.
// This class allows for the manipulation of natural numbers of
// arbitarily large size.

class Natural
{
public:
  Natural();
  //   Natural k;
  //post:  k == 0

  Natural (unsigned i);
  //   Natural k = i;
  //post:  k == i

  bool operator== (const Natural& i) const;
  bool operator<= (const Natural& i)  const;
  bool operator< (const Natural& i) const;
  Natural& operator++ ();
  Natural operator++ (int);
  Natural& operator-- ();
  Natural operator-- (int);	
  Natural operator= (const Natural& i);
 

private:

  // Implemented as a linked list of digits, each digit should
  // be a number between 0 and 9, inclusive.
  struct DigitList {
    int data;
    DigitList* prev;
    DigitList* next;

    DigitList (unsigned d, DigitList* prv = 0, DigitList* nxt = 0)
      : data(d), prev(prv), next(nxt)
      {}
  };
  unsigned nDigits;
  DigitList* leadingDigit;
  DigitList* trailingDigit;


  void clear();
  // Empty the list

  void normalize();
  // Between operations (though not necessarily during the implementation
  // of an operation), all Natural numbers should be in "normal form" 
  //   - each digit should be between 0 and 9, inclusive
  //   - the leading digit should be non-zero, unless the number itself is
  //     zero, in which case it will have a single zero digit
  // This function scans the number, correcting any deviations from
  // normal form.

  // Internal operations for list manipulation
  void addLeadingZero();
  // adds a 0 digit to the front of the number

  void timesTen();
  // adds a 0 digit to the end of the number, effectively multiplying it by 10

  void divTen();
  // removes the trailing digit of a number, effectively dividing it by 10

  friend Natural operator+ (const Natural& i, const Natural& j);
  friend Natural operator- (const Natural& i, const Natural& j);
  friend Natural operator* (const Natural& i, const Natural& j);
  friend Natural operator/ (const Natural& i, const Natural& j);
  
  friend std::ostream&  operator<< (std::ostream& out, const Natural& k);


  // Utility functions

  void multiplyByDigit (unsigned i);
  //pre: 0 <= i < 10
  //   Natural k = k0;  k.multiplyBy(i);
  //post: k == k0 * i

  void divideBy2 ();  
  //   Natural k = k0;  j = k.divideBy2();
  //post: k0 == 2*k  || k0 = 2*k + 1

  void subtract1 ();
  //   Natural k = k0; k.subtract1();
  //Post:  k0 == k + 1

};

Natural operator+ (const Natural& i, const Natural& j);
Natural operator- (const Natural& i, const Natural& j);
Natural operator* (const Natural& i, const Natural& j);
Natural operator/ (const Natural& i, const Natural& j);

using namespace std::rel_ops;
  
std::ostream&  operator<< (std::ostream& out, const Natural& k);
  
Natural operator+= (const Natural& i, const Natural& j);

#endif
