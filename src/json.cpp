#include <json.h>
// Only need boost for creating directories
#include <boost/filesystem.hpp>

using namespace rapidjson;
using namespace std;

Json::Json()
{
  // Ensure the location of files for temp output. REMOVE IN FINAL VERSION
  boost::filesystem::path dir("./tempFiles/json");
  boost::filesystem::create_directories(dir);
}

void Json::saveToFile(std::string _jsonString)
{
    ofstream jsonFile("./tempFiles/json/jsonString.json");

    if (jsonFile.is_open())
        jsonFile << _jsonString << std::endl;
        std::cout<<"Written to file: "<<std::endl;
    jsonFile.close();
}

void Json::replaceWord(std::string _oldWord, std::string _newWord)
{
    ifstream jsonFile("./tempFiles/json/jsonString.json");
    if(!jsonFile)
    {
        std::cout<<"Unable to open file."<<std::endl;
        exit(1);
    }
    std::string lineRead;
    size_t pos;
    std::string foundWord;
    while (jsonFile.good())
    {
        getline(jsonFile, lineRead);
        pos=lineRead.find(_oldWord);
        if(pos!=string::npos)              //reads through the line
        {
            lineRead.replace(pos, _oldWord.length(), _newWord);
            break;
        }
    }
    jsonFile.close();
    ofstream jsonFileNew("./tempFiles/json/jsonString.json");
    std::cout<<"\nNew file: \n"<<lineRead << std::endl;
    if (jsonFileNew.is_open())
        jsonFileNew << lineRead << std::endl;
    jsonFileNew.close();
}

//std::string Json::shaderType(int _shaderNumber)
//{
//    std::string shaderType;

//    if (_shaderNumber==1)
//    {
//        shaderType = "noise3D";
//    };
//    if (_shaderNumber==2)
//    {
//        shaderType = "cloud3D";
//    };

//    return shaderType;
//}

std::string Json::buildJson()
{
//    NOTE: This is useful to input a string directly into the writer. Input parameter int_shaderNumber to use.
//    std::string shaderTypeStr = shaderType(_shaderNumber);

//    std::cout<<"\nshader type: "<< shaderType(1)<<std::endl;
//    char *cstr = new char[shaderTypeStr.length() +1];
//    strcpy(cstr, shaderTypeStr.c_str());

    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("ShaderProgram");
        writer.StartObject();
        writer.Key("noise3D");
        writer.String("Phong");
        writer.Key("debug");
        writer.Bool(true);
        writer.Key("Shaders");
        writer.StartArray();
            writer.StartObject();
            writer.Key("type");
            writer.String("Vertex");
            writer.Key("name");
            writer.String("PhongVertex");
            writer.Key("path");
            writer.StartArray();
                writer.String("shaders/version.glsl");
                writer.String("shaders/common.glsl");
                writer.String("shaders/PhongVertex.glsl");
                writer.EndArray();
                writer.EndObject();
            writer.StartObject();
                writer.Key("type");
                writer.String("Fragment");
                writer.Key("name");
                writer.String("PhongFragment");
                writer.Key("path");
                writer.StartArray();
                    writer.String("shaders/version.glsl");
                    writer.String("shaders/common.glsl");
                    writer.String("shaders/noise3D.glsl");
                    writer.String("shaders/PhongFragment.glsl");
                    writer.EndArray();
                writer.EndObject();
            writer.EndArray();
        writer.EndObject();
    writer.EndObject();

    saveToFile(s.GetString());
    std::cout<<s.GetString()<<std::endl;

    return s.GetString();
}

Json::~Json()
{

}

//int main()
//{
//    buildJson();
//    replaceWord("Shader", "CHANGED");
//}