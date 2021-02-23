// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "StreamDecoder.h"

std::vector<char> StreamDecoder::decodeFilteredStream(const char* streamData, std::size_t length, const StreamDecoder::flate_params& params) {
    boost::iostreams::filtering_istream decodeStream;
    decodeStream.push(predictor_reader({params.predictor, params.colors, params.bitsPerComponent, params.columns}, length));
    decodeStream.push(boost::iostreams::zlib_decompressor{});
    decodeStream.push(boost::iostreams::array_source{streamData, length});

    std::vector<char> outVector;
    boost::iostreams::back_insert_device<std::vector<char>> outSink{outVector};
    boost::iostreams::copy(decodeStream, outSink);
    
    return outVector;
}
