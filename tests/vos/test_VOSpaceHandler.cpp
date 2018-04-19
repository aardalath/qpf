#include "test_VOSpaceHandler.h"

#include <iostream>
#include <fstream>  

//#define CheckResultOf(s,r) do {                                         \
//    ev.clear();                                                         \
//    ev.set(std::string( #s ));                                          \
//    EXPECT_EQ(ev.getValue(), r);                                        \
//    } while (0)

namespace TestVOSpaceHandler {

TEST_F(TestVOSpaceHandler, Test_setVOSpaceUrl) {

    std::string url("https://vospace.esac.esa.int/vospaece/");
    
    VOSpaceHandler * vosHdl = new VOSpaceHandler(url);
    EXPECT_NE(vosHdl, nullptr);

    vosHdl->setVOSpaceUrl(url);
    
    delete vosHdl;
}

TEST_F(TestVOSpaceHandler, Test_setAuth) {
    
    std::string url("https://vospace.esac.esa.int/vospace/");
    
    VOSpaceHandler * vosHdl = new VOSpaceHandler(url);
    EXPECT_NE(vosHdl, nullptr);

    std::string user("eucops"), pwd("Eu314_clid");
    vosHdl->setAuth(user, pwd);
    
    delete vosHdl;
}

TEST_F(TestVOSpaceHandler, Test_uploadAndDownload) {
    
    std::string url("https://vospace.esac.esa.int/vospace/");
    
    VOSpaceHandler * vosHdl = new VOSpaceHandler(url);
    EXPECT_NE(vosHdl, nullptr);

    std::string user("eucops"), pwd("Eu314_clid");
    vosHdl->setAuth(user, pwd);

    std::string folder("tests");
    std::string fileName1("file1-test.txt");
    std::string fileName2("file2-test.txt");

    std::string content("This is the file content");

    bool result = vosHdl->uploadToFile(folder, fileName1, content);
    EXPECT_EQ(result, true);

    std::ofstream upfile(fileName2);
    std::string content2("This is another different file content");
    upfile << content2 << std::endl;
    upfile.close();

    result = vosHdl->uploadFile(folder, fileName2);

    EXPECT_EQ(result, true);

    std::string retrieved;
    result = vosHdl->downloadFromFile(folder, fileName1, retrieved);

    EXPECT_EQ(result, true);
    EXPECT_EQ(retrieved, content);

    result = vosHdl->downloadFile(folder, fileName2, fileName2);

    EXPECT_EQ(result, true);
    
    std::string retrieved2;
    std::ifstream dwnfile(fileName2);
    std::getline(dwnfile, retrieved2);
    dwnfile.close();

    EXPECT_EQ(retrieved2, content2);

    delete vosHdl;
    
}

}           
