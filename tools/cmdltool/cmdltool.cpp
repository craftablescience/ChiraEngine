#include <cstdlib>
#include <filesystem>
#include <fstream>

#include <core/CommandLine.h>
#include <core/Engine.h>
#include <loader/mesh/ChiraMeshLoader.h>
#include <loader/mesh/OBJMeshLoader.h>
#include <render/mesh/MeshData.h>

#include "../ToolHelpers.h"

using namespace chira;

CHIRA_SETUP_CLI_TOOL(CMDLTOOL, "1.0",
                     "Parameters:"                                                  "\n"
                     "-h               : Display this help message"                 "\n"
                     "-i <input file>  : Path of the file to convert"               "\n"
                     "-s <type>        : Type of the input file (cmdl, obj, etc.)"  "\n"
                     "-t <type>        : Type of the output file (cmdl, obj, etc.)" "\n"
                     "                   The default is cmdl"                       "\n"
                     "-o <output file> : Destination for the converted file"        "\n");

int main(int argc, char* argv[]) {
    Engine::preinit(argc, argv);

    // make sure we actually discard resources. we don't ever call Engine::run()
    // so we never do the proper shutdown and have to manually call this
    std::atexit(Resource::discardAll);

    if (argc == 0) {
        printHelp();
        return EXIT_FAILURE;
    }

    if (CommandLine::has("-h")) {
        printHelp();
        return EXIT_SUCCESS;
    }

    std::filesystem::path inputPath;
    if (auto input = CommandLine::get("-i"); !input.empty()) {
        inputPath = input;
    } else {
        LOG_CMDLTOOL.error("No input file provided!\n");
        printHelp();
        return EXIT_FAILURE;
    }

    std::string inputType = CommandLine::get("-s").data();
    if (inputType.empty()) {
        LOG_CMDLTOOL.error("No input type provided!\n");
        printHelp();
        return EXIT_FAILURE;
    }

    std::string outputType = CommandLine::getOr("-t", "cmdl").data();

    std::filesystem::path outputPath;
    if (auto output = CommandLine::get("-o"); !output.empty()) {
        outputPath = output;
    } else {
        LOG_CMDLTOOL.error("No output file provided!\n");
        printHelp();
        return EXIT_FAILURE;
    }

    // todo: populate these through some kind of registry
    IMeshLoader::addMeshLoader("obj", new OBJMeshLoader{});
    IMeshLoader::addMeshLoader("cmdl", new ChiraMeshLoader{});

    LOG_CMDLTOOL.info("Attempting to convert mesh file \"{}\"...", inputPath.filename().string());

    MeshData mesh{};
    //Resource::addResourceProvider(new FilesystemResourceProvider{inputPath.parent_path().string()});
    //mesh.appendMeshData(inputType, FilesystemResourceProvider::getResourceIdentifier(inputPath.string()));

    std::ofstream file{outputPath.string(), std::ios::binary};
    std::vector<byte> meshData = mesh.getMeshData(outputType);
    file.write(reinterpret_cast<const char*>(meshData.data()), static_cast<std::streamsize>(meshData.size()));
    file.close();
    LOG_CMDLTOOL.infoImportant("Conversion complete! File written to \"{}\"", outputPath.string());

    return EXIT_SUCCESS;
}
