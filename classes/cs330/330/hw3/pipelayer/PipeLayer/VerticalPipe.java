package PipeLayer;

/**
 * Represents a vertical pipe connecting Left and Right.
 */
public class VerticalPipe extends Pipe
{

  /**
   *  Default pipe initialization.
   *  Pipes are initially empty.
   */
  public VerticalPipe()
  {
      super();
  }

  /**
   * Is the pipe open at the indicated end?
   *
   *  @param direction  Which end are we asking about?
   *  @return True if liquid can enter or leave a pipe from the indicated
   *            direction.
   */
  public boolean isOpenAtThisEnd (int direction)
  {
    return (direction == Up) || (direction == Down);
  }

  /**
   * From what direction will the liquid emerge?
   *
   *  @param entryDirection The direction from which liquid is entering
   *           the pipe.
   *  @return The direction at which the liquid will emerge.
   */
  public int emergeDirection (int entryDirection)
  //pre: isOpenAtThisEnd(entryDirection)
  {
    if (entryDirection == Up)
      return Down;
    else
      return Up;
  }


  

  /**
   * Make a copy of the current pipe.
   */
  public Object clone() 
  {
      Pipe p = new VerticalPipe();
      if (isFilled())
	  p.fill();
      return p;
  }
  
}
