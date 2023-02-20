namespace HamsterPoweredReflection;

public class CodeGenerator
{
    List<MetaClass> classes = new List<MetaClass>();
    
    public CodeGenerator(List<MetaClass> classes)
    {
        this.classes = classes;
    }
    
    public bool GenerateSerializationCode(string sourceDir)
    {
        Parallel.ForEach(classes, metaClass =>
        {
            // find the class header file
            string headerFile = sourceDir + metaClass.HeaderPath.Split(".")[0] + ".generated.h";
            var file = File.CreateText(headerFile);

            // write the header
            file.WriteLine("#define GENERATED() \\");
            file.WriteLine("public: \\");

            file.WriteLine("void Deserialize(nlohmann::json& j) override" + "{ \\");
            foreach (var parent in metaClass.ParentClasses)
            {
                file.WriteLine("\t" + parent + "::Deserialize(j); \\");
            }

            foreach (var variable in metaClass.Vars)
            {
                file.WriteLine("\t" + variable.Name + " = j[\"" + variable.Name + "\"]; \\");
            }


            file.WriteLine("} \\");

            //SERIALIZE
            file.WriteLine("void Serialize(nlohmann::json& j) override" + "{ \\");
            foreach (var parent in metaClass.ParentClasses)
            {
                file.WriteLine("\t" + parent + "::Serialize(j); \\");
            }

            file.WriteLine("\tj[\"Type\"] = \"" + metaClass.Name + "\"; \\");

            foreach (var variable in metaClass.Vars)
            {
                file.WriteLine("\t" + "j[\"" + variable.Name + "\"] = " + variable.Name + "; \\");
            }

            file.WriteLine("} ");


            file.Close();

        });
        

        return true;
    }
}