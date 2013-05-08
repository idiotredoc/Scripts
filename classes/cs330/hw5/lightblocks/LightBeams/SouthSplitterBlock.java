package LightBeams;


/**
 *  An South Splitter block splits South to West/East, blocks North.
 */
public class SouthSplitterBlock extends LightBlocks
{
    /**
     *  Identify what kind of block this is (for display purposes).
     */
    public String getName()
    {
	return "v";
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
        result.N = new LightColor();
	result.W = input.S.scale(.5f);
	result.E = input.S.scale(.5f);
	result.S = new LightColor((input.W.getRed()+input.E.getRed()),(input.W.getGreen()+input.E.getGreen()),(input.W.getBlue()+input.E.getBlue()));
	return result;
    }
    
    /**
     * The category is a simple text string used to group related 
     * blocks together when presenting choices in the GUI
     */
    public String getCategory() {return "Splitters";}
}