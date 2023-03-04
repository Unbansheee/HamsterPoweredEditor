#include "ShaderGraph.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

void Node::InitializeNodes(std::string jsonPath)
{
    nlohmann::json j;
    std::ifstream jsonFile(jsonPath);
    jsonFile >> j;

    // Iterate through all Nodes stored in the JSON file
    for (auto& node : j["Nodes"])
    {
        std::string name = node["Name"];
        std::vector<NodeIO> inputs;
        std::vector<NodeIO> outputs;
        std::string code = node["Code"];

        for (auto& input : node["Inputs"])
        {
            NodeIO& in = inputs.emplace_back();
            std::string name = input["Name"];
            in.name = name;
            in.type = input["Type"];
            in.default_value = &input["Default"];
        }

        for (auto& output : node["Outputs"])
        {
            NodeIO& out = inputs.emplace_back();
            std::string name = output["Name"];
            out.name = name;
            out.type = output["Type"];
            out.default_value = &output["Default"];
        }
        if (nodes.count(name))
        {
            delete nodes[name];
            nodes[name] = nullptr;
        }
        
        nodes[name] = new Node(name, code, inputs, outputs);
    }

    // print out all the nodes
    /*
    for (auto& node : nodes)
    {
        std::cout << node.second->name() << std::endl;
        std::cout << "Inputs:" << std::endl;
        for (auto& input : node.second->inputs())
        {
            std::cout << input.name << " " << input.type << std::endl;
        }
        std::cout << "Outputs:" << std::endl;
        for (auto& output : node.second->outputs())
        {
            std::cout << output.name << " " << output.type << std::endl;
        }
    }
    */
}
