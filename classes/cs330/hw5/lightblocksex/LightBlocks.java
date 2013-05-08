package LightBeams;

/**
 *  A LightBlock is a kind of building block that reacts to light
 *  shining upon each of its 4 sides.
 *  Some LightBlocks are simply transparent, others opaque, others emit
 *  light, and still others reflect light in various ways.
 */
abstract public class LightBlocks implements Cloneable
{
    /**
     *  Identify what kind of block this is (for display purposes).
     */
    abstract public String getName();

    /**
     *  Indicate what color that name should be displayed in.
     */
    abstract public java.awt.Color getNameColor();

    /**
     *  Indicate how this block reacts when illuminated.
     *  "input" shows whether light is shining on each of the 4 sides
     *     of the block. (Note that a block may be subjected to input
     *     light from all 4 different dies simultaneously.
     *  The return value must indicate which sides of the block light
     *     is shining from.
     */
    abstract public
	Illumination illuminate (Illumination input);

    /**
     * The category is a simple text string used to group related 
     * blocks together when presenting choices in the GUI
     */
    abstract public String getCategory();
}
