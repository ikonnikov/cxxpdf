// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <stdint.h>

#include <any>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <utility>
#include <variant>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/lexical_cast.hpp>

class Tokeniser;
class PDFArray;
class PDFDictionary;
class PDFIndirectReference;
class PDFLiteral;
class PDFName;
class PDFNumber;
class PDFObject;
class PDFStream;
class PDFString;

using PDFArrayPtr = std::shared_ptr<PDFArray>;
using TokeniserPtr = std::shared_ptr<Tokeniser>;
using PDFDictionaryPtr = std::shared_ptr<PDFDictionary>;
using PDFIndirectReferencePtr = std::shared_ptr<PDFIndirectReference>;
using PDFLiteralPtr = std::shared_ptr<PDFLiteral>;
using PDFNamePtr = std::shared_ptr<PDFName>;
using PDFNumberPtr = std::shared_ptr<PDFNumber>;
using PDFObjectPtr = std::shared_ptr<PDFObject>;
using PDFStreamPtr = std::shared_ptr<PDFStream>;
using PDFStringPtr = std::shared_ptr<PDFString>;

using PDFFileStream = boost::iostreams::stream<boost::iostreams::file_source>;
using PDFArrayStream = boost::iostreams::stream<boost::iostreams::array_source>;
