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
        Console.WriteLine(classes.Count + " classes found");
        // sort classes to alphabetical order
        classes.Sort((a, b) => a.Name.CompareTo(b.Name));
        
        
        
        Parallel.ForEach(classes, metaClass =>
        {
            // find the class header file
            string headerFile = sourceDir + metaClass.HeaderPath.Split(".")[0] + ".generated.h";
            var file = File.CreateText(headerFile + ".tmp");

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

            file.WriteLine("} \n");
            //file.WriteLine("#undef GENERATED()");


            file.Close();
            
            // compare the file with the existing one
            if (File.Exists(headerFile))
            {
                string oldFile = File.ReadAllText(headerFile);
                string newFile = File.ReadAllText(headerFile + ".tmp");
                if (oldFile != newFile)
                {
                    File.Delete(headerFile);
                    File.Move(headerFile + ".tmp", headerFile);
                    Console.WriteLine("Codegen Refreshed for: " + metaClass.Name);
                }
                else
                {
                    File.Delete(headerFile + ".tmp");
                    Console.WriteLine("No Codegen update: " + metaClass.Name);
                }
            }
            else
            {
                File.Move(headerFile + ".tmp", headerFile);
                Console.WriteLine("Codegen created for: " + metaClass.Name);
            }
        });

        var spawnableClasses = new List<MetaClass>(classes);
        // sort by category and then name
        spawnableClasses.Sort((a, b) =>
        {
            if (a.Category == b.Category)
            {
                return a.Name.CompareTo(b.Name);
            }

            return a.Category.CompareTo(b.Category);
        });
        
        {
            //open ComponentRegistry.generated.h
            string registryFile = sourceDir + "Core/ComponentRegistry.generated.h";
            var file = File.CreateText(registryFile + ".tmp");

            //write class registry functions to header as a function
            foreach (var include in spawnableClasses)
            {
                file.WriteLine("#include \"" + include.HeaderPath + "\"");
            }

            file.WriteLine("#define GENERATED_REGISTRY() \\");
            file.WriteLine("public: \\");
            file.WriteLine("static void RegisterAll() \\");
            file.WriteLine("{ \\");
            foreach (var metaClass in spawnableClasses)
            {
                file.WriteLine("\tComponentRegistry::RegisterComponent<" + metaClass.Name + ">(\"" + metaClass.Name +
                               "\"); \\");
            }

            file.WriteLine("} \\");


            file.Close();

            // compare the file with the existing one
            if (File.Exists(registryFile))
            {
                string oldFile = File.ReadAllText(registryFile);
                string newFile = File.ReadAllText(registryFile + ".tmp");
                if (oldFile != newFile)
                {
                    File.Delete(registryFile);
                    File.Move(registryFile + ".tmp", registryFile);
                    Console.WriteLine("Codegen Refreshed for: ComponentRegistry");
                }
                else
                {
                    File.Delete(registryFile + ".tmp");
                    Console.WriteLine("No Codegen update: ComponentRegistry");
                }
            }
            else
            {
                File.Move(registryFile + ".tmp", registryFile);
                Console.WriteLine("Codegen created for: ComponentRegistry");
            }
        }


        //Spawn function generator
        string spawnFile = sourceDir + "Actors/ComponentSpawns.generated.h";
        var tempfile = File.CreateText(spawnFile + ".tmp");

        //write class registry functions to header as a function
        foreach (var include in spawnableClasses)
        {
            tempfile.WriteLine("#include \"" + include.HeaderPath + "\"");
        }
        
        tempfile.WriteLine("#define GENERATED_COMPONENTADDS() \\");
        foreach (var metaclass in spawnableClasses)
        {
            // format for spawning:
            // if (type == "NameComponent")
            //{
            //    component = GetComponent<NameComponent>();
            //    if (!component)
            //    {
            //        component = AddComponent<NameComponent>();
            //    }
            //}
            
            
            tempfile.WriteLine("else if (type == \"" + metaclass.Name + "\")\\");
            tempfile.WriteLine("{\\");
            tempfile.WriteLine("\tcomponent = GetComponent<" + metaclass.Name + ">();\\");
            tempfile.WriteLine("\tif (!component)\\");
            tempfile.WriteLine("\t{\\");
            tempfile.WriteLine("\t\tcomponent = AddComponent<" + metaclass.Name + ">();\\");
            tempfile.WriteLine("\t}\\");
            tempfile.WriteLine("}\\");
            
        }
        
        tempfile.WriteLine("\n\n");
        
        tempfile.WriteLine("#define GENERATED_COMPONENTADDSUI() \\");

        // format for spawning:
        //if (ImGui::MenuItem("Mesh"))
        //{
        //    selected->AddComponent<MeshComponent>();
        //    ImGui::CloseCurrentPopup();
        //}
            
        // depending on their file path, we can have submenus specified by
        // ImGui::BeginMenu("Submenu/Submenu2");
        // ImGui::EndMenu();
            
        // the classes category is metaclass.Category and is split by "/"
        
        
        
        
        // we need to keep track of the current category so we can open and close menus. we can use a stack for this
        // no category is defined by "Default" and will not have a submenu
        var categoryStack = new Stack<string>();
        foreach (var metaclass in spawnableClasses)
        {
            // if the category is different, we need to close the menu
            while (categoryStack.Count > 0 && !metaclass.Category.StartsWith(categoryStack.Peek()))
            {
                categoryStack.Pop();
                tempfile.WriteLine("\tImGui::EndMenu(); }\\");
            }
            
            // if the category is different, we need to open the menu
            var category = metaclass.Category.Split('/');
            for (int i = 0; i < category.Length; i++)
            {
                if ((categoryStack.Count <= i || categoryStack.ElementAt(i) != category[i]))
                {
                    categoryStack.Push(category[i]);
                    tempfile.WriteLine("\tif (ImGui::BeginMenu(\"" + category[i] + "\")){\\");
                }
            }
            
            tempfile.WriteLine("\tif (ImGui::MenuItem(\"" + metaclass.Name + "\"))\\");
            tempfile.WriteLine("\t{\\");
            tempfile.WriteLine("\t\tselected->AddComponent<" + metaclass.Name + ">();\\");
            tempfile.WriteLine("\t\tImGui::CloseCurrentPopup();\\");
            tempfile.WriteLine("\t}\\");
        }
        // close all menus
        while (categoryStack.Count > 0)
        {
            categoryStack.Pop();
            tempfile.WriteLine("\tImGui::EndMenu(); }\\");
        }
        //tempfile.WriteLine("\tImGui::EndMenu(); }");
        
        tempfile.Close();
        // compare the file with the existing one
        if (File.Exists(spawnFile))
        {
            string oldFile = File.ReadAllText(spawnFile);
            string newFile = File.ReadAllText(spawnFile + ".tmp");
            if (oldFile != newFile)
            {
                File.Delete(spawnFile);
                File.Move(spawnFile + ".tmp", spawnFile);
                Console.WriteLine("Codegen Refreshed for: SpawnDefinitions");
            }
            else
            {
                File.Delete(spawnFile + ".tmp");
                Console.WriteLine("No Codegen update: SpawnDefinitions");
            }
        }
        else
        {
            File.Move(spawnFile + ".tmp", spawnFile);
            Console.WriteLine("Codegen created for: SpawnDefinitions");
        }
        
        
        Console.WriteLine(spawnableClasses.Count + " classes found");


        

        return true;
    }
}