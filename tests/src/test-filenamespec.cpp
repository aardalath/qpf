// -*- C++ -*-

#include "filenamespec.h"

#include <cstdlib>
#include <string>

using namespace QPF;

#define TEST_SINGLE
#define MISSION

#ifdef TEST_SINGLE

#   ifdef MISSION
std::string strRegexp("([A-Z]+)_([A-Z]+)_([A-Z0-9_]+)_([^_]+)_([0-9T]+[\\.0-9]*Z)[_]*([0-9]*\\.*[0-9]*)");
std::string strAssign("%M=1;%T=2+3;%I=2;%S=4;%D=5;%f=5;%v=6");
std::string strTpl("%M_%T_%S_%f");
#   else
std::string strRegexp("EUC_([A-Z]+)_([A-Z0-9_]+)_([^_]+)_([0-9T]+[\\.0-9]*Z)[_]*([0-9]*\\.*[0-9]*)");
std::string strAssign("%T=1+2;%I=1;%S=3;%D=4;%f=4;%v=5");
std::string strTpl("%M_%T_%S_%f");
#   endif

#else
std::string strRegexp("EUC_([A-Z]+)_([A-Z0-9_]+)_([^_]+)_([0-9T]+[\\.0-9]*Z)[_]*([0-9]*\\.*[0-9]*)");
std::string strAssign("%T=1+2;%I=1;%S=3;%D=4;%f=4;%v=5");
std::string strTpl("%M_%T_%S_%f");
#endif


int main(int argc, char * argv[])
{
    int first = 1;

    if (strcmp(argv[first], "-r") == 0) {
        strRegexp = std::string(argv[first + 1]);
        first += 2;
    }

    if (strcmp(argv[first], "-a") == 0) {
        strAssign = std::string(argv[first + 1]);
        first += 2;
    }

    if (strcmp(argv[first], "-t") == 0) {
        strTpl = std::string(argv[first + 1]);
        first += 2;
    }

    FileNameSpec fs(strRegexp, strAssign);
    fs.setProductIdTpl(strTpl);

    for (int i = first; i < argc; ++i) {
        std::string file(argv[i]);
        ProductMetadata m;

        bool b = fs.parseFileName(file, m);
        //ProductMetadata::dump(std::cerr, &m);
        std::cerr << "Product size: " << m.productSize
                  << ".........." << (b ? "OK" : "ERROR!")
                  << std::endl;
    }

    return EXIT_SUCCESS;
}
