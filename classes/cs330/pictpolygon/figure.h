#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>
#include "rectarea.h"

/** A "figure" in this context is anything that can be
    subjected to a small set of common graphics transformations.

    This is an abstract base class whose primary subclasses will be
      1) individual shapes and
      2) pictures: containers of shapes that, when drawn, produce
         output on a file or a window
**/



class Figure {
public:

  virtual ~Figure();


  // Read a figure from an input stream
  virtual void get(std::istream& in) = 0;

  // Write a description of this picture
  virtual void put(std::ostream& out) const = 0;
   
  // Shrink or magnify the picture by the
  // indicated amount (by scaling each shape relative to
  // the center).
  virtual void scale (const Point& center, double size) = 0;

  // Shift the picture by the indicated
  // indicated amount in the x and y directions.
  virtual void translate (double deltaX, double deltaY) = 0;


  // Transform the picture into the image that would be seen by looking
  // into a (double-sided) mirror lying along the line X=mirrorX
  virtual void reflectHorizontally (double mirrorX) = 0;

  // Transform the picture into the image that would be seen by looking
  // into a (double-sided) mirror lying along the line Y=mirrory
  virtual void reflectVertically (double mirrorY) = 0;

  // Compute the smallest rectangular area that encloses the picture
  virtual RectangularArea boundingBox() const = 0;

};


inline
std::ostream& operator<< (std::ostream& out, const Figure& p)
{
  p.put(out);
  return out;
}

#endif
