using System.Text.RegularExpressions;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

namespace HamsterPoweredReflection;

[JsonConverter(typeof(StringEnumConverter))]
public enum Access {
    Public,
    Internal,
    Protected,
    Private
}

public class MetaClass
{
    public List<MetaFunction> Functions = new List<MetaFunction>();
    public List<MetaVar> Vars = new List<MetaVar>();
    public List<string> ParentClasses = new List<string>();

    public string HeaderPath;
    
    public string Name { get; set; }
    public bool IsAbstract { get; set; }

    public bool Unpack(string headerPath, string headerSource)
    {
        HeaderPath = headerPath.Replace("\\", "/");
        HeaderPath = HeaderPath.Split("Source/")[1];

        var currentAccess = Access.Private;
        
        // Get name of class. Ensure there is no semicolon at the end of the line. Don't look for the first {, as it may be on the next line. 
        var nameMatch = Regex.Match(headerSource, @"\bclass\s+(\w+)\b(?!\s*;)");
        if (!nameMatch.Success)
        {
            return false;
        }
        
        Name = nameMatch.Groups[1].Value;
        
        // Get all parent classes, capture the whole line
        var parentMatch = Regex.Match(headerSource, @"class\s+\w+\s*:\s*(.*)");
        if (parentMatch.Success)
        {
            var tail = parentMatch.Value.Split(":", 2)[1];
            var parents = tail.Split(",");
            foreach (var parent in parents)
            {
                var classname = parent.Replace("public", "").Replace("protected", "").Replace("private", "").Trim();
                ParentClasses.Add(classname);
            }
            
        }
        

        //Separate header file into lines
        var lines = headerSource.Split(new[] { Environment.NewLine }, StringSplitOptions.RemoveEmptyEntries);
        
        
        // Iterate through the lines and find variables
        foreach (var line in lines)
        {
            // Check for access modifiers
            if (line.Contains("public:"))
            {
                currentAccess = Access.Public;
            }
            else if (line.Contains("protected:"))
            {
                currentAccess = Access.Protected;
            }
            else if (line.Contains("private:"))
            {
                currentAccess = Access.Private;
            }
            else if (line.Contains("internal:"))
            {
                currentAccess = Access.Internal;
            }
            
            // Check for variables. Any variable will have the SERIALIZEDVAR macro in it

            
            if (line.Contains("SERIALIZEDVAR"))
            {
                var pureline = line.Replace("SERIALIZEDVAR", "");
                var split = pureline.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                var varType = split[0];
                var varName = split[1].Replace(";", "");
                var isStatic = pureline.Contains("static");
                var isConst = pureline.Contains("const");
                
                var metavar = new MetaVar();
                metavar.Name = varName;
                metavar.Type = varType;
                metavar.Access = currentAccess;


                if (line.Contains("="))
                {
                    var value = pureline.Split(new[] { '=' }, StringSplitOptions.RemoveEmptyEntries)[1];
                    value = value.Replace(";", "");
                    metavar.DefaultValue = value.Trim();
                    
                }

                Vars.Add(metavar);
            }
            else if (line.Contains("SERIALIZEDFUNCTION"))
            {
                var pureline = line.Replace("SERIALIZEDFUNCTION", "");
                var split = pureline.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                var returnType = split[0];
                var functionName = split[1].Split("(")[0].Replace(";", "");
                var isStatic = pureline.Contains("static");

                var metafunc = new MetaFunction();
                metafunc.Name = functionName;
                metafunc.ReturnType = returnType;
                metafunc.Access = currentAccess;
                metafunc.IsStatic = isStatic;

                
                // Get the parameters
                var paramMatch = Regex.Match(pureline, @"\((.*)\)");
                if (paramMatch.Success)
                {
                    var paramStr = paramMatch.Groups[1].Value;
                    var paramSplit = paramStr.Split(new[] { ',' }, StringSplitOptions.RemoveEmptyEntries);
                    foreach (var param in paramSplit)
                    {
                        var paramSplit2 = param.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                        string paramName;
                        string paramType;
                        if (paramSplit2.Contains("const"))
                        {
                            paramType = paramSplit2[0] + " " + paramSplit2[1];
                            paramName = paramSplit2[2];
                        }
                        else
                        {
                            paramType = paramSplit2[0];
                            paramName = paramSplit2[1];
                        }
                        
                        var paramMeta = new MetaParameter();
                        paramMeta.Name = paramName;
                        paramMeta.Type = paramType;
                        
                        // Check for default value
                        if (param.Contains("="))
                        {
                            var value = param.Split(new[] { '=' }, StringSplitOptions.RemoveEmptyEntries)[1];
                            paramMeta.DefaultValue = value;
                        }
                        
                        
                        metafunc.Parameters.Add(paramMeta);

                    }
                }
                
                Functions.Add(metafunc);
            }
        }
        return true;
    }
}