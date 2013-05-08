package genetics;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Genetics extends JFrame
{   
    private JLabel generations;
    private JPanel inputs;
    private GeneticsView viewer;
    private JApplet applet;
    private JTextField textField;
    private JButton goButton;
    private JButton stopButton;
    private JButton resetButton;
    public Genetics(JApplet inApplet)
    {
	applet = inApplet;
	
	getContentPane().setLayout (new BorderLayout());

	viewer = new GeneticsView(this);
	getContentPane().add(viewer, BorderLayout.CENTER);	

	inputs = new JPanel();
	getContentPane().add(inputs, BorderLayout.SOUTH);
	
	generations = new JLabel("Generations:");
	inputs.add(generations);

	textField = new JTextField(5);
	textField.setActionCommand("setgenerations");
	inputs.add(textField);	
	textField.addActionListener(new ActionListener()
	{
	    public void actionPerformed(ActionEvent e)
	    {
		try
		{
			JTextField field = (JTextField)e.getSource();
			Integer num = new Integer(field.getText());
	        	viewer.setMaxGenerations(num.intValue());
			viewer.setGenerations(0);
	    	} catch (Exception ex) {} 
	    }
	});

	goButton = new JButton("Go");
	goButton.setActionCommand("Go");
	goButton.addActionListener(new ActionListener()
	{
            public void actionPerformed(ActionEvent e)
            {
                try
                {      
			Integer num = new Integer(textField.getText());
			viewer.setGenerations(0);
			viewer.setMaxGenerations(num.intValue());
                } catch (Exception ex) {}
            }
        });
	inputs.add(goButton);

	stopButton = new JButton("Stop");
        stopButton.setActionCommand("Stop");
        stopButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                try
                {
                        viewer.setMaxGenerations(0);
                } catch (Exception ex) {}
            }
        });
	inputs.add(stopButton);

	resetButton = new JButton("Reset");
        resetButton.setActionCommand("Reset");
  	resetButton.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                try
                {
                        viewer.setMaxGenerations(0);
			viewer.reset();
                } catch (Exception ex) {}
            }
        });
	inputs.add(resetButton);

	/** Need to add the following components to the content pane
            - The viewer
	    - JTextField for entering # of generations
	       Changes to this should set maxGenerations in the 
               viewer accordingly
	    - Go button: pressing this should setGenerations in the viewer to 0
	    - Stop button: pressing this should setMaxGenerations in the
	       viewer to 0
	    - Reset button: pressing this should setMaxGenerations in the
	         viewer to 0 and then reset the viewer
	**/

	
	addWindowListener
	    (new WindowAdapter() {
		    public void windowClosing(WindowEvent e) {
			if (applet == null)
			    System.exit(0);
			else
			    dispose();
		    }
		});
    }
    public Dimension getPreferredSize()
    {
	return new Dimension(550, 550);
    }
    

    public static void main(String args[]) {
	Genetics window = new Genetics(null);
	window.setTitle("CS330 GUI Assignment");
	window.pack();
	window.setVisible(true);
    }

}
