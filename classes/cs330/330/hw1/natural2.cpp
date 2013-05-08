#include "natural.h"

using namespace std;

void Natural::subtract1 ()
// Natural k = k0; k.subtract1();
// Post: k0 == k + 1
{
   for (DigitList* d = leadingDigit; d != 0; d = d->next)
	d->data -= 1;
   normalize();
}

Natural operator- (const Natural& i, const Natural& j)
{
  Natural k = i;
  k - j;
  return k;
}

bool Natural::operator<= (const Natural& i) const
{
  return (((trailingDigit < i.trailingDigit) && (leadingDigit < i.leadingDigit)) || ((trailingDigit == i.trailingDigit) && (leadingDigit == i.leadingDigit)));
} 
bool Natural::operator== (const Natural& i) const
{
  return ((trailingDigit == i.trailingDigit) && (leadingDigit == i.leadingDigit));
}

bool Natural::operator< (const Natural& i) const
{
  return ((trailingDigit < i.trailingDigit) && (leadingDigit < i.leadingDigit));
}

Natural operator+= (const Natural& i, const Natural& j)
{ 
  Natural k = i;
  k = k + j;
  return k; 
}

Natural Natural::operator= (const Natural& i)
{
  if (this != &i)
     {
        leadingDigit = i.leadingDigit;
        trailingDigit = i.trailingDigit;
        return *this;
     }	
}
