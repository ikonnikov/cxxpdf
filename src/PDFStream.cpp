// Copyright (c) 2020-2021 cxxPDF project, Ikonnikov Kirill, All rights reserved.
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
    
    const PDFDictionaryPtr decodeDictionaryToken = PDFMutator::xcast<PDFDictionary>(get("DecodeParms"));

    std::int64_t predictor = PDFMutator::xcast<PDFNumber>(decodeDictionaryToken->get("Predictor"))->asInteger(1);
    std::int64_t colors = PDFMutator::xcast<PDFNumber>(decodeDictionaryToken->get("Colors"))->asInteger(1);
    std::int64_t bitsPerComponent = PDFMutator::xcast<PDFNumber>(decodeDictionaryToken->get("BitsPerComponent"))->asInteger(8);
    std::int64_t columns = PDFMutator::xcast<PDFNumber>(decodeDictionaryToken->get("Columns"))->asInteger(1);

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
