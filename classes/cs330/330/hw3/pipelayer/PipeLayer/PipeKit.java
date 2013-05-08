package PipeLayer;


import java.util.ArrayList;


/**
 * A collection of one each of all the available kinds of pipes.
 *
 */
public class PipeKit
{
  /**
   * Holds the pipes.
   */
  private ArrayList<Pipe> thePipes;
  

  /**
   * Builds the pipekit, placing one of each of the different kinds of pipes
   * within it.
   */
  public PipeKit()
  {
    // Initialize the vector
    thePipes = new ArrayList<Pipe>();

    // Add pipes
    thePipes.add (new HorizontalPipe());
    thePipes.add (new VerticalPipe());
    thePipes.add (new LeftDownPipe());
    thePipes.add (new LeftUpPipe());
    thePipes.add (new RightUpPipe());
    thePipes.add (new RightDownPipe());
    thePipes.add (new CrossPipe());
  }

  /**
   * How many pipes in this kit?
   *
   */
  public int size()
  {
    return thePipes.size();
  }

  /**
   * Select a pipe from the kit.
   *
   */
  public Pipe getPipe (int index)
  //pre: index >= 0 && index < size()
  {
    return thePipes.get(index);
  }

}
