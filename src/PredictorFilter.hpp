// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <boost/iostreams/detail/buffer.hpp>
#include <boost/iostreams/constants.hpp>   // buffer size.
#include <boost/iostreams/filter/symmetric.hpp>
#include <boost/iostreams/pipeline.hpp>
#include <boost/iostreams/detail/config/dyn_link.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/copy.hpp>

struct predictor_params {
    std::int64_t predictor;
    std::int64_t colors;
    std::int64_t bitsPerComponent;
    std::int64_t columns;

    predictor_params(std::int64_t predictor = 1, std::int64_t colors = 1, std::int64_t bitsPerComponent = 8, std::int64_t columns = 1) :
        predictor(predictor),
        colors(colors),
        bitsPerComponent(bitsPerComponent),
        columns(columns)
    {}
};


class predictor_base {
 public:
     typedef char char_type;

 protected:
     explicit predictor_base(const predictor_params& params = predictor_params());
     virtual ~predictor_base() noexcept;

     bool predictorDecodeImpl(const char*& begin_in, const char* end_in, char*& begin_out, char* end_out, bool flush);

 private:
     std::int64_t m_predictor;
     std::int64_t m_colors;
     std::int64_t m_bitsPerComponent;
     std::int64_t m_columns;
};


template<typename Alloc = std::allocator<char> >
class predictor_reader_impl : public predictor_base {
 public:
     typedef char char_type;

     explicit predictor_reader_impl(const predictor_params& params = predictor_params());
     ~predictor_reader_impl();

     bool filter(const char*& begin_in, const char* end_in, char*& begin_out, char* end_out, bool flush);
     void close();
};

template<typename Alloc = std::allocator<char> >
struct basic_predictor_reader : boost::iostreams::symmetric_filter<predictor_reader_impl<Alloc>, Alloc> {
 private:
     typedef predictor_reader_impl<Alloc>                          impl_type;
     typedef boost::iostreams::symmetric_filter<impl_type, Alloc>  base_type;

 public:
     typedef typename base_type::char_type  char_type;
     typedef typename base_type::category   category;

     basic_predictor_reader(const predictor_params& params = predictor_params(), std::streamsize buffer_size = boost::iostreams::default_filter_buffer_size);
};
BOOST_IOSTREAMS_PIPABLE(basic_predictor_reader, 1)

typedef basic_predictor_reader<> predictor_reader;

template<typename Alloc>
basic_predictor_reader<Alloc>::basic_predictor_reader(const predictor_params& params, std::streamsize buffer_size) : base_type(buffer_size, params) {}

template<typename Alloc>
predictor_reader_impl<Alloc>::predictor_reader_impl(const predictor_params& params) : predictor_base(params) {}

template<typename Alloc>
predictor_reader_impl<Alloc>::~predictor_reader_impl() {}

template<typename Alloc>
bool predictor_reader_impl<Alloc>::filter(const char*& begin_in, const char* end_in, char*& begin_out, char* end_out, bool flush) {
    return predictorDecodeImpl(begin_in, end_in, begin_out, end_out, flush);
}

template<typename Alloc>
void predictor_reader_impl<Alloc>::close() {}
