

#ifndef NEWTESTCLASS_H
#define NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>
#include "../mymfsKernel.h"

class newtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclass);

    CPPUNIT_TEST(configTest);
    CPPUNIT_TEST(importTest);
    CPPUNIT_TEST(listallTest);
    CPPUNIT_TEST(exportTest);
    CPPUNIT_TEST(removeTest);
    CPPUNIT_TEST(removeAllTest);
    CPPUNIT_TEST(head100Test);
    CPPUNIT_TEST(tail100Test);
    CPPUNIT_TEST(grepTest);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclass();
    virtual ~newtestclass();
    void setUp();
    void tearDown();

private:
    void configTest();
    void importTest();
    void listallTest();
    void exportTest();
    void removeTest();
    void removeAllTest();
    void head100Test();
    void tail100Test();
    void grepTest();
    bool fileExist(std::string filename);
};

#endif /* NEWTESTCLASS_H */

