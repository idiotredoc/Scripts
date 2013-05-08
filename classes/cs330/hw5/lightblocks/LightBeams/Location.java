package LightBeams;

public class Location {
    int x;
    int y;

    Location (int xx, int yy)
    {
	x = xx;
	y = yy;
    }

    public String toString()
    {
	return "(" + x + "," + y + ")";
    }
}
