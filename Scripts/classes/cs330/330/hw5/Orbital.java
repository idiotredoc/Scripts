import javax.swing.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.*;
import java.awt.*;
import java.lang.*;

/**
   An applet/application drawing an orbital fractal,
   based on the Quadrup Two fractal at 
   http://www.daa.com.au/~james/fractals/orbit/quadruptwo.html
*/

public class Orbital extends JApplet 
{
    
    // The Model
    
    /** the color used for the first dots drawn (shades gradually into 
	<code>color2</code> as more dots are printed */
    private Color color1 = Color.red;
    
    /** the color used for the last dots drawn (shades gradually from 
	<code>color1</code> as more dots are printed */
    private Color color2 = Color.blue;
    
    /** one of the parameters governing the fractal shape */
    private int a = 34;
    /** one of the parameters governing the fractal shape */
    private int b = 1;
    /** one of the parameters governing the fractal shape */
    private int c = 5;
    
    /** The number of dots to be plotted is 1000 times this value. */
    private int nDots = 65; 
    
    
    
    // The View & Controls
    
    /** the graphics area in which the fractal shape is drawn */
    private JPanel canvas;
    
    /** the menu bar under the fractal */
    private JPanel optionspanel;

    /** an Image which will hold the fractal shape */
    private Image buffer = null;
    
    
    /** button to bring up color selection dialog for <code>color1</code>
        @see #color1
    */
    private JButton colorChooser1;
    
    /** button to bring up color selection dialog for <code>color2</code> 
        @see #color2
    */
    private JButton colorChooser2;
    
    /** Labelled integer input area for fractal parameter a */
    private IntegerInput aIn;
    /** Labelled integer input area for fractal parameter b */
    private IntegerInput bIn;
    /** Labelled integer input area for fractal parameter c */
    private IntegerInput cIn;
    
    /** Labelled integer input area for fractal parameter nDots
        @see #nDots
    */
    private IntegerInput nDotsIn;
    
    
    
    /** Saved value of "this: - useful in nested classes
     */
    private JApplet self;
   
    
    /**
       Create the orbital applet
    */
    public Orbital() 
    {
	self = this;
    }
    
    /** 
	Build the GUI components for this applet.
    */
    public void init() {
	// set up the components
	
	optionspanel = new JPanel (); // For the menu bar under the orbital
	
        canvas = new JPanel () {
		// Rather than draw detailed graphics  directly to the 
		// screen, this canvas draws the contents of an Image
		// named "buffer" - think of it as an in-memory
		// representation of the screen. The detailed graphics
		// are drawn into the buffer, which may take a great 
		// deal of time, but the canvas can then show the current
		// state of the buffer anytime it gets the paint() command.
		public void paint (Graphics g) {
		    if (buffer != null)
			g.drawImage(buffer,0,0,this);
		}
	    };
	canvas.setBackground(Color.white);
	canvas.setPreferredSize(new Dimension(400,400));
	getContentPane().add (canvas, BorderLayout.CENTER);
	getContentPane().add (optionspanel, BorderLayout.SOUTH);
	
	colorChooser1 = new JButton("Color 1");
	colorChooser1.setForeground(color1);
	colorChooser1.addActionListener 
	    (new ActionListener() {
		    public void actionPerformed(ActionEvent e) {
			try {
			    color1 = 
				JColorChooser.showDialog(self, "Color 1", color1); 
			} catch (Exception ex) {}
			colorChooser1.setForeground(color1);
			redrawFractal();
		    }
		}
	     );
	
	aIn = new IntegerInput ("a:", a, 3);
	aIn.addActionListener (new ActionListener()
	    {
		public void actionPerformed(ActionEvent e) {
		    int a0 = a;
		    try {
			a = aIn.getInt();
			if (a != a0) redrawFractal();
		    } catch (Exception ex) {};
		}
	    });
	
	//** Add code to create the remaining controls and to display them
	//** in the frame. (Probably want to replace the two calls below.)
	
	colorChooser2 = new JButton("Color 2");
        colorChooser2.setForeground(color2);
        colorChooser2.addActionListener
            (new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        try {
                            color2 =
                                JColorChooser.showDialog(self, "Color 2", color2);
                        } catch (Exception ex) {}
                        colorChooser1.setForeground(color2);
                        redrawFractal();
                    }
                }
             );

	bIn = new IntegerInput ("b:", b, 3);
        bIn.addActionListener (new ActionListener()
            {
                public void actionPerformed(ActionEvent e) {
                    int b0 = b;
                    try {
                        b = bIn.getInt();      
                        if (b != b0) redrawFractal();
                    } catch (Exception ex) {};
                }
            });

	cIn = new IntegerInput ("c:", c, 3);
        cIn.addActionListener (new ActionListener()
            {
                public void actionPerformed(ActionEvent e) {
                    int c0 = c;
                    try {
                        c = cIn.getInt();      
                        if (c != c0) redrawFractal();
                    } catch (Exception ex) {};
                }
            });

	nDotsIn = new IntegerInput ("# dots:",nDots ,3);
	nDotsIn.addActionListener (new ActionListener()
            {
                public void actionPerformed(ActionEvent e) {
                    int n0 = nDots;
                    try {
                        nDots = nDotsIn.getInt();
                        if (nDots != n0) redrawFractal();
                    } catch (Exception ex) {};
                }
            });

	optionspanel.add(colorChooser2);
        optionspanel.add(colorChooser1);
	optionspanel.add(aIn);
	optionspanel.add(bIn);
	optionspanel.add(cIn);
	optionspanel.add(nDotsIn);
	validate();
    }
    
    
    /**
       Start the applet when the web page is loaded
    */
    public void start() {
	redrawFractal();
    }
    
    /**
       Invoked when the web page is exited
    */
    public void stop() {
    }
    
    /**
       Invoked when the applet is no longer needed
    */
    public void destroy() {
    }
    
    
    
    /**
       Interpolate between two extremal integer values
       @param x value to return when i==0
       @param y value to reutrn when i==steps-1
       @param steps number of steps to take between x and y
       @param i desired step number
       @return a value that is i/(steps-1) between x and y
    */
    private
	int interpolate (int x, int y, int i, int steps)
    {
	return (i * y + (steps-i)*x) / steps;
    }
    
    
    /**
       Interpolate between two extremal color values
       @param c1 value to return when i==0
       @param c2 value to reutrn when i==steps-1
       @param steps number of steps to take between c1 and c2
       @param i desired step number
       @return a value that is i/(steps-1) between c1 and c2
    */
    private
	Color interpolate(Color c1, Color c2, int i, int steps)
    {
	return new Color
	    (interpolate(c1.getRed(), c2.getRed(), i, steps),
	     interpolate(c1.getGreen(), c2.getGreen(), i, steps),
	     interpolate(c1.getBlue(), c2.getBlue(), i, steps));
    }
    
    
    
    /**
       Draw a fractal shape
       @param g graphics context/device onto which to draw
       @param d size of the area in which to draw
    */
    private class drawFractal implements Runnable
    {
	Graphics g;
	Dimension d;
	
	public drawFractal(Graphics g1, Dimension d1)
	{
	g = g1;
	d = d1;
	}

	public void run() 
	{
		double x = 0.0;
		double y = 0.0;
		int dmin = (d.width < d.height) ? d.width : d.height;
	
		double xmin = 0.0;
		double xmax = 0.0;
		double ymin = 0.0;
		double ymax = 0.0;
	
		for (int dot = 0; dot < nDots; ++dot) {
		    for (int dot2 = 0; dot2 < 1000; ++dot2) {
			double x2 = nextX(x, y);
			double y2 = nextY(x, y);
			xmin = (x2 < xmin) ? x2 : xmin;
			xmax = (x2 > xmax) ? x2 : xmax;
			ymin = (y2 < ymin) ? y2 : ymin;
			ymax = (y2 > ymax) ? y2 : ymax;
			x = x2;
			y = y2;
		 	}
		    Thread.yield();
		}
	
    		double dx = ((double) (d.width)) / (xmax - xmin);
		double dy = ((double) (d.height)) / (ymax - ymin);
	
		
		x = 0.0;
		y = 0.0;
		for (int dot = 0; dot < nDots; ++dot) {
		    g.setColor (interpolate(color1, color2, dot, nDots));
		    for (int dot2 = 0; dot2 < 1000; ++dot2) {
			double x2 = nextX(x, y);
			double y2 = nextY(x, y);
			int dotx = (int) (dx * (x2 - xmin));
			int doty = (int) (dy * (y2 - ymin));
			g.drawLine (dotx, doty, dotx, doty);
			x = x2;
			y = y2;
	    		}
	    	canvas.repaint();
	    	Thread.yield();
	        }
	
    	}
    }
    
    /**
       Run as a standalone application, opening a window cotaining
       the fractal applet (as a panel).
       @param args ignored
    */
    static public void main (String args[]) 
    {
	Orbital panel = new Orbital();
	panel.init();
	
	JFrame theWindow = new JFrame();
	theWindow.getContentPane().add (panel);
	theWindow.addWindowListener
	    (new WindowAdapter() {
		    public void windowClosing(WindowEvent e) {
			System.exit(0);
		    }
		});
	theWindow.setTitle ("CS330, Spring 2011, Orbital GUI");
	theWindow.pack();
	theWindow.setVisible(true);
	panel.start();
    }
    
    
    /**
       Return the frame/window containing this applet/panel. (Used
       when launching dialog boxes)
       @return the enclosing frame/window
    */
    private JFrame getFrame ()
    {
	java.awt.Container c = this;
	while (c != null && !(c instanceof JFrame)) {
	    c = c.getParent();
	}
	return (JFrame)c;
    }
    
    /** One of the two functions that actually define the fractal shape.
	After plotting a point at (x,y), we compute the location of the
	next dot as (nextX(x,y), nextY(x,y))
	@param x x-coordinate of previously-plotted point
	@param y y-coordinate of previously-plotted point
	@return  x-coordinate of next point
	@see #nextY(double x, double y)
    */
    private double nextX (double x, double y)
    {
	if (x == 0.0) {
	    return y;
	} else {
	    double tmp = Math.log(Math.abs(c * x - b));
	    double offset = Math.sin(Math.log(Math.abs(b * x - c))) *
		Math.atan(tmp * tmp);
	    if (x < 0) 
		return y + offset;
	    else
		return y - offset;
	}
    }
    
    /** One of the two functions that actually define the fractal shape.
	After plotting a point at (x,y), we compute the location of the
	next dot as (nextX(x,y), nextY(x,y))
	@param x x-coordinate of previously-plotted point
	@param y y-coordinate of previously-plotted point
	@return  y-coordinate of next point
	@see #nextX(double x, double y)
    */
    private double nextY (double x, double y)
    {
	return a - x;
    }
    
    /** Force a redraw of the fractal shape (invoked after any of 
	the parameters have been changed */
    private void redrawFractal()
    {
	buffer = createImage(canvas.getSize().width,
			     canvas.getSize().height);
	Graphics gbuffer = buffer.getGraphics();
	Thread fractal = new Thread(new drawFractal(gbuffer, canvas.getSize()));
	fractal.start();
	canvas.repaint();
    }
    
    
}
