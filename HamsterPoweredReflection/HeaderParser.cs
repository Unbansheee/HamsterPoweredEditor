
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


public class HeaderData
{
    public List<SerializedVariable> Variables;
    public string Name;
    public string BaseClass;

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

    public void GenerateDeserializationFunction(string headerPath, HeaderData data)
    {
        List<SerializedVariable> variables = data.Variables;
        string name = data.Name;
        string baseClass = data.BaseClass;
        

        //DESERIALIZE
        var newHeaderFile = headerPath.Split(".")[0] + ".reflection" + ".cpp";
        var className = headerPath.Split(".")[0];
        className = className.Split("\\")[className.Split("\\").Length - 1];
        
        
        
        var file = File.CreateText(newHeaderFile);
        file.WriteLine("#pragma once");
        file.WriteLine("#include \"" + className + ".h\"");
        file.WriteLine("#include \"Core/JSONConversions.h\"\n\n" );
        
        file.WriteLine("void " + className + "::Deserialize(nlohmann::json& j)" + "{");
       // if (baseClass != "")
        {
            file.WriteLine("\t" + "__super::Deserialize(j);");
        }
        
        foreach (var variable in variables)
        {
            file.WriteLine("\t" + variable.Name + " = j[\"" + variable.Name + "\"];");
        }
        
        
        file.WriteLine("}");
        
        //SERIALIZE
        file.WriteLine("\n");
        file.WriteLine("void " + className + "::Serialize(nlohmann::json& j)" + "{");
        //if (baseClass != "")
        {
            file.WriteLine("\t" + "__super::Serialize(j);");
        }
        
        file.WriteLine("\tj[\"Type\"] = \"" + className + "\";");
        
        foreach (var variable in variables)
        {
            file.WriteLine("\t" + "j[\"" + variable.Name + "\"] = " + variable.Name + ";");
        }

        file.WriteLine("}");
        
        file.Close();
        
        //open HamsterPoweredEditor.vcxproj
        //add new file to project
        var srcFolder = Program.SourceFolder;
        var projFolder = Directory.GetParent(srcFolder).Parent.FullName;
        var projectPath = projFolder + "/HamsterPoweredEditor.vcxproj";
        var projectFile = File.ReadAllText(projectPath);
        var relativeHeaderPath = headerPath.Replace(projFolder, "");
        relativeHeaderPath = relativeHeaderPath.Split(".")[0] + ".reflection" + ".cpp";
        relativeHeaderPath = relativeHeaderPath.Substring(1);
        var newFile = "\n\t<ClCompile Include=\"" + relativeHeaderPath + "\" />";
        if (!projectFile.Contains(newFile.Trim()))
        {
            // find the last </ClCompile> tag
            var lastClCompile = projectFile.LastIndexOf(".cpp\" />") + 9;
            var newProjectFile = projectFile.Insert(lastClCompile, newFile);
            File.WriteAllText(projectPath, newProjectFile);
        }
    }
    
    
    public void Parse(string outputPath)
    {
        OutputDirectory = outputPath;
        List<string> files = GetHeaders(SourceDirectory);
        foreach (string file in files)
        {
            
            var data = ParseHeader(file);
            if (data == null) continue;
            var jsonPath = OutputDirectory + Path.GetFileNameWithoutExtension(file) + ".json";
            var json = JsonConvert.SerializeObject(data.Variables, Formatting.Indented);
            File.WriteAllText(jsonPath, json);
            
            if (data.Variables.Count > 0)
            {
                GenerateDeserializationFunction(file, data);
            }

        }
    }
    
    private HeaderData ParseHeader(string path)
    {
        Console.WriteLine("Parsing " + path);
        HeaderData data = new HeaderData();
        // Check each line for the REFLECT macro
        var lines = File.ReadAllLines(path);     
        var variables = new List<SerializedVariable>();
        foreach (string line in lines)
        {
            if (line.Contains("class") && (line.Contains("public") || line.Contains("private")))
            {
                var split = line.Split(" ");
                data.BaseClass = split[2];
                data.Name = split[1];
            }

            
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
        
        data.Variables = variables;
        if (variables.Count > 0) return data;
        return null;

    }
    
    private List<string> GetHeaders(string path)
    {
        List<string> files = new List<string>();
        foreach (string file in Directory.GetFiles(path, "*.h", SearchOption.AllDirectories))
        {
            if (!file.Contains(".reflection")) files.Add(file);
        }
        return files;
    }
}