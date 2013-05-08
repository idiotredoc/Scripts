package LightBeams;


/**
 *  A transparent block allows light to pass from one side to the other.
 */
public class GreenFilterBlock extends LightBlocks
{
    /**
     *  Identify what kind of block this is (for display purposes).
     */
    public String getName()
    {
	return "#";
    }
    

    /**
     *  Indicate what color that name should be displayed in.
     */
    public java.awt.Color getNameColor()
    {
	return java.awt.Color.green;
    }


    /**
     *  Indicate how this block reacts when illuminated.
     *  "input" shows whether light is shining on each of the 4 sides
     *     of the block.
     *  The return value must indicate which sides of the block light
     *     is shining from.
     */
    public Illumination illuminate (Illumination input)
    {
	Illumination result = new Illumination();
	final float z = (float)0.0;
	result.N = new LightColor(z, input.S.getGreen(), z);
	result.S = new LightColor(z, input.N.getGreen(), z);
	result.W = new LightColor(z, input.E.getGreen(), z);
	result.E = new LightColor(z, input.W.getGreen(), z);
	return result;
    }
    
    /**
     * The category is a simple text string used to group related 
     * blocks together when presenting choices in the GUI
     */
    public String getCategory() {return "Filters";}
}
