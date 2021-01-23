// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <iterator>
#include <istream>
//#include <sstream>
//#include <iostream>
//#include <boost/iostreams/filtering_streambuf.hpp>
//#include <boost/iostreams/filter/zlib.hpp>
//#include <boost/iostreams/filter/gzip.hpp>
//#include <boost/iostreams/copy.hpp>
//#include <fstream>
//#include <boost/iostreams/device/array.hpp>
//#include <boost/iostreams/filtering_stream.hpp>

#include "PDFDictionary.h"
#include "PDFNumber.h"
#include "StreamDecoder.h"

class PDFStream;
using PDFStreamPtr = std::shared_ptr<PDFStream>;

class PDFStream : public virtual PDFDictionary {
 public:
    PDFStream();
    explicit PDFStream(const PDFDictionaryPtr& pdfDictionary);
    virtual ~PDFStream() noexcept;

    std::size_t getLength() const override;
    void setRawStreamBytes(const char* bytes, std::size_t length);
    //std::istream decodeFilteredStream() const;
    std::shared_ptr<std::istream> decodeFilteredStream() const;

 protected:
     std::size_t m_length;
     char* m_streamData;
};
