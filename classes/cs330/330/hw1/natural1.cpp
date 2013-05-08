#include "natural.h"

using namespace std;

// The natural numbers are the non-negative integers.
// This class allows for the manipulation of natural numbers of
// arbitarily large size.

Natural::Natural()
//   Natural k;
//post:  k == 0
{
  nDigits = 1;
  leadingDigit = trailingDigit = new DigitList(0);
}

Natural::Natural (unsigned i)
//   Natural k = i;
//post:  k == i
{
  nDigits = 1;
  leadingDigit = trailingDigit = new DigitList(i);
  normalize();
}



Natural& Natural::operator++ ()
{
  ++(trailingDigit->data);
  normalize();
  return *this;
}

Natural Natural::operator++ (int)
{
  Natural original = *this;
  ++(trailingDigit->data);
  normalize();
  return original;
}

Natural& Natural::operator-- ()
{
  --(trailingDigit->data);
  normalize();
  return *this;
}

Natural Natural::operator-- (int)
{
  Natural original = *this;
  --(trailingDigit->data);
  normalize();
  return original;
}



void Natural::normalize()
// Between operations (though not necessarily during the implementation
// of an operation), all Natural numbers should be in "normal form" 
//   - each digit should be between 0 and 9, inclusive
//   - the leading digit should be non-zero, unless the number itself is
//     zero, in which case it will have a single zero digit
// This function scans the number, correcting any deviations from
// normal form.
{
  DigitList* d = trailingDigit;
  int carry = 0;
  while (d != 0)
    {
      d->data += carry;
      carry = 0;
      if (d->data >= 10)
	{
	  carry = d->data / 10;
	  d->data = d->data % 10;
	}
      else if (d->data < 0)
	{
	  carry = 1 + (-(d->data+1)) / 10;
	  d->data = (carry * 10 + d->data);
	  carry = -carry;
	}
      d = d->prev;
    }
  while (carry > 0)
    {
      addLeadingZero();
      leadingDigit->data = carry % 10;
      carry = carry / 10;
    }
  d = leadingDigit;
  while (d != 0 && d->data == 0 && d != trailingDigit) 
    {
      leadingDigit = d->next;
      delete d;
      leadingDigit->prev = 0;
      --nDigits;
      d = leadingDigit;
    }
  if (carry < 0)
    {
      clear();
      nDigits = 1;
      leadingDigit = trailingDigit = new DigitList(0);
    }
}


// Internal operations for list manipulation

void Natural::clear()
{
  for (DigitList* d = leadingDigit; d != NULL;)
      {
	DigitList* nxt = d->next;
	delete d;
	d = nxt;
      }
  nDigits = 0;
}



void Natural::addLeadingZero()
// adds a 0 digit to the front of the number
{
  DigitList* d = new DigitList(0, 0, leadingDigit);
  leadingDigit->prev = d;
  leadingDigit = d;
  ++nDigits;
}



void Natural::timesTen()
// adds a 0 digit to the end of the number, effectively 
//  multiplying it by 10
{
  DigitList* d = new DigitList(0, trailingDigit, 0);
  trailingDigit->next = d;
  trailingDigit = d;
  ++nDigits;
}


void Natural::divTen()
// Removes the trailing digit of the number, effectively 
//  dividing it by 10
{
  DigitList* d = trailingDigit;
  trailingDigit = d->prev;
  if (trailingDigit == 0)
    leadingDigit = 0;
  else
    trailingDigit->next = 0;
  delete d;
  --nDigits;
}


Natural operator+ (const Natural& i, const Natural& j)
{
  Natural k = i;
  k += j;
  return k;
}


Natural operator* (const Natural& i, const Natural& j)
{
  Natural sum;
  Natural ii = i;
  for (Natural::DigitList* d = j.trailingDigit; d != 0; d = d->prev)
    {
      Natural m = ii;
      m.multiplyByDigit(d->data);
      sum += m;
      ii.timesTen();
    }
  return sum;
}


Natural operator/ (const Natural& i, const Natural& j)
{
  Natural quotient;
  Natural divisor = j;
  Natural remainder = i;

  int nShifts = 0;

  while (divisor.nDigits < remainder.nDigits)
    {
      divisor.timesTen();
      ++nShifts;
    }

  for (int i = 0; i <= nShifts; ++i)
    {
      int digit = 0;
      while (divisor <= remainder)
	{
	  remainder = remainder - divisor;
	  ++digit;
	}
      if (i > 0)
	quotient.timesTen();
      quotient.trailingDigit->data = digit;
      divisor.divTen();
    }
  quotient.normalize();
  return quotient;
}
  


ostream&  operator<< (ostream& out, const Natural& k)
{
  for (Natural::DigitList* d = k.leadingDigit; d != 0; d = d->next)
    out << d->data;
  return out;
}


// Utility functions

void Natural::multiplyByDigit (unsigned i)
//pre: 0 <= i < 10
//   Natural k = k0;  k.multiplyBy(i);
//post: k == k0 * i
{
  for (DigitList* d = leadingDigit; d != 0; d = d->next)
    d->data *= i;
  normalize();
}


void Natural::divideBy2 ()
//   Natural k = k0;  j = k.divideBy2();
//post: k0 == 2*k  || k0 = 2*k + 1
{
  int carry = 0;
  for (DigitList* d = leadingDigit; d != 0; d = d->next)
    {
      int k = 10 * carry + d->data;
      d->data = k / 2;
      carry = k % 2;
    }
  normalize();
}






