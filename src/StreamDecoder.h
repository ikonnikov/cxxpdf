// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <memory>
#include <istream>
#include <stdio.h>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>

#include "PredictorFilter.hpp"

class StreamDecoder {
 public:
     struct flate_params {
         std::int64_t predictor;
         std::int64_t colors;
         std::int64_t bitsPerComponent;
         std::int64_t columns;

         flate_params(std::int64_t predictor = 1, std::int64_t colors = 1, std::int64_t bitsPerComponent = 8, std::int64_t columns = 1) :
             predictor(predictor),
             colors(colors),
             bitsPerComponent(bitsPerComponent),
             columns(columns)
         {}
     };

 public:
     static std::vector<char> decodeFilteredStream(const char* streamData, std::size_t length, const StreamDecoder::flate_params& params);
};
