package LightBeams;


/**
 *  An East Splitter block splits East to North/South, blocks West.
 */
public class EastSplitterBlock extends LightBlocks
{
    /**
     *  Identify what kind of block this is (for display purposes).
     */
    public String getName()
    {
	return "<";
    }
    

    /**
     *  Indicate what color that name should be displayed in.
     */
    public java.awt.Color getNameColor()
    {
	return java.awt.Color.black;
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
        result.W = new LightColor();
	result.N = input.E.scale(.5f);
	result.S = input.E.scale(.5f);
	result.E = new LightColor((input.N.getRed()+input.S.getRed()),(input.N.getGreen()+input.S.getGreen()),(input.N.getBlue()+input.S.getBlue()));
	return result;
    }
    
    /**
     * The category is a simple text string used to group related 
     * blocks together when presenting choices in the GUI
     */
    public String getCategory() {return "Splitters";}
}
