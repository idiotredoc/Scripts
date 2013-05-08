#ifndef PSPICTURE_H
#define PSPICTURE_H

#include <iostream>
#include <string>
#include "figure.h"
#include "graphics.h"
#include "rectarea.h"


/**  A Picture is a Figure that contains a number of shapes, and supports
     the drawing of those shapes onto a given graphics device **/

class Shape;




class Picture: public Figure {
public:

  // Create a new picture, associated with the indicated Graphics device
  Picture(Graphics& gd);

  Picture (const Picture&);

  ~Picture();

  
  // Remove all shapes form the picture
  void clear();

  // Add a shape
  void add (const Shape&);

  // Draw the currrent picture to the file.
  void draw() const;

  // Read a figure from an input stream
  virtual void get(std::istream& in);

  // Write a description of this picture
  virtual void put(std::ostream& out) const;
   
  // Shrink or magnify the picture by the
  // indicated amount (by scaling each shape relative to
  // the center).
  virtual void scale (const Point& center, double factor);

  // Shift the picture by the indicated
  // indicated amount in the x and y directions.
  virtual void translate (double deltaX, double deltaY);


  // Transform the picture into the image that would be seen by looking
  // into a (double-sided) mirror lying along the line X=mirrorX
  virtual void reflectHorizontally (double mirrorX);

  // Transform the picture into the image that would be seen by looking
  // into a (double-sided) mirror lying along the line Y=mirrory
  virtual void reflectVertically (double mirrorY);

  // Compute the smallest rectangular area that encloses the picture
  virtual RectangularArea boundingBox() const;

private:

  /** Insert your own declarations here to store the shapes and 
      the Graphics device that make up this picture. */

};


inline
std::ostream& operator<< (std::ostream& out, const Picture& p)
{
  p.put(out);
  return out;
}

#endif
