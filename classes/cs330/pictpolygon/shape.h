#ifndef SHAPE_H
#define SHAPE_H

#include "figure.h"
#include "graphics.h"	
#include "color.h"
#include "rectarea.h"

class Point;

class Shape: public Figure
{
public:
  Shape (Color edgeColor = Color::black, 
	 Color fillColor = Color::transparent):
    _edge(edgeColor), _fill(fillColor)
    {}

  virtual ~Shape() {}
  
  virtual Shape* clone() const = 0;

  // Get/Set edge and fill colors
  Color getEdgeColor () const;
  void setEdgeColor (Color c);

  Color getFillColor () const;
  void setFillColor (Color c);

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
  
  // Draw a shape   
  virtual void draw(Graphics& g) const = 0;

  // Read a shape from an input stream
  virtual void get(std::istream& in) = 0;

protected:
  // Utility function for rounding off floating point numbers to
  // the nearest integer.
  static int round (double d) {return (int)(d + 0.5);}

  Color _edge;
  Color _fill;

};


inline
std::ostream& operator<< (std::ostream& out, const Shape& sh)
{
  sh.put(out);
  return out;
}


std::istream& operator>> (std::istream& in, Shape*& sh);

inline Color Shape::getEdgeColor() const
{
  return _edge;
}

inline Color Shape::getFillColor() const
{
  return _fill;
}

inline void Shape::setEdgeColor(Color c)
{
  _edge = c;
}

inline void Shape::setFillColor(Color c)
{
  _fill = c;
}


#endif
