
using Newtonsoft.Json;

namespace HamsterPoweredReflection;

public struct SerializedVariable
{
    public SerializedVariable(string type, string name, string value)
    {
        Type = type;
        Name = name;
        Value = value;
    }
    
    public string Name;
    public string Type;
    public string Value;
}



public class HeaderParser
{
    public string SourceDirectory { get; set; }
    public string OutputDirectory { get; set; }
    public HeaderParser(string directory)
    {
        SourceDirectory = directory;
        OutputDirectory = directory + "/Meta/";
        
    }

    public void GenerateSerializationFunction(string headerPath, List<SerializedVariable> variables)
    {
        var newHeaderFile = headerPath.Split(".")[0] + ".reflection" + ".h";
        var className = headerPath.Split(".")[0];
        className = className.Split("\\")[className.Split("\\").Length - 1];
        
        var file = File.CreateText(newHeaderFile);
        file.WriteLine("#pragma once");
        file.WriteLine("#include \"" + className + "\".h");
        
        file.WriteLine("void " + className + "::Deserialize(nlohmann::json& j)" + "{");
        foreach (var variable in variables)
        {
            file.WriteLine("this->" + variable.Name + " = j[\"" + variable.Name + "\"];");
        }
        file.WriteLine("}");
    }
    
    public void Parse(string outputPath)
    {
        OutputDirectory = outputPath;
        List<string> files = GetHeaders(SourceDirectory);
        foreach (string file in files)
        {
            var variables = ParseHeader(file);
            var jsonPath = OutputDirectory + Path.GetFileNameWithoutExtension(file) + ".json";
            var json = JsonConvert.SerializeObject(variables, Formatting.Indented);
            File.WriteAllText(jsonPath, json);
            
            GenerateSerializationFunction(file, variables);
        }
    }
    
    private List<SerializedVariable> ParseHeader(string path)
    {
        Console.WriteLine("Parsing " + path);

        // Check each line for the REFLECT macro
        var lines = File.ReadAllLines(path);     
        var variables = new List<SerializedVariable>();
        foreach (string line in lines)
        {
            if (line.Contains("REFLECT") && !line.Contains("#define"))
            {
                var trimmed = line.Trim();
                trimmed = trimmed.Replace(";", "");
                trimmed = trimmed.Replace("REFLECT", "");
                var split = trimmed.Split(' ');
                var type = split[1];
                var name = split[2];
                
                if (trimmed.Contains("="))
                {
                    var splitval = trimmed.Split('=');
                    var value = splitval[1].Trim();
                    value = value.Split(";")[0];
                    variables.Add(new SerializedVariable(type, name, value));
                }
                else
                {
                    variables.Add(new SerializedVariable(type, name, ""));
                }
            }
        }
        
        return variables;
    }
    
    private List<string> GetHeaders(string path)
    {
        List<string> files = new List<string>();
        foreach (string file in Directory.GetFiles(path, "*.h", SearchOption.AllDirectories))
        {
            files.Add(file);
        }
        return files;
    }
}