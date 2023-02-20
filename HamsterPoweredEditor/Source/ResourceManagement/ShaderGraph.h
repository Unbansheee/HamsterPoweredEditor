#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/type_vec3.hpp>




struct NodeIO {
  typedef std::variant<float, int, bool, glm::vec3, glm::vec4> NodeValue;
  
    std::string name;
    std::string type;
    NodeValue default_value;

  //implement std::to_string for NodeValue
  std::string to_string() const
  {
    if (default_value.index() == 0)
      return std::to_string(std::get<0>(default_value));
    else if (default_value.index() == 1)
      return std::to_string(std::get<1>(default_value));
    else if (default_value.index() == 2)
      return std::to_string(std::get<2>(default_value));
    else if (default_value.index() == 3)
    {
      glm::vec3 v = std::get<3>(default_value);
      return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
    }
    else if (default_value.index() == 4)
    {
      glm::vec4 v = std::get<4>(default_value);
      return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w);
    }
    return "";
  
  }
};




class Node {
public:
    static void InitializeNodes(std::string jsonPath);
    inline static std::unordered_map<std::string, Node*> nodes;
    
    // Constructor that takes the name, code snippet, and input/output structs as arguments
    Node(const std::string& name, const std::string& code,
         const std::vector<NodeIO>& inputs,
         const std::vector<NodeIO>& outputs)
        : name_(name), code_(code), inputs_(inputs), outputs_(outputs) {}

    Node() = default;

    // Returns the name of the node
    const std::string& name() const { return name_; }

    // Returns the code snippet of the node
    const std::string& code() const { return code_; }

    // Returns the input structs of the node
    const std::vector<NodeIO>& inputs() const { return inputs_; }

    // Returns the output structs of the node
    const std::vector<NodeIO>& outputs() const { return outputs_; }

private:
    std::string name_;
    std::string code_;
    std::vector<NodeIO> inputs_;
    std::vector<NodeIO> outputs_;
};

class NodeInstance {
public:
  // Constructor that takes a pointer to the Node object as an argument
  NodeInstance(Node* node) : node_(node) {
    // Initialize the input and output values with the default values from the Node object
    for (const NodeIO& input : node_->inputs()) {
      inputs_[input.name] = input.default_value;
    }
    for (const NodeIO& output : node_->outputs()) {
      outputs_[output.name] = output.default_value;
    }
  }
  

  // Returns the name of the node
  const std::string& name() const { return node_->name(); }

  // Returns the code snippet of the node
  const std::string& code() const { return node_->code(); }

  // Returns the input names of the node
  const std::vector<NodeIO>& inputs() const { return node_->inputs(); }

  // Returns the output names of the node
  const std::vector<NodeIO>& outputs() const { return node_->outputs(); }

  // Connects the output of the source node to the input of this node
  void Connect(const std::string& input_name, NodeInstance* source, const std::string& output_name) {
    input_connections_[input_name] = {source, output_name};
  }

  // Sets the value of the input with the given name
  template <typename T>
  void SetInput(const std::string& name, T value) {
    inputs_[name] = value;
  }

  // Returns the value of the output with the given name
  template <typename T>
  T GetOutput(const std::string& name) const {
    return std::get<T>(outputs_.at(name));
  }

  friend class ShaderGraph;
private:
  Node* node_ = nullptr;
  std::unordered_map<std::string, NodeIO::NodeValue> inputs_;
  std::unordered_map<std::string, NodeIO::NodeValue> outputs_;
  std::unordered_map<std::string, std::pair<NodeInstance*, std::string>> input_connections_;
};

class ShaderGraph {
public:
  ShaderGraph()
  {
    nodes_.reserve(1024);
  }
  
  // Adds a node to the graph
  NodeInstance* AddNode(Node* node) {
    if (node) return &nodes_.emplace_back(node);
    return nullptr;
    
  }

  // Returns the list of nodes in the graph
  const std::vector<NodeInstance>& nodes() const { return nodes_; }

  std::string GenerateFunction(const NodeInstance& node)
  {
    std::string glsl;
    std::string nodeName = node.name();
    // Call the function for the node with the appropriate inputs
    glsl += node.name() + "(";
    for (const NodeIO& inputSlot : node.inputs())
    {
      std::string inputName = inputSlot.name;
      // If the input is connected to another node, use the output of that node
      if (node.input_connections_.count(inputName))
      {
        NodeInstance* source = node.input_connections_.at(inputName).first;
        std::string outputName = node.input_connections_.at(inputName).second;
        
        glsl += source->name() + "(" + GenerateFunction(*source) + ")";
        
      }
      else
      {
        glsl += inputSlot.to_string();
      }
      if (&inputSlot != &node.inputs().back())
      {
        glsl += ", ";
      }
    }
      
    //glsl += ");\n";
    return glsl;
  }
  
  // Connects the output of the source node to the input of the destination node
  void Connect(NodeInstance* dest, const std::string& input_name, NodeInstance* source, const std::string& output_name) {
    dest->Connect(input_name, source, output_name);
  }

  // Builds the GLSL shader by traversing the graph and composing the code snippets from the nodes
  std::string Build() {
    std::string glsl;
    // Generate a separate function for each node
    for (const NodeInstance& node : nodes_) {
      std::string parameters;
      // Generate the parameters for the function
      for (const NodeIO& input : node.inputs()) {
        parameters += input.type + " " + input.name + " = " + input.to_string();
        if (&input != &node.inputs().back()) {
          parameters += ", ";
        }
      }
      glsl += "// Function for node " + node.name() + "\n";
      glsl += "void " + node.name() + "(" + parameters + ") {\n";
      glsl += "  " + node.code() + "\n";
      glsl += "}\n\n";
    }
    
    glsl += "\nvoid main()\n{\n";

    // Generate the code for each node
    for (const auto& node : nodes_)
    {
      glsl += GenerateFunction(node);
    }

    glsl += "}\n";

    return glsl;
  }

private:
  std::vector<NodeInstance> nodes_;
};