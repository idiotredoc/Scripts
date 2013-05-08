#include "polygon.h"

using namespace std;

Polygon::Polygon(Color edgeColor, Color fillColor)
  : Shape(edgeColor, fillColor)
{}


Polygon::Polygon(vector<Point> corners, int npoints, 
		     Color edgeColor, Color fillColor)
  : Shape(edgeColor, fillColor),
    _corners(corners), _npoints(npoints)
{
}

void Polygon::draw(Graphics& g) const
{  
  if (getFillColor() != Color::transparent)
   {
     g.setColor(getFillColor());
     for (int i=0; i< _npoints; i++)
     g.fillPolygon((_corners[i].x()),
		(_corners[i].y()),
		_npoints);
   }
  if (getEdgeColor() != Color::transparent)
    {
      g.setColor(getEdgeColor());
      for (int i=0; i< _npoints; i++)
      g.drawPolygon((_corners[i].x()),
                (_corners[i].y()),
                _npoints);

    }
}

void Polygon::put(ostream& os) const
{
  for (int i=0; i< _npoints; i++)
  os << "Polygon: " << _corners[i];
}

void Polygon::scale(const Point& center, double s)
{
  for (int i=0; i< _npoints; i++)
   _corners[i] = _corners[i].scale(center, s);
}

void Polygon::translate(double x, double y)
{
  for (int i=0; i< _npoints; i++) 
    _corners[i] = _corners[i].translate(x, y);
}

void Polygon::reflectHorizontally(double mirrorX)
{ 
  for (int i=0; i< _npoints; i++)
	_corners[i] = _corners[i].reflectHorizontally(mirrorX);
}

void Polygon::reflectVertically(double mirrorY)
{
   for (int i=0; i< _npoints; i++) 
        _corners[i] = _corners[i].reflectVertically(mirrorX); 
}

RectangularArea Polygon::boundingBox() const
{
  for (int i=0 i< _npoints; i++) 
  Point ul = Point(min(_corners[i].x(),
  return RectangularArea(ul,lr);
}

void Polygon::get(std::istream& in)
{
  in >> _edge >> _fill;   // read edge and fill colors
  int nCorners;
  in >> nCorners;
  _corners.clear();
  for (int i = 0; i < nCorners; ++i)
    {
      Point p;
      in >> p;
      _corners[i] = p;
      _npoints++;
    }
}

*/
