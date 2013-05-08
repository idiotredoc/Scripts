package LightBeams;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;



/**
 *  The main program and window launcher for the LightBeams GUI.
 *
 */
public class RegisteredBlocks
    implements Registerable
{
    private Hashtable<String,Vector<LightBlocks> > categories;
    private LightBlocks selectedBlock;
    private JDialog dialog;
    
    public RegisteredBlocks ()
    {
	categories = new Hashtable<String,Vector<LightBlocks> >();
    }


    /**
       Register a new kind of block within some category
    */
    public void register (LightBlocks lb, String category)
    {
	Vector<LightBlocks> v = categories.get(category);
	if (v == null)
	    {
		v = new Vector<LightBlocks>();
		categories.put (category, v);
	    }
	v.add (lb);
    }




    private class SelectorButton extends JButton {

	private LightBlocks block;

	public SelectorButton (LightBlocks lb)
	{
	    super();
	    block = lb;
	    setText (block.getName());
	    setForeground (block.getNameColor());
	    setBackground (Color.lightGray);
	    addActionListener (new ActionListener () {
		    public void actionPerformed (ActionEvent e)
		    {
			selectedBlock = block;
			dialog.setVisible(false);
		    }
		});
	}
    }


    /** Retrieve any block in the given category */
    LightBlocks getAnyBlock (String category)
    {
	Vector v = (Vector) categories.get(category);
	if (v == null)
	    return null;
	else
	    return (LightBlocks)v.elementAt(0);
    } 
	


    /** Pops up a dialog box allowing a person to select one from among
	each type of registered block. Returns 
	the registered block if a selection is made, or 
	null if Cancel is selected.
    */
    public LightBlocks selectBlock (Component c)
    {
	Container parent = c.getParent();
	while (parent != null && !(parent instanceof Frame))
	    parent = parent.getParent();
	Frame parentFrame = (Frame)parent;

	dialog = new JDialog(parentFrame, "Select a Block", true);
	Container content = dialog.getContentPane();
	content.setLayout (new GridLayout(categories.size(), 1));

	Enumeration catEnum = categories.keys();
	while (catEnum.hasMoreElements()) {
	    String category = (String)(catEnum.nextElement());
	    Vector blocks = (Vector)categories.get(category);
	    JPanel p = new JPanel();
	    p.add(new JLabel(category + ": "));
	    for (int i = 0; i < blocks.size(); ++i) {
		LightBlocks lb = (LightBlocks)blocks.elementAt(i);
		p.add (new SelectorButton(lb));
	    }
	    content.add (p);
	}
	dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
	dialog.pack();
	dialog.setVisible(true);
	return selectedBlock;
    }
}
