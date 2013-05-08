#include <fstream>
#include <algorithm>

#include "picture.h"	
#include "point.h"
#include "psgraphics.h"
#include "shape.h"

/*-------------------------------------------------------------*/





using namespace std;




RectangularArea getPictureBounds (string fileName)
{
  ifstream in (fileName.c_str());

  PSGraphics gr(fileName + ".bb");
  Picture p0(gr);

  Shape* shape;
  while (in >> shape)
    {
      if (shape != NULL)
	p0.add(*shape);
    }
  return p0.boundingBox();
}


void writePicture (Picture p, string fileName)
{
  RectangularArea bb = p.boundingBox();
  cout << "Original bounding box for " << fileName << " is " 
       << p.boundingBox() << endl;

  ofstream dataOut ((fileName + ".dat").c_str());
  cout << "Writing picture to  " << fileName << ".dat" << endl;
  dataOut << bb << "\n" << p << endl;

  cout << "Normalizing picture and drawing to  " << fileName << endl;
  p.translate (-bb.upperLeft().x(), -bb.upperLeft().y());
  double scaleFactor = 400.0 / max(1.0, max(bb.width(), bb.height()));
  p.scale (Point(0.0, 0.0), scaleFactor);
  p.draw();
}


void simpleTest (string inFileName, string outFileName,
		 const RectangularArea& bb)
{
  PSGraphics gr (outFileName);
  Picture p(gr);
  
  ifstream in (inFileName.c_str());
  Shape* shape;
  while (in >> shape)
    {
      if (shape != NULL)
	{
	  p.add(*shape);
	}
    }
  writePicture (p, outFileName);
}


void tiling (string inFileName, string outFileName,
		 const RectangularArea& bb)
{
  PSGraphics gr (outFileName);
  Picture p(gr);
  
  double scaleFactor = 1.0 / max(1.0, max(bb.width(), bb.height()));
  double offsetx = -bb.upperLeft().x();
  double offsety = -bb.upperLeft().y();
  double dx = scaleFactor * bb.width();
  double dy = scaleFactor * bb.height();

  for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      {
	ifstream in (inFileName.c_str());
	Shape* shape;
	while (in >> shape)
	  {
	    if (shape != NULL)
	      {
		shape->translate (offsetx, offsety);
		shape->scale (Point(0.0, 0.0), scaleFactor);
		if (i % 2 == 1) 
		  {
		    shape->reflectHorizontally(dx);
		    shape->translate (-dx, 0.0);
		  }
		if (j % 2 == 1) 
		  {
		    shape->reflectVertically(dy);
		    shape->translate (0.0, -dy);
		  }
		shape->translate (i*dx, j*dy);
		p.add(*shape);
	      }
	  }
      }
  writePicture (p, outFileName);
}


void kaleidoscope (string inFileName, string outFileName,
		 const RectangularArea& bb)
{
  PSGraphics gr (outFileName);
  Picture p(gr);
  
  double centerX = bb.upperLeft().x() + bb.width()/2.0;
  double centerY = bb.upperLeft().y() + bb.height()/2.0;


  ifstream in (inFileName.c_str());
  Shape* shape;
  while (in >> shape)
    {
      if (shape != NULL)
	{
	  shape->translate (-centerX, -centerY);
	  Shape* s1 = shape->clone();
	  Shape* s2 = shape->clone();
	  Shape* s3 = shape->clone();
	  s1->reflectHorizontally(0.0);
	  s3->reflectHorizontally(0.0);
	  s2->reflectVertically(0.0);
	  s3->reflectVertically(0.0);
	  p.add(*shape);
	  p.add(*s1);
	  p.add(*s2);
	  p.add(*s3);
	  delete shape;
	  delete s1;
	  delete s2;
	  delete s3;
	}
    }
  writePicture (p, outFileName);
}


int main(int argc, char** argv)
{
  if (argc != 2)
    {
      cerr << "Usage: " << argv[0] << " pictureFileName" << endl;
      return -1;
    }


  string inFileName (argv[1]);
  RectangularArea bb = getPictureBounds (argv[1]);
  simpleTest (inFileName, inFileName+".eps", bb);
  kaleidoscope (inFileName, inFileName+".kal.eps", bb);
  tiling (inFileName, inFileName+".tile.eps", bb);
  return 0;
}
