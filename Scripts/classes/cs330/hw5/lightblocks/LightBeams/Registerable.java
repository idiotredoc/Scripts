package LightBeams;

public interface Registerable
{
    /**
       Before any new kind of block can be used by the game,
       a single instance of that block must be registered.
       The category name is used simly to group blocks when presenting
       a list of choices.
    */
    public void register (LightBlocks block, String category);
}
