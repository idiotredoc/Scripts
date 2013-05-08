package LightBeams;


/**
 *  A simple structure used to record whether light is present
 *  in each of the four directions (North, South, East, West)
 */
public class Illumination
{
    public LightColor N;
    public LightColor S;
    public LightColor E;
    public LightColor W;
    
    /** Create an illumination object with black (no light)
	on all directions. */
    public Illumination()
    {
	N = S = E = W = new LightColor();
    }
    

    /** Create an illumination object with the indicated 4 colors */
    public Illumination(LightColor n, LightColor s,
			LightColor e, LightColor w)
    {
	N = n;
	S = s;
	E = e;
	W = w;
    }


    /** Create an illumination object with the indicated 4 colors */
    public Illumination(java.awt.Color n, java.awt.Color s,
			java.awt.Color e, java.awt.Color w)
    {
	N = new LightColor(n);
	S = new LightColor(s);
	E = new LightColor(e);
	W = new LightColor(w);
    }


    public float totalRed()
    {
	return N.getRed() + S.getRed() + W.getRed() + E.getRed();
    }

    public float totalGreen()
    {
	return N.getGreen() + S.getGreen() + W.getGreen() + E.getGreen();
    }

    public float totalBlue()
    {
	return N.getBlue() + S.getBlue() + W.getBlue() + E.getBlue();
    }


    public String toString()
    {
	return "(N=" + N + ", S=" + S + ", W=" + W + ", E=" + E + ")";
    }

}
