
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>


SceneSerializer::SceneSerializer()
{
}

SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}

SceneSerializer::~SceneSerializer()
{
}

void SceneSerializer::Serialize(const std::string& output_file)
{
    YAML::Emitter out;
    out << "Hello, World!";

    out << YAML::BeginSeq;
    out << "eggs";
    out << "bread";
    out << "milk";
    out << YAML::EndSeq;

    out << YAML::BeginMap;
    out << YAML::Key << "name";
    out << YAML::Value << "Ryan Braun";
    out << YAML::Key << "position";
    out << YAML::Value << "LF";
    out << YAML::EndMap;

    out << YAML::BeginMap;
    out << YAML::Key << "name";
    out << YAML::Value << "Barack Obama";
    out << YAML::Key << "children";
    out << YAML::Value << YAML::BeginSeq << "Sasha" << "Malia" << YAML::EndSeq;
    out << YAML::EndMap;

    out << YAML::Flow;
    out << YAML::BeginSeq << 2 << 3 << 5 << 7 << 11 << YAML::EndSeq;

    std::cout << "Here's the output YAML:\n\n" << out.c_str() << "\n\n"; // prints "Hello, World!"
}

void SceneSerializer::DeSerialize(const std::string& input_file)
{
    std::ifstream stream(input_file);
    std::stringstream strStream;
    strStream << stream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());


}
