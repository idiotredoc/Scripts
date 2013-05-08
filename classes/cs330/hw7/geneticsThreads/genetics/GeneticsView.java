package genetics;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JColorChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;


public class GeneticsView extends JPanel {

    private GeneticsModel theModel;

    private Dimension modelSize;
    private final int granularity = 4;  // number of pixels per cell

    private JFrame theWindow;
    private int generationCount;
    private int generationMax;

    private JLabel generationDisplay;
    private JPanel canvas;

    private Color fillColor;
    private Point mouseClickedAt;
    private Point mouseDraggedTo;
    private Point mouseAt;

    public GeneticsView(JFrame win)
    {
	generationMax = 10;
	modelSize = null;

	theWindow = win;
	theModel = new GeneticsModel(new Dimension(16,16));

	setLayout(new BorderLayout());

	JPanel modelControls = new JPanel();
	add (modelControls, BorderLayout.NORTH);

	JPanel modelStatus = new JPanel();
	add (modelStatus, BorderLayout.SOUTH);

	generationDisplay = new JLabel("                              ");
	generationDisplay.setBackground(Color.white);
	setGenerationCount(0);
	modelStatus.add (generationDisplay);

	JCheckBox attract = new JCheckBox("Opposites Attract",
					  theModel.getOppositesAttract());
	attract.addItemListener (new ItemListener() {
		public void itemStateChanged(ItemEvent e) {
		    JCheckBox attr = (JCheckBox)e.getSource();
		    theModel.setOppositesAttract (attr.isSelected());
		}
	    });
	modelControls.add (attract);


	JLabel  fl = new JLabel("Fertility:");
	fl.setBackground(Color.white);
	JTextField fIn = new JTextField(""+theModel.getFertility(), 4);
	fIn.setBackground(Color.white);
	fIn.addActionListener (new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    try {
			JTextField field = (JTextField)e.getSource();
			Integer val = new Integer(field.getText());
			theModel.setFertility (val.intValue());
		    } catch (Exception ex) {}
		}
	    });
	modelControls.add (fl);
	modelControls.add (fIn);


	JLabel  mul = new JLabel("Mutation Interval:");
	mul.setBackground(Color.lightGray);
	JTextField muIn = new JTextField(""+theModel.getMutationInterval(), 5);
	muIn.setBackground(Color.lightGray);
	muIn.addActionListener (new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    try {
			JTextField field = (JTextField)e.getSource();
			Integer val = new Integer(field.getText());
			theModel.setMutationInterval (val.intValue());
		    } catch (Exception ex) {}
		}
	    });
	modelControls.add (mul);
	modelControls.add (muIn);

	

	JButton cButton = new JButton("Color");
	fillColor = Color.yellow;
	cButton.setBackground(fillColor);
	cButton.addActionListener (new ActionListener() {
		public void actionPerformed(ActionEvent e) {
		    try {
			fillColor = JColorChooser.showDialog
			    (theWindow,
			     "Choose Fill Color",
			     fillColor);
			JButton b = (JButton)e.getSource();
			b.setBackground(fillColor);
		    } catch (Exception ex) {}
		}
	    });
	modelControls.add (cButton);




	canvas = new JPanel() {
		public void paint (Graphics g)
		{
		    Image buffer = createImage(canvas.getSize().width,
					       canvas.getSize().height);
		    Graphics gbuffer = buffer.getGraphics();
		    paintCanvas(gbuffer);
		    g.drawImage(buffer,0,0,this);
		    
		    // Draw a "rubber box" if the mouse is being dragged
		    if (mouseClickedAt != null && mouseDraggedTo != null) {
			g.setXORMode(Color.white);
			g.drawRect
			    (Math.min(mouseClickedAt.x, mouseDraggedTo.x),
			     Math.min(mouseClickedAt.y, mouseDraggedTo.y),
			     Math.abs(mouseClickedAt.x - mouseDraggedTo.x),
			     Math.abs(mouseClickedAt.y - mouseDraggedTo.y));
			mouseDraggedTo = null;
			g.setPaintMode();
		    }
		    if (mouseClickedAt != null && mouseAt != null) {
			g.setXORMode(Color.white);
			g.drawRect (Math.min(mouseClickedAt.x,mouseAt.x),
				    Math.min(mouseClickedAt.y,mouseAt.y),
				    Math.abs(mouseClickedAt.x - mouseAt.x),
				    Math.abs(mouseClickedAt.y - mouseAt.y));
			mouseDraggedTo = mouseAt;
			mouseAt = null;
			g.setPaintMode();
			canvas.repaint();
		    }
		}

		public void update (Graphics g)
		{
		    paint (g);
		}
      };
	canvas.setBackground(Color.black);
	
	canvas.addMouseListener (new MouseAdapter() {
		public void mousePressed (MouseEvent e) {
		    mouseClickedAt = e.getPoint();
		}
		public void mouseReleased (MouseEvent e) {
		    Point p = e.getPoint();
		    if (mouseClickedAt != null
			&& (mouseClickedAt.x != p.x
			    || mouseClickedAt.y != p.y)
			) {
			fillWithColor (mouseClickedAt, p, fillColor);
			canvas.repaint();
		    }
		}
	    });
    
	canvas.addMouseMotionListener (new MouseMotionAdapter() {
		public void mouseDragged (MouseEvent e) {
		    mouseAt = e.getPoint();
		    canvas.repaint();
		}
	    });
	
	canvas.addComponentListener (new ComponentAdapter() {
		public void componentShown(ComponentEvent e) {
		    componentResized(e);
		}
		
		public void componentResized(ComponentEvent e) {
		    if (modelSize == null || modelSize != canvas.getSize()) {
			modelSize = canvas.getSize();
			modelSize.width /= granularity;
			modelSize.height /= granularity;
			reset();
		    }
		}
	    });
	
	add (canvas, BorderLayout.CENTER);
	
    }
    

    public void reset()
    {
	theModel.setSize(modelSize);
	setGenerationCount(0);
	canvas.repaint();
    }
    
    public  void setMaxGenerations(final int newMax)
    {
	    Thread t = new Thread(new Runnable() {           
            public void run() { 
		generationMax = newMax;
	        setGenerationCount(generationCount);
	        while (generationCount < generationMax)
        	    nextGeneration();
	 }
        });
        t.start();
    }
    
    public  void setGenerations(final int newGen)
    {
		setGenerationCount(newGen);
        	while (generationCount < generationMax)
            	nextGeneration();
    }
    

    public  void nextGeneration ()
    {
	theModel.nextGeneration();
	setGenerationCount(generationCount+1);
	canvas.repaint();
    }
    
    
    private void setGenerationCount (final int c)
    {
		 generationCount = c;
        generationDisplay.setText("Generation: " + generationCount
                                  + " / " + generationMax + "          ");
    }
   
	
 
    private void paintCanvas (Graphics g)
    {
	Dimension canvasSize = canvas.getSize();
	
	for (int x = 0; x < canvasSize.width; ++x)
	    for (int y = 0; y < canvasSize.height; ++y) {
		Color c = theModel.getCell(x,y);
		g.setColor(c);
		g.fillRect (x*granularity, y*granularity, granularity, granularity);
	    }
    }
    
    
    
    private void fillWithColor (Point p1, Point p2, Color c)
    {
	System.err.println ("filling " + p1 + " to " + p2);
	theModel.fillRegion ((p1.x + granularity/2)/granularity,
			     (p1.y + granularity/2)/granularity,
			     (p2.x + granularity/2)/granularity,
			     (p2.y + granularity/2)/granularity, c);
    }
    
}
