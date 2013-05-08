package LightBeams;


/**
 *  LightColor represents the shade and brightness of a light on
 *  an RGB (red/green/blue) scale
 */
public class LightColor
{
    private float red;
    private float green;
    private float blue;

    /** create a black color */
    public LightColor()
    {
	red = green = blue = (float)0.0;
    }
    
    /** create a color with the indicated red/green/blue mix */
    public LightColor(float r, float g, float b)
    {
	red = r;
	green = g;
	blue = b;
    }


    /** create a color equivalent to a Java AWT color */
    public LightColor(java.awt.Color c)
    {
	final float maxC = (float)255.0;
	red = ((float)c.getRed()) / maxC;
	green = ((float)c.getGreen()) / maxC;
	blue = ((float)c.getBlue()) / maxC;
    }



    float getRed() {return red;}
    float getGreen() {return green;}
    float getBlue() {return blue;}


    /** Compute an overall brightness. */
    public float brightness()
    {
	return red + green + blue;
    }

    /** Return a new light in which the overall brightness has been
	adjusted up or down by a fixed amount. */
    public LightColor scale (float scalingFactor)
    {
	return new LightColor(red * scalingFactor,
			      green * scalingFactor,
			      blue * scalingFactor);
    }


    public String toString()
    {
	return "[" + String.format("%.3f", red) + ":" 
	    + String.format("%.3f", green) + ":" 
	    + String.format("%.3f", blue) + "]";
    }


    public boolean equals (Object o)
    {
	if (o instanceof LightColor) {
	    LightColor c = (LightColor)o;
	    return (red == c.red && green == c.green && blue == c.blue);
	} else
	    return false;
    }
}
