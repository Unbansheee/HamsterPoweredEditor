using Newtonsoft.Json;

namespace HamsterPoweredReflection;

public class Parser
{
    private List<MetaClass> classes = new List<MetaClass>();

    private string SourceDirectory { get; set; }
    private string OutputDirectory { get; set; }
    

    public Parser(string sourceFolder, string outputFolder)
    {
        SourceDirectory = sourceFolder;
        OutputDirectory = outputFolder;
    }

    public List<MetaClass> BeginBatchParse()
    {
        classes.Clear();

        var files = RetrieveHeaders(SourceDirectory);


        //Parse all files
        foreach(var filePath in files)
        {
            Console.WriteLine("Parsing " + filePath);
            var file = File.ReadAllText(filePath);
            

            if (file.Contains("SERIALIZEDCLASS"))
            {
                var metaClass = new MetaClass();
                if (metaClass.Unpack(filePath, file))
                {
                    classes.Add(metaClass);
                    WriteToJSON(metaClass, OutputDirectory);
                }
            }
        };
        
        
        
        return classes;
    }
    
    public void WriteToJSON(MetaClass _class, string outputDirectory)
    {
        var json = JsonConvert.SerializeObject(_class, Formatting.Indented);
        File.WriteAllText(outputDirectory + _class.Name + ".meta", json);
    }
    
    private List<string> RetrieveHeaders(string path)
    {
        List<string> files = new List<string>();

        foreach (string filePath in Directory.GetFiles(path, "*.h", SearchOption.AllDirectories))
        {
            if (!filePath.Contains(".generated"))
            {
                files.Add(filePath);
            }
        }
        return files;
    }
    




}