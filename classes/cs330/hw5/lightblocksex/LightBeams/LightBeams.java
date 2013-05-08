package LightBeams;

import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;

/**
 *  The main program and window launcher for the LightBeams GUI.
 *
 */
public class LightBeams extends JApplet
{
    static final int gridSize = 12;
    static final int recalcLimit = gridSize * gridSize * gridSize;
    private LightCells[][] cells;

    private RegisteredBlocks registeredBlocks;


    public LightBeams ()
    {
    }

    public void init()
    {
	Container pane = getContentPane();
	pane.setLayout (new GridLayout (gridSize, gridSize));

	cells = new LightCells[gridSize][gridSize];

	registeredBlocks = new RegisteredBlocks();
	LightBlockRegistration.doRegistrations (registeredBlocks);

	MouseAdapter cellSelected = new MouseAdapter()
	{
	    public void mouseClicked (MouseEvent e)
	    {
		Component src = (Component)e.getSource();
		while (src != null && !(src instanceof LightCells))
		    src = src.getParent();
		LightCells cell = (LightCells)src;

		LightBlocks lb = registeredBlocks.selectBlock(cell);
		if (lb != null) {
		    cell.setBlock (lb);
		    // System.out.println ("Added a "+lb.getName() + " block");
		}
		cell.repaint();
		recalc();
	    }
	};

	
	// Fill in the grid with our cells and labels
	LightBlocks transp = registeredBlocks.getAnyBlock("Transparent");
	for (int j = 0; j < gridSize; ++j)
	    {
		for (int i = 0; i < gridSize; ++i)
		    {
		      cells[i][j] = new LightCells(new Location(i,j),
						   transp, cellSelected);
		      pane.add (cells[i][j]);
		    }
	    }
	

	recalc();
	
    }

    public void start() {}
    public void stop() {}
    public void destroy() {}

    
    public void recalc()
    {
      // calculate the light coming from each block


	// Start by setting the output of all cells to Black
	// and adding all cells to a list of changed cells needing
	// (re)evaluation
	Illumination allBlack = new Illumination();
	java.util.LinkedList<LightCells> changes 
	    = new java.util.LinkedList<LightCells>(); 
	for (int j = 0; j < gridSize; ++j)
	    for (int i = 0; i < gridSize; ++i) {
		cells[i][j].setOutput(allBlack);
		changes.addLast(cells[i][j]);
	    }
      

	// For each cell needing evaluation, compute its output. If this
	// has changed, add it's neighbors to the list to force them to be
	// re-evaluated later.
	for (int k = 0; (!changes.isEmpty()) && (k < recalcLimit); ++k)
	{
	  LightCells cell = changes.getFirst();
	  changes.removeFirst();

	  // Compute the current input illumination for this cell
	  Location loc = cell.getCellLocation();
	  int x = loc.x;
	  int y = loc.y;
	  Illumination input = new Illumination
	      (getNeighborIllumination(x, y-1).S, 
	       getNeighborIllumination(x, y+1).N, 
	       getNeighborIllumination(x+1, y).W,
	       getNeighborIllumination(x-1, y).E);
	  Illumination oldOutput = cell.getOutput();
	  Illumination output = cell.getBlock().illuminate(input);

	  /*
	    System.out.println ("" + loc
			      + "\tinput=" + input
			      + "\toldOutput=" + oldOutput
			      + "\toutput=" + output);
	  */

	  // Check for output changes and add neighbors to list as needed
	  if (legalLocation(x,y-1) && !output.N.equals(oldOutput.N)) {
	      changes.addLast (cells[x][y-1]);
	      // System.out.println ("added " + x + " " + (y-1));
	  }
	  if (legalLocation(x,y+1) && !output.S.equals(oldOutput.S)) {
	      changes.addLast (cells[x][y+1]);
	      // System.out.println ("added " + x + " " + (y+1));
	  }
	  if (legalLocation(x-1,y) && !output.W.equals(oldOutput.W)) {
	      changes.addLast (cells[x-1][y]);
	      // System.out.println ("added " + (x-1) + " " + y);
	  }
	  if (legalLocation(x+1,y) && !output.E.equals(oldOutput.E)) {
	      changes.addLast (cells[x+1][y]);
	      // System.out.println ("added " + (x+1) + " " + y);
	  }
	  cell.setOutput(output);
	}

	// Each cell should "glow" with the colors representing the sum
	// of it's input illuminations
	float maxStrength = (float)0.0;
	/*
	for (int y = 0; y < gridSize; ++y)
	    for (int x = 0; x < gridSize; ++x) {
		Illumination input = new Illumination
		    (getNeighborIllumination(x, y-1).S, 
		     getNeighborIllumination(x, y+1).N, 
		     getNeighborIllumination(x-1, y).E, 
		     getNeighborIllumination(x+1, y).W);
		if (input.totalRed() > maxStrength)
		    maxStrength = input.totalRed();
		if (input.totalGreen() > maxStrength)
		    maxStrength = input.totalGreen();
		if (input.totalBlue() > maxStrength)
		    maxStrength = input.totalBlue();
	    }
	*/
	maxStrength = (float)1.0;
	for (int y = 0; y < gridSize; ++y)
	    for (int x = 0; x < gridSize; ++x) {
		Illumination input = new Illumination
		    (getNeighborIllumination(x, y-1).S, 
		     getNeighborIllumination(x, y+1).N, 
		     getNeighborIllumination(x-1, y).E, 
		     getNeighborIllumination(x+1, y).W);
		float r = input.totalRed()/maxStrength;
		float g = input.totalGreen()/maxStrength;
		float b = input.totalBlue()/maxStrength;
		r = (r < 1.0) ? r: (float)1.0;
		g = (g < 1.0) ? g: (float)1.0;
		b = (b < 1.0) ? b: (float)1.0;
		java.awt.Color c = new java.awt.Color(r,g,b);
		cells[x][y].setBackground(c);
	    }

	repaint();
    }
    
    public static void main(String[] args) {
	JFrame mainWin = new JFrame();
	LightBeams lb = new LightBeams();
	lb.init();
	
	mainWin.getContentPane().setLayout (new BorderLayout());
	mainWin.getContentPane().add (lb, "Center");
	
	mainWin.addWindowListener(new WindowAdapter() {
	    public void windowClosing(WindowEvent e) {
		System.exit (0);
	    }
	});

	mainWin.pack();
	mainWin.setVisible(true);

    }



    private boolean legalLocation(int x, int y)
    {
	return (x >= 0) && (x < gridSize) && (y >= 0) && (y < gridSize);
    }

    private Illumination getNeighborIllumination(int x, int y)
    {
	if (legalLocation(x, y))
	    return cells[x][y].getOutput();
	else
	    return new Illumination ();
    }
    
}
