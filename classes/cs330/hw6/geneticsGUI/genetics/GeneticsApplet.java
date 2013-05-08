package genetics;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JApplet;
import javax.swing.JButton;


public class GeneticsApplet extends JApplet
{
  
  JApplet self;

  public GeneticsApplet()
  {
    self = this;
  }
  


    
    
  public void init()
  {
    JButton start = new JButton("Start");
    getContentPane().add(start);
    start.addActionListener (new ActionListener () {
        public void actionPerformed (ActionEvent e)
        {
          Genetics g = new Genetics (self);
          g.setTitle("CS330 GUI Asst");
          g.pack();
          g.setVisible(true);
        }
      });
    validate();
  }

  public void start() {}



}
