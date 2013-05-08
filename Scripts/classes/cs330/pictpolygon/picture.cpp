#include "picture.h"
#include "shape.h"
#include "point.h"
#include "graphics.h"

using namespace std;

/** Insert your code to implement the picture class here.

*/


void Picture::get(std::istream& in)
{
  clear();
  char c;
  while ((in >> c) && c != '[') ;
  while ((in >> c) && c != ']')
    {
      in.putback (c);
      Shape* s;
      in >> s;
      if (s != 0)
	{
	  add (*s);
	  delete s;
	}
    }
}
