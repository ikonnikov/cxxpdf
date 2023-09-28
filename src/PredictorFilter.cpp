// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PredictorFilter.hpp"

predictor_base::predictor_base(const predictor_params& params)
    : m_predictor(params.predictor),
      m_colors(params.colors),
      m_bitsPerComponent(params.bitsPerComponent),
      m_columns(params.columns) {
}

predictor_base::~predictor_base() noexcept {}

bool predictor_base::predictorDecodeImpl(const char*& begin_in, const char* end_in, char*& begin_out, char* end_out, bool flush) {
    boost::iostreams::array_source src_data{begin_in, end_in};
    boost::iostreams::stream<boost::iostreams::array_source> src_stream{src_data};

    boost::iostreams::array_sink dst_data{begin_out, end_out};
    boost::iostreams::stream<boost::iostreams::array_sink> dst_stream{dst_data};

    std::size_t rowLength = (m_colors * m_bitsPerComponent * m_columns + 7) / 8;
    std::size_t pixelLength = m_colors * m_bitsPerComponent / 8;

    std::vector<char> bufferCurr(rowLength);
    std::vector<char> bufferPrev(rowLength);

    while (!src_stream.eof()) {
        int filterType = 0;
        if (!src_stream.read((char*)&filterType, 1))
            return false;

        std::size_t counter = src_stream.gcount();
        begin_in += counter;

        if (filterType < 0)
            return false;

        if (!src_stream.read(&bufferCurr[0], rowLength))
            return false;

        counter = src_stream.gcount();
        begin_in += counter;
        
        switch (filterType) {
            case 0:  // PNG_FILTER_NONE
                break;

            case 1:  // PNG_FILTER_SUB
                for (std::size_t i = pixelLength; i < rowLength; i++) {
                    bufferCurr[i] += bufferCurr[i - pixelLength];
                }
                break;

            case 2:  // PNG_FILTER_UP
                for (std::size_t i = 0; i < rowLength; i++) {
                    bufferCurr[i] += bufferPrev[i];
                }
                break;

            case 3:  // PNG_FILTER_AVERAGE
                throw std::exception("PNG Predictor 3 not implemented yet");

            case 4:  // PNG_FILTER_PAETH
                throw std::exception("PNG Predictor 4 not implemented yet");

            default:
                throw std::exception("Unknown PNG predictor type");
                break;
        }

        dst_stream.write(&bufferCurr[0], counter);
        begin_out += counter;

        bufferPrev = bufferCurr;
    }

    return false;
}
