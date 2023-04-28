#include <core/Engine.h>
#include <core/Logger.h>
#include <resource/provider/FilesystemResourceProvider.h>
#include <render/mesh/MeshDataResource.h>
#include <filesystem>
#include <fstream>

#define VERSION "1.0"

using namespace chira;

CHIRA_CREATE_LOG(CMDLTOOL);

void dispHelp() {
    std::string helpLines[] = {
        "Usage:",
        "-h   display help message",
        "-i <input-file>   file to convert",
        "-o <output-file>   destination for converted file"
    };

    for (auto line : helpLines) {
        LOG_CMDLTOOL.info(line);
    }
}

int main(int argc, const char* const argv[]) {
    Engine::preInit(argc, argv);

    LOG_CMDLTOOL.info("CMDLTool v%s", VERSION);

    if (argc == 0) {
        dispHelp();
        // make sure we actually discard resources.
        // we don't ever call Engine::run() so we never do the proper shutdown
        // and have to manually call this
        Resource::discardAll();
        return 0;
    }

    std::string inputFile;
    std::string outputFile;

    for (int i = 0; i < argc; i++) {
        auto param = argv[i];

        if (strcmp(param, "-h") == 0) {
            dispHelp();
            Resource::discardAll();
            return 0;
        }

        if (strcmp(param, "-i") == 0)
            inputFile = std::string(argv[i + 1]);

        if (strcmp(param, "-o") == 0)
            outputFile = std::string(argv[i + 1]);
    }

    if (inputFile.empty()) {
        LOG_CMDLTOOL.error("No input file provided.");
        dispHelp();
        Resource::discardAll();
        return 0;
    }

    if (outputFile.empty()) {
        LOG_CMDLTOOL.error("No output file provided.");
        dispHelp();
        Resource::discardAll();
        return 0;
    }

    // path of input is added to provider
    std::filesystem::path if_path = std::filesystem::path(inputFile);
    Resource::addResourceProvider(new FilesystemResourceProvider{if_path.remove_filename().string()});

    SharedPointer<MeshDataResource> mesh = Resource::getResource<MeshDataResource>(if_path.filename());

    std::ofstream file{outputFile, std::ios::binary};
    std::vector<byte> meshData = mesh->getMeshData("cmdl");
    file.write(reinterpret_cast<const char*>(meshData.data()), static_cast<std::streamsize>(meshData.size()));
    file.close();

    Resource::discardAll();
    return 0;
}