#ifndef POLYGON_H
#define POLYGON_H



#include "shape.h"
#include "point.h"
#include "rectangle.h"
#include <vector>


class Polygon : public Shape
{
public:
  Polygon (Color edgeColor = Color::black, 
	 Color fillColor = Color::transparent):
  Polygon (vector<Point> corners, int npoints,
	  Color edgeColor = Color::black,
	  Color fillColor = Color:: transparent);
  
//  virtual * clone() const = 0;

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

  vector<Point> _corners;
  int _npoints;

};

#endif
