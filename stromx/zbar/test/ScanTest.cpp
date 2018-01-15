/* 
*  Copyright 2016 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include <zbar.h>

#include <stromx/runtime/ConstDataRef.h>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/List.h>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/String.h>
#include <stromx/cvsupport/Image.h>

#include "stromx/zbar/Scan.h"

namespace stromx
{
namespace zbar
{

class ScanTest : public CPPUNIT_NS :: TestFixture
{
    CPPUNIT_TEST_SUITE (ScanTest);
    CPPUNIT_TEST (testExecute);
    CPPUNIT_TEST (testExecuteWrongSymbolType);
    CPPUNIT_TEST (testSetSymbolType);
    CPPUNIT_TEST_SUITE_END ();

    public:
        ScanTest() : m_operator(0) {}
        
        void setUp();
        void tearDown();

    protected:
        void testExecute();
        void testExecuteWrongSymbolType();
        void testSetSymbolType();
        
    private:
        runtime::OperatorTester* m_operator;
};

CPPUNIT_TEST_SUITE_REGISTRATION (ScanTest);

void ScanTest::setUp ( void )
{
    m_operator = new runtime::OperatorTester(new Scan());
    m_operator->initialize();
    m_operator->activate();
}

void ScanTest::testExecute()
{
    runtime::DataContainer input(new cvsupport::Image("barcode.png", cvsupport::Image::GRAYSCALE)); 
    m_operator->setParameter(Scan::SYMBOL_TYPE, runtime::Enum(::zbar::ZBAR_EAN13));
    m_operator->setInputData(Scan::INPUT, input);
    
    runtime::DataContainer result = m_operator->getOutputData(Scan::SYMBOLS);
    
    runtime::ReadAccess access(result);
    const std::vector<const runtime::Data*> & content = access.get<runtime::List>().content();
    CPPUNIT_ASSERT_EQUAL(std::size_t(1), content.size());
    const runtime::String* symbol = runtime::data_cast<runtime::String>(content[0]);
    CPPUNIT_ASSERT_EQUAL(symbol->get(), std::string("9876543210128"));
}

void ScanTest::testExecuteWrongSymbolType()
{
    runtime::DataContainer input(new cvsupport::Image("barcode.png", cvsupport::Image::GRAYSCALE)); 
    m_operator->setParameter(Scan::SYMBOL_TYPE, runtime::Enum(::zbar::ZBAR_EAN8));
    m_operator->setInputData(Scan::INPUT, input);
    
    runtime::DataContainer result = m_operator->getOutputData(Scan::SYMBOLS);
    
    runtime::ReadAccess access(result);
    const std::vector<const runtime::Data*> & content = access.get<runtime::List>().content();
    CPPUNIT_ASSERT_EQUAL(std::size_t(0), content.size());
}

void ScanTest::testSetSymbolType()
{
    m_operator->setParameter(Scan::SYMBOL_TYPE, runtime::Enum(::zbar::ZBAR_EAN13));
    runtime::DataRef value = m_operator->getParameter(Scan::SYMBOL_TYPE);
    
    CPPUNIT_ASSERT_EQUAL(runtime::Enum(::zbar::ZBAR_EAN13),
                         runtime::data_cast<runtime::Enum>(value));
}

void ScanTest::tearDown ( void )
{
    delete m_operator;
}

}
}

