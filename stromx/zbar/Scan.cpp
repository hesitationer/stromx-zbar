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

#include "stromx/zbar/Scan.h"

#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/List.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Matrix.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/String.h>
#include <stromx/runtime/Variant.h>

#include "stromx/zbar/Locale.h"

#include <zbar.h>

using namespace stromx::runtime;
    
namespace stromx
{
namespace zbar
{        

const std::string Scan::TYPE("Scan");

const std::string Scan::PACKAGE(STROMX_ZBAR_PACKAGE_NAME);
const runtime::Version Scan::VERSION(0, 1, 0);
    
Scan::Scan()
  : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
  , m_zbarScanner(new ::zbar::ImageScanner)
  , m_symbolType(::zbar::ZBAR_NONE)
{
}

Scan::~Scan()
{
    delete m_zbarScanner;
}

const runtime::DataRef Scan::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case SYMBOL_TYPE:
        return m_symbolType;
    default:
        throw WrongParameterId(id, *this);
    }
}

void Scan::setParameter(const unsigned int id, const runtime::Data& value)
{
    try
    {
        switch(id)
        {
        case SYMBOL_TYPE:
            m_symbolType = data_cast<Enum>(value);
            break;
        default:
            throw WrongParameterId(id, *this);
        }
    }
    catch(std::bad_cast&)
    {
        throw WrongParameterType(parameter(id), *this);
    }
}

void Scan::execute(runtime::DataProvider& provider)
{
    Id2DataPair inputMapper(INPUT);
    provider.receiveInputData(inputMapper);
    ReadAccess access(inputMapper.data());
    
    const runtime::Image & image = access.get<runtime::Image>();
    if (! image.variant().isVariant(runtime::Variant::MONO_8_IMAGE))
        throw runtime::InputError(INPUT, *this, "Wrong input data variant.");
    
    ::zbar::Image zbarImage(image.width(), image.height(), "Y800", image.data(),
                            image.height() * image.stride());
    
    ::zbar::ImageScanner scanner;
    scanner.set_config(::zbar::zbar_symbol_type_t(0), 
                       ::zbar::ZBAR_CFG_ENABLE, 0);
    scanner.set_config(::zbar::zbar_symbol_type_t(int(m_symbolType)),
                       ::zbar::ZBAR_CFG_ENABLE, 1);
    scanner.scan(zbarImage);
    
    runtime::List* symbols = new runtime::List();
    
    const ::zbar::SymbolSet & zbarSymbols = scanner.get_results();
    for (::zbar::SymbolIterator iter = zbarSymbols.symbol_begin();
         iter != zbarSymbols.symbol_end();
         ++iter)
    {
        symbols->content().push_back(new String(iter->get_data()));
    }
    
    DataContainer result(symbols);
    provider.sendOutputData(runtime::Id2DataPair(SYMBOLS, result));
}

const std::vector<const runtime::Input*> Scan::setupInputs()
{
    std::vector<const Input*> inputs;
    
    runtime::Input* input = new runtime::Input(INPUT, runtime::Variant::MONO_8_IMAGE);
    input->setTitle(L_("Input"));
    inputs.push_back(input);
                    
    return inputs;
}

const std::vector<const runtime::Output*> Scan::setupOutputs()
{
    std::vector<const runtime::Output*> outputs;
    
    runtime::Output* symbols = new runtime::Output(SYMBOLS, runtime::Variant::LIST);
    symbols->setTitle(L_("Symbols"));
    outputs.push_back(symbols);
    
    return outputs;
}

const std::vector<const runtime::Parameter*> Scan::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    EnumParameter* symbolType = new EnumParameter(SYMBOL_TYPE);
    symbolType->setTitle(L_("Symbol type"));
    symbolType->setAccessMode(Parameter::ACTIVATED_WRITE);
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_NONE), L_("None")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_EAN8), L_("EAN-8")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_UPCE), L_("UPC-E")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_ISBN10), L_("ISBN-10")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_UPCA), L_("UPC-A")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_EAN13), L_("EAN-13")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_ISBN13), L_("ISBN-13")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_I25), L_("Interleaved 2 of 5")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_CODE39), L_("Code 39")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_PDF417), L_("PDF417")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_QRCODE), L_("QR Code")));
    symbolType->add(EnumDescription(Enum(::zbar::ZBAR_CODE128), L_("Code 128")));   
    
    parameters.push_back(symbolType);                                  
                                
    return parameters;
}

} 
}
