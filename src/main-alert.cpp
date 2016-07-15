#include <iostream>
#include "alert.h"

using namespace QPF;

int main(int argc, char * argv[])
{
    time_t t = time(0);
    Alert::TimeStampType timeStamp;

    (void)localtime_r(&t, &timeStamp);

    std::vector<std::string> lines;
    lines.push_back("Cannot file required file");

    Alert a1;
    a1.setTimeStamp(timeStamp)
        .setGroup(Alert::System)
        .setSeverity(Alert::Warning)
        .setType(Alert::Resource)
        .setOrigin("CheckBias")
        .setMessages(lines);

    lines.pop_back();
    lines.push_back("CountBadPixels failed");

    Alert a2(timeStamp, Alert::Diagnostics, Alert::Error, Alert::Diagnostic, "Background", lines);

    lines.pop_back();
    lines.push_back("Number of pixels out of range");

    int numOfPix = 657;
    Alert a3(timeStamp, Alert::Diagnostics, Alert::Error, Alert::Diagnostic, "CheckOversamplig", lines);
    a3.setVar(Alert::createNewVar("numOfPix", numOfPix, 15, 150));

    std::cerr << "Alert: " << a1.dump() << std::endl;
    std::cerr << "Alert: " << a2.dump() << std::endl;
    std::cerr << "Alert: " << a3.dump() << std::endl;

    return 0;
}
