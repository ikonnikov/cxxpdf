// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFStream.h"

PDFStream::PDFStream() : m_length(0), m_streamData(nullptr) {
    this->m_type = PDFObject::Types::kSTREAM;
}

PDFStream::PDFStream(const PDFDictionaryPtr& pdfDictionary) : PDFStream() {
    std::copy(pdfDictionary->begin(), pdfDictionary->end(), std::inserter(m_value, m_value.end()));
}

PDFStream::~PDFStream() noexcept {
    if (m_streamData)
        delete[] m_streamData;
}

std::size_t PDFStream::getLength() const {
    return m_length;
}

void PDFStream::setRawStreamBytes(const char* bytes, std::size_t length) {
    m_streamData = new char[length]();
    ::memcpy_s(m_streamData, length, bytes, length);

    m_length = length;
}

std::vector<char> PDFStream::decodeFilteredStream() const {
    // todo: enum filters and get decode params (Predictor, Colors, BitsPerComponent, Columns)
    PDFObjectPtr decodeParmsToken = get("DecodeParms");

    std::int64_t predictor = 1;
    std::int64_t colors = 1;
    std::int64_t bitsPerComponent = 8;
    std::int64_t columns = 1;

    if (decodeParmsToken != nullptr) {
        if (decodeParmsToken->isDictionary()) {
            PDFDictionaryPtr decodeDictionaryToken = std::dynamic_pointer_cast<PDFDictionary>(decodeParmsToken);

            const PDFObjectPtr predictorToken = decodeDictionaryToken->get("Predictor");

            if (predictorToken != nullptr) {
                predictor = std::dynamic_pointer_cast<PDFNumber>(predictorToken)->asInteger(predictor);
            }

            const PDFObjectPtr colorsToken = decodeDictionaryToken->get("Colors");

            if (colorsToken != nullptr) {
                colors = std::dynamic_pointer_cast<PDFNumber>(colorsToken)->asInteger(colors);
            }

            const PDFObjectPtr bitsPerComponentToken = decodeDictionaryToken->get("BitsPerComponent");

            if (bitsPerComponentToken != nullptr) {
                bitsPerComponent = std::dynamic_pointer_cast<PDFNumber>(bitsPerComponentToken)->asInteger(bitsPerComponent);
            }

            const PDFObjectPtr columnsToken = decodeDictionaryToken->get("Columns");

            if (columnsToken != nullptr) {
                columns = std::dynamic_pointer_cast<PDFNumber>(columnsToken)->asInteger(columns);
            }
        }
    }

    return StreamDecoder::decodeFilteredStream(m_streamData, m_length, {predictor, colors, bitsPerComponent, columns});

    //return StreamDecoder::makeFlateDecodeStream(m_streamData, m_length, {predictor, colors, bitsPerComponent, columns});

    // todo: delete below

    /*
    boost::iostreams::array_source streamer{m_streamData, m_length};

    char buf[512];
    ::memset(&buf[0], '\0', 512);
    
    streamer.read(&buf[0], 100);
    

    boost::iostreams::filtering_istream inStream;
    inStream.push(boost::iostreams::zlib_decompressor());
    inStream.push(streamer);

    std::stringstream outStream;
    boost::iostreams::copy(inStream, outStream);
    */
    //!!!!!!!!!!!return std::make_unique<std::stringstream>(std::move(outStream));

    //return outStream;
    
    //outStream.read(buf, 512);
    //std::size_t read_size = outStream.gcount();

    //std::istream is(&in);
    //is.read(buf, 512);
    //std::size_t read_size = is.gcount();

    //std::ofstream arch(".//test//resources//stream.zip", std::ios::out | std::ios::binary);
    //boost::iostreams::copy(in, arch);

    //std::ifstream file(".//test//resources//minimal.pdf", std::ios::in | std::ios::binary);
    //std::ofstream arch(".//test//resources//minimal.pdf.zip", std::ios::out | std::ios::binary);
    //boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    //in.push(boost::iostreams::gzip_compressor());
    //in.push(file);
    //boost::iostreams::copy(in, arch);




    //streamer.rdbuf
    //xReadWrite.write((const char*)x, 6);
    //std::istream stream;
    //(m_streamData);
    
    //return nullptr;
}
