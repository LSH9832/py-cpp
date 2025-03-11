#ifndef TEST_ARGPARSER_CPP
#define TEST_ARGPARSER_CPP

#include "pylike/argparse.h"


argparse::ArgumentParser getArgs(int argc, char** argv)
{
    argparse::ArgumentParser parser("test args", argc, argv);
    parser.add_argument({"version"}, true, "software version");   // positional argument

    // the following are optional arguments
    parser.add_argument({"-s", "--source"}, "/dev/video0", "video source");                    // string
    parser.add_argument({"-sz", "--imgsz"}, std::vector<int>{640, 640}, "image size", "+");    // std::vector<int>
    parser.add_argument({"--show"}, STORE_TRUE, "show image");                                 // bool
    parser.add_argument({"--num-images"}, 512, "number of images");                            // int
    parser.add_argument({"-c", "--confidence"}, 0.25f, "confidence threshold");                // float or double

    logaddAndSetFromParser2(parser, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG);  // if need logger
    return parser; //.parse_args();
}


int main(int argc, char** argv)
{
    /**
     * enter the following command to test argparser
     * ./argtest v0.0.1 --source ./test.mp4 --imgsz 384 640 --num-images 45 --confidence 0.31415 --show --add-log ./test.log
     **/ 
    auto args = getArgs(argc, argv);
    
    // read args
    pystring version = args["version"];     // std::string version = args["version"];
    pystring source = args["source"];       // std::string source = args["source"];
    int height = args["imgsz"][0];
    int width = args["imgsz"][1];
    bool show = args["show"];
    int num_imgs = args["num-images"];
    float conf_thres = args["confidence"];

    auto imgsz = args["imgsz"].asList<int>();

    // if no arg name equals what you input, isValid() will return false;
    std::cout << "test valid: " << (args["non-exist-name"].isValid()?"true":"false") << std::endl;

    // show args
    std::cout << "version: " << version << std::endl;
    std::cout << "source: " << source << std::endl;
    std::cout << "image size: [" << imgsz[0] << ", " << imgsz[1] << "], " << args["imgsz"].asString() << "," << "[" << height << ", " << width << "]" << std::endl;
    std::cout << "show: " << (show?"true":"false") << "," << args["show"].asString() << std::endl;
    std::cout << "num images: " << num_imgs << std::endl;
    std::cout << "conf threshold: " << conf_thres << std::endl; 

    // show log settings
    std::cout << "log name: " << args["add-log"].as<pystring>() << std::endl;
    std::cout << "log level: " << args["log-level"].as<int>() << std::endl;
    std::cout << "show level: " << args["show-level"].as<int>() << std::endl;

    INFO << "nice" << ENDL;
    INFO << "end of log." << ENDLALL;   // if it is the last log of this program, use ENDLALL instead of ENDL
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}


#endif
