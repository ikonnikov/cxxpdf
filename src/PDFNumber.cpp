// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFNumber.h"

PDFNumber::PDFNumber(int64_t intValue) : m_value(intValue) {
    std::cout << __FUNCSIG__ << " = " << std::get<int64_t>(m_value) << std::endl;
    
    this->m_type = PDFObject::Types::kNUMBER_INT;
}

PDFNumber::PDFNumber(double realValue) : m_value(realValue) {
    std::cout << __FUNCSIG__ << " = " << std::get<double>(m_value) << std::endl;

    this->m_type = PDFObject::Types::kNUMBER_REAL;
}

PDFNumber::~PDFNumber() {
    if (this->m_type == PDFObject::Types::kNUMBER_REAL)
        std::cout << __FUNCSIG__ << " = " << std::get<double>(m_value) << std::endl;
    else if (this->m_type == PDFObject::Types::kNUMBER_INT)
        std::cout << __FUNCSIG__ << " = " << std::get<int64_t>(m_value) << std::endl;
    else
        std::cout << __FUNCSIG__ << " !!! (n/a) !!!" << std::endl;
}

size_t PDFNumber::getLength() const {
    return 1;
}
