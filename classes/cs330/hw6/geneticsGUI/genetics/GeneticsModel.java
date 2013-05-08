package genetics;
import java.awt.Color;
import java.awt.Dimension;

public class GeneticsModel {

  private Dimension size;

  private int matingInterval;       // On any generation, a cell has a 
                                    // 1/fertilityInterval chance of mating
                                    // (1..10000)

  private int fertility;            // A mating produces this many offspring
                                    //   (1..8)

  private boolean oppositesAttract; //  Do mating cells combine with 
                                    //  neighboring cells most similar to 
                                    //  or different from themselves?

  private int mutationInterval;     // Any gene (bit) combined from two 
                                    // parents has a 1/mutationInterval chance
                                    // being flipped in the child.
                                    // (0..10000)  0: special, means never mutate



  private Color cells[];


  private int forceRange (int value, int min, int max)
  {
    if (value < min)
      return min;
    else if (value > max)
      return max;
    return value;
  }


  public GeneticsModel(Dimension initialSize)
  {
    matingInterval = 2;
    fertility = 6;
    oppositesAttract = false;
    mutationInterval = 4096;
    size = null;
	setSize (initialSize);
  }


  public int  getMatingInterval() { return matingInterval; }
  public void setMatingInterval(int val) 
  { matingInterval = forceRange(val, 1, 10000); }


  public int  getFertility() { return fertility; }
  public void setFertility(int val) 
  { fertility = forceRange(val, 1, 8); }

  public int  getMutationInterval() { return mutationInterval; }
  public void setMutationInterval(int val) 
  { mutationInterval = forceRange(val, 0, 10000); }

  public boolean getOppositesAttract() { return oppositesAttract; }
  public void    setOppositesAttract(boolean val) 
  { oppositesAttract = val; }


  
  public void setSize(Dimension newSize)
  {
    if (size == null || !size.equals(newSize)) {
      size = newSize;
      cells = new Color[size.width*size.height];
    }
    double w = (double)size.width;
    double scale = 2.0 * Math.PI / 50.0;
	for (int y = 0; y < size.height; ++y)
      for (int x = 0; x < size.width; ++x) {
        double xx = (double)x;
        double yy = (double)y;
        double d1 = Math.sqrt(xx*xx + yy*yy);
        double d2 = Math.sqrt((w-xx)*(w-xx) + yy*yy);

        double r = 0.5 * (1.0 + Math.cos(d1*scale));
        double g = 0.5 * (1.0 + Math.cos(d2*scale));
        double b = 0.5 * (1.0 + Math.sin(yy*scale/2.0));
        cells[index(x,y)] = new Color((float)r, (float)g, (float)b);
      }
  }


  public Dimension getSize() { return size; }
  

  // Simulate a 2D array, mapping (x,y) coordinates onto a unique int
  private int index(int x, int y) { return x * size.height + y; }

  public Color getCell(int x, int y) 
  {
    if (x >= 0 && x < size.width && y >= 0 && y < size.height)
      return cells[index(x,y)];
    else
      return null;
  }

  public void fillRegion (int x0, int y0, int x1, int y1, Color c)
  {
    x0 = forceRange(x0, 0, size.width-1);
    x1 = forceRange(x1, 0, size.width-1);
    y0 = forceRange(y0, 0, size.height-1);
    y1 = forceRange(y1, 0, size.height-1);

    if (x0 > x1) {
      int t = x1;
      x1 = x0;
      x0 = t;
    }
    if (y0 > y1) {
      int t = y1;
      y1 = y0;
      y0 = t;
    }
    for (int x = x0; x <= x1; ++x)
      for (int y = y0; y <= y1; ++y)
        cells[index(x,y)] = c;
  }
    

  public void nextGeneration ()
  {
	Color[] newCells = new Color[size.width*size.height];
	for (int x = 0; x < size.width; ++x)
	    for (int y = 0; y < size.height; ++y)
          newCells[index(x,y)] = cells[index(x,y)];

	for (int x = 0; x < size.width; ++x)
      for (int y = 0; y < size.height; ++y) {
        // Is cell(x,y) going to mate?
        if (Math.random() < 0.5/(float)matingInterval) {
          // Choose a mate from among its neighbors
          int mate = -1;
          Color c = cells[index(x,y)];
          int bestScore = (oppositesAttract) ? -1 : 10000;
          for (int mx = x-1; mx <= x+1; ++mx)
            if (mx >= 0 && mx < size.width)
              for (int my = y-1; my <= y+1; ++my)
                if (my >= 0 && my < size.height && (mx != x || my != y)) {
                  Color cm = cells[index(mx,my)];
                  int score = mateScore(c, cm);
                  if ((oppositesAttract && (score > bestScore))
                      || ((!oppositesAttract) && (score < bestScore))) {
                    bestScore = score;
                    mate = index(mx, my);
                  }
                }
          // Produce the offspring
          for (int childNum = 0; childNum < fertility; ++childNum)
          {
            Color child = mixGenes(c, cells[mate]);
            // and put it in a randomly chosen neighboring position
            int cx = x + (int)(3.01*Math.random()) - 1;
            int cy = y + (int)(3.01*Math.random()) - 1;
            if (cx >= 0 && cx < size.width && cy >= 0 && cy < size.height)
              newCells[index(cx,cy)] = child;
          }
        }
      }
    cells = newCells;
  }


  private int mateScore (Color c1, Color c2)
  {
    return mateScore(c1.getRed(), c2.getRed())
      + mateScore(c1.getGreen(), c2.getGreen())
      + mateScore(c1.getBlue(), c2.getBlue());
  }

  private int mateScore (int c1, int c2)
  {
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
      sum += ((c1 % 2) == (c2 % 2)) ? 0 : 1;
      c1 /= 2;
      c2 /= 2;
    }
    return sum;
    // return Math.abs(c1-c2);
  }

  private Color mixGenes (Color c1, Color c2)
  {
    return new Color(mixGenes(c1.getRed(), c2.getRed()),
                     mixGenes(c1.getGreen(), c2.getGreen()),
                     mixGenes(c1.getBlue(), c2.getBlue()));
  }

  private int mixGenes (int c1, int c2)
  {
    int mult = 1;
    int result = 0;
    // Combine each bit of the genome
    for (int geneNum = 0; geneNum < 8; ++geneNum) {
      int gene1 = c1 % 2;
      int gene2 = c2 % 2;
      int newGene;
      if (gene1 == gene2)
        newGene = gene1;
      else
        newGene = (Math.random() < 0.5) ? gene1 : gene2;
      // Mutation?
      if (mutationInterval > 0 && Math.random() < 1.0/(float)mutationInterval)
        newGene = (Math.random() < 0.5) ? 1 : 0;

      // Combine new gene into result
      result += mult * newGene;
      mult *= 2;

      c1 /= 2;
      c2 /= 2;
    }
    return result;
  }


}
