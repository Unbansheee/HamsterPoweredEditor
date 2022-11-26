using System.Net.Mime;
using HamsterPoweredReflection;

class Program
{
    public static string SourceFolder;
    public static string DestinationFolder;
    static void Main(string[] args)
    {

#pragma warning disable CS8602
        SourceFolder = Directory.GetParent(System.AppContext.BaseDirectory).Parent.Parent.Parent.Parent + "\\HamsterPoweredEditor\\Source\\";
        DestinationFolder = Directory.GetParent(System.AppContext.BaseDirectory).Parent.Parent.Parent.Parent + "\\HamsterPoweredEditor\\Meta\\";
#pragma warning restore CS8602
        
        Console.WriteLine(SourceFolder);

        //print all args
        foreach (var arg in args)
        {
            Console.WriteLine(arg);
        }
        
        if (args.Length == 2)
        {
            SourceFolder = args[0];
            DestinationFolder = args[1];
        }
        
        
        HeaderParser parser = new HeaderParser(SourceFolder);
        parser.Parse(DestinationFolder);
    }
    
}