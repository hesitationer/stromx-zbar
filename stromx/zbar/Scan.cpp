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
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/Image.h>
#include <stromx/runtime/List.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Matrix.h>
#include <stromx/runtime/MatrixDescription.h>
#include <stromx/runtime/ReadAccess.h>
#include <stromx/runtime/Variant.h>

#include "stromx/zbar/Locale.h"

#include <zbar.h>

using namespace stromx::runtime;
    
namespace stromx
{
namespace zbar
{        

const std::string Scan::TYPE("SCAN");

const std::string Scan::PACKAGE(STROMX_ZBAR_PACKAGE_NAME);
const runtime::Version Scan::VERSION(0, 1, 0);
    
Scan::Scan()
  : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs(), setupParameters())
  , m_zbarScanner(new ::zbar::ImageScanner)
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
    
    runtime::List* symbols = new runtime::List();
    DataContainer result(symbols);
    
    provider.sendOutputData(runtime::Id2DataPair(SYMBOLS, result));
}

const std::vector<const runtime::Description*> Scan::setupInputs()
{
    std::vector<const Description*> inputs;
    
    runtime::Description* input = new runtime::Description(INPUT, runtime::Variant::MONO_8_IMAGE);
    input->setTitle(L_("Input"));
    inputs.push_back(input);
                    
    return inputs;
}

const std::vector<const runtime::Description*> Scan::setupOutputs()
{
    std::vector<const runtime::Description*> outputs;
    
    runtime::Description* symbols = new runtime::Description(SYMBOLS, runtime::Variant::LIST);
    symbols->setTitle(L_("Symbols"));
    outputs.push_back(symbols);
    
    return outputs;
}

const std::vector<const runtime::Parameter*> Scan::setupParameters()
{
    std::vector<const runtime::Parameter*> parameters;
                                
    return parameters;
}

} 
}
