package LightBeams;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;



/**
 *  A single cell in the LightBeams GUI
 *
 */
public class LightCells extends JPanel
{
    private LightBlocks block;
    private Illumination output;
    private Location location;
    private ActionListener action;

    public LightCells (Location loc, LightBlocks initialBlock, MouseListener action)
    {
	final int prefSize = 36;
	setPreferredSize(new Dimension(prefSize, prefSize));
	setBorder (BorderFactory.createLineBorder(Color.lightGray));


	block = initialBlock;
	output = new Illumination(Color.black, Color.black,
				  Color.black, Color.black);
	setBackground(Color.darkGray);
	location = loc;

	addMouseListener(action);
    }



    public void setBlock (LightBlocks lb)
    {
	block = lb;
	repaint();
    }	


    public LightBlocks getBlock ()
    {
	return block;
    }


    public Illumination getOutput()
    {
	return output;
    }


    public void setOutput(Illumination out)
    {
	output = out;
    }


    public Location getCellLocation()
    {
	return location;
    }


    public void paintComponent(Graphics g) {
        super.paintComponent(g); //paint background

	Font font = new Font("SansSerif", Font.PLAIN, 14);
	FontMetrics metrics = getFontMetrics(font);
	int x =  getWidth()/2 - metrics.stringWidth(block.getName()) / 2;
	int y =  getHeight()/2 + metrics.getAscent() / 2;
	g.setColor (block.getNameColor());
	g.drawString (block.getName(), x, y);
    }

}
