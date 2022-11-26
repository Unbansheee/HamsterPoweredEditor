using System.Net.Mime;
using HamsterPoweredReflection;

class Program
{
    static void Main(string[] args)
    {

#pragma warning disable CS8602
        var sourceFolder = Directory.GetParent(System.AppContext.BaseDirectory).Parent.Parent.Parent.Parent + "\\HamsterPoweredEditor\\Source\\";
        var outputFolder = Directory.GetParent(System.AppContext.BaseDirectory).Parent.Parent.Parent.Parent + "\\HamsterPoweredEditor\\Meta\\";
#pragma warning restore CS8602
        
        Console.WriteLine(sourceFolder);

        //print all args
        foreach (var arg in args)
        {
            Console.WriteLine(arg);
        }
        
        if (args.Length == 2)
        {
            sourceFolder = args[0];
            outputFolder = args[1];
        }
        
        
        HeaderParser parser = new HeaderParser(sourceFolder);
        parser.Parse(outputFolder);
    }
    
}