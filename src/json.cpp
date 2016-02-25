#include <json.h>

// Only need boost for creating directories
#include <boost/filesystem.hpp>

//using namespace rapidxml;
using namespace rapidjson;
using namespace std;
using json = nlohmann::json;

// ----------------------------------------------------------------------------------------------------------------------
Json::Json()
{
  // Ensure the location of files for temp output. REMOVE IN FINAL VERSION
  boost::filesystem::path dir("./");
  boost::filesystem::create_directories(dir);
}

// ----------------------------------------------------------------------------------------------------------------------
// Writes an input string into to a file
void Json::writeFile(std::string _fileName, std::string _stringData)
{              
    ofstream fileName("./tempFiles/"+_fileName);   // ofstream jsonFile("./tempFiles/shader.json");
    if (fileName.is_open())
        fileName << _stringData << std::endl;
    fileName.close();
}

// ----------------------------------------------------------------------------------------------------------------------
// Reads an input file name and prints the file contents.
void Json::readFile(std::string _fileName)
{
    ifstream jsonFile("./tempFiles/"+_fileName+".json");
    if(!jsonFile)
    {
        std::cout<<"Unable to open file."<<std::endl;
        exit(1);
    }
    std::cout << "File contents \n " << jsonFile;
}

// ----------------------------------------------------------------------------------------------------------------------
// Builds the shader in json format
void Json::defaultShader()
{
    auto shaderProgramJson = R"(
    {
      "ShaderProgram": {
        "name": "Phong",
        "debug" : true,
      "Shaders"  : [
        {
          "type": "Vertex",
          "name": "PhongVertex",
          "path" : ["shaders/version.glsl",
                    "shaders/common.glsl",
                    "shaders/PhongVertex.glsl"]

        },
        {
          "type": "Fragment",
          "name": "PhongFragment",
          "path" : ["shaders/version.glsl",
                    "shaders/common.glsl",
                    "shaders/noise3D2.glsl",
                    "shaders/PhongFragment.glsl"]
        }
      ]
     }
    }
    )"_json;
    writeFile("jsonString.json", shaderProgramJson.dump(1));
}


// ----------------------------------------------------------------------------------------------------------------------
// Appends shader data (name, type, value).
void Json::addShaderData(string _name, string _type, double _value)
{
    json shaderDataJson;

    shaderDataJson[_name]={{"Name", _name}, {"Type", _value}, {"Value", _value}};
    writeFile("shaderData.json", shaderDataJson.dump(1));
}

// ----------------------------------------------------------------------------------------------------------------------
// (may be redundant) Reads through the Json string, finds a word and replaces with a new word.
void Json::replaceWord(std::string _oldWord, std::string _newWord)
{

//    string hello = "true";
//    string defaultShader = R"string(
//     {
//      "ShaderProgram": {
//        "name": )string + hello + "string2(,
//        "debug" : "hello"}
//    }
//    )string2";


//    string defaultShader = R"(
//     {
//      "ShaderProgram": {
//        "name": "Phong",
//        "debug" : "hello"}
//    }
//    )";


    string defaultShader = R"(
     {
      "ShaderProgram": {
        "name": "Phong",
        "debug" : "hello",
      "Shaders"  : [
        {
          "type": "Vertex",
          "name": "PhongVertex",
          "path" : ["shaders/version.glsl",
                    "shaders/common.glsl",
                    "shaders/PhongVertex.glsl"]

        },
        {
          "type": "Fragment",
          "name": "PhongFragment",
          "path" : ["shaders/version.glsl",
                    "shaders/common.glsl",
                    "shaders/noise3D2.glsl",
                    "shaders/PhongFragment.glsl"]
        }
      ]
     }
    }
    )";

    _oldWord = "Phong";
    _newWord = "NEW WORD";
    size_t pos;
    pos = defaultShader.find(_oldWord);
    if (pos!=string::npos)
    {
        defaultShader.replace(pos, _oldWord.length(), _newWord);
    };
    //defaultShader.replace(defaultShader.begin(), defaultShader.end(), "hello", "there");

    //string jsonString = "{\"ShaderProgram\": {\"name\": \"Phong\",\"debug\" : \"hello\",\"Shaders\" }}";
    //string jsonString = "{\"ShaderProgram\":"+True+", \"pi\": 3.141 }";
   // string jsonString = "{\"ShaderProgram\": {\"name\":\"Phong\", \"debug\": "+True+"}}";
   // string defaultShader = R"({"ShaderProgram": {"name": "Phong","debug" : "hello","Shaders"  : [{"type": "Vertex","name": "PhongVertex","path" : ["shaders/version.glsl","shaders/common.glsl","shaders/PhongVertex.glsl"]}]}}")";

    auto shaderProgramJson = json::parse(defaultShader);
    writeFile("jsonString2.json", shaderProgramJson.dump(1));





//    string jsonString = (("{\"ShaderProgram\":"+string("true")+string(", \"pi\": 3.141 }")));
    //json shaderProgramJson = string("{"ShaderProgram": true}" +hello+"{}"),_json;
    //json shaderProgramJson = string("{ \"ShaderProgram\": true, \"pi\": 3.141 }"),_json;

//    ifstream jsonFile("./tempFiles/jsonString.json");
////    if(!jsonFile)
////    {
////        std::cout<<"Unable to open file."<<std::endl;
////        exit(1);
////    }
//    std::cout<<"HELLOOOOO"<<std::endl;
//    std::cout<< "printing jsonFile" << jsonFile;
//    std::string lineRead;
//    size_t pos;
//    std::string foundWord;
//    while (jsonFile.good())
//    {
//        getline(jsonFile, lineRead);
//        pos=lineRead.find(_oldWord);
//        if(pos!=string::npos)              //reads through the line
//        {
//            lineRead.replace(pos, _oldWord.length(), _newWord);
//            break;
//        }
//    }
//    jsonFile.close();
//    ofstream jsonFileNew("./tempFiles/jsonString.json");
//    std::cout<<"\nNew file: \n"<<lineRead << std::endl;
//    if (jsonFileNew.is_open())
//        jsonFileNew << lineRead << std::endl;
//    jsonFileNew.close();


//    boost::property_tree::ptree jsontree;
//    boost::property_tree::read_json(jsonFile, jsontree);
//    jsonFile.get<char>
//    StringStream s (jsonFile)
//    Document document;
//    document.Parse(jsonFile);
}


Json::~Json()
{}




//string defaultShader = R"(
// {
//  "ShaderProgram": {
//    "name": "Phong",
//    "debug" : "hello",
//  "Shaders"  : [
//    {
//      "type": "Vertex",
//      "name": "PhongVertex",
//      "path" : ["shaders/version.glsl",
//                "shaders/common.glsl",
//                "shaders/PhongVertex.glsl"]

//    },
//    {
//      "type": "Fragment",
//      "name": "PhongFragment",
//      "path" : ["shaders/version.glsl",
//                "shaders/common.glsl",
//                "shaders/noise3D2.glsl",
//                "shaders/PhongFragment.glsl"]
//    }
//  ]
// }
//}
//)";
