
#include "newtestclass.h"


CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

newtestclass::newtestclass() {
}

newtestclass::~newtestclass() {
}

void newtestclass::setUp() {
}

void newtestclass::tearDown() {
}

void newtestclass::configTest()
{
    config("D", "D E F");
    std::string conf = "D";
    int counter = 0;
    conf.append(CONFIG_FILE);  
    if (fileExist(conf))
        counter++;
    
    conf = "E";
    conf.append(CONFIG_FILE);
    if (fileExist(conf))
        counter++;
    
    conf = "F";
    conf.append(CONFIG_FILE);
    if (fileExist(conf))
        counter++;
    
    CPPUNIT_ASSERT(3 == counter);
}


void newtestclass::importTest()
{
    import("D", "tests/test.jpg");
    bool parts = false;
    int counter = 0;
    if (fileExist("D://test.jpg.1")
        && fileExist("D://test.jpg.2")
        && fileExist("D://test.jpg.3"))
    {
        counter++;
        if (fileExist("E://test.jpg.zip") || fileExist("F://test.jpg.zip"))
            counter++;
    }
    if (fileExist("E://test.jpg.1")
    && fileExist("E://test.jpg.2")
    && fileExist("E://test.jpg.3"))
    {
        counter++;
        if (fileExist("D://test.jpg.zip") || fileExist("F://test.jpg.zip"))
            counter++;
    }
    if (fileExist("F://test.jpg.1")
    && fileExist("F://test.jpg.2")
    && fileExist("F://test.jpg.3"))
    {
        counter++;
        if (fileExist("D://test.jpg.zip") || fileExist("E://test.jpg.zip"))
            counter++;
    }
    
    CPPUNIT_ASSERT(2 == counter);
}


void newtestclass::listallTest()
{
    CPPUNIT_ASSERT(1 == listall("D"));
}


void newtestclass::exportTest()
{
    _export("D", "tests/test.jpg", "export.jpg");
    
    CPPUNIT_ASSERT(fileExist("export.jpg"));
}


void newtestclass::removeTest()
{
    _remove("D", "tests/test.jpg");
    int counter = 0;
    if (fileExist("D://test.jpg.1")
        && fileExist("D://test.jpg.2")
        && fileExist("D://test.jpg.3"))
    {
        counter++;
        if (fileExist("E://test.jpg.zip") || fileExist("F://test.jpg.zip"))
            counter++;
    }
    if (fileExist("E://test.jpg.1")
    && fileExist("E://test.jpg.2")
    && fileExist("E://test.jpg.3"))
    {
        counter++;
        if (fileExist("D://test.jpg.zip") || fileExist("F://test.jpg.zip"))
            counter++;
    }
    if (fileExist("F://test.jpg.1")
    && fileExist("F://test.jpg.2")
    && fileExist("F://test.jpg.3"))
    {
        counter++;
        if (fileExist("D://test.jpg.zip") || fileExist("E://test.jpg.zip"))
            counter++;
    }
    
    CPPUNIT_ASSERT(0 == counter);
}


void newtestclass::removeAllTest()
{
    removeAll("D");
    
    CPPUNIT_ASSERT(0 == listall("D"));
}


void newtestclass::head100Test()
{
    import("D", "tests/pi.txt");
    CPPUNIT_ASSERT(head100("D", "tests/pi.txt"));
}


void newtestclass::tail100Test()
{
    CPPUNIT_ASSERT(tail100("D", "tests/pi.txt"));
}


void newtestclass::grepTest()
{
    CPPUNIT_ASSERT(grep("D", "tests/pi.txt", "Eve"));
}


bool newtestclass::fileExist(std::string filename)
{
    if (FILE *file = fopen(filename.c_str(), "r"))
    {
        fclose(file);
        return true;
    }
    else
        return false;
}