// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFNumber.h"

PDFNumber::PDFNumber(std::int64_t intValue) : m_value(intValue) {
    this->m_type = PDFObject::Types::kNUMBER_INT;
}

PDFNumber::PDFNumber(double realValue) : m_value(realValue) {
    this->m_type = PDFObject::Types::kNUMBER_REAL;
}

PDFNumber::PDFNumber() {
    this->m_type = PDFObject::Types::kNA;
}

PDFNumber::~PDFNumber() {
    // dtor
}

std::size_t PDFNumber::getLength() const {
    return 1;
}

const std::variant<std::int64_t, double> PDFNumber::getValue() const {
    return m_value;
}

std::int64_t PDFNumber::asInteger(std::int64_t defaultValue) const {
    if (!this->isInteger())
        return defaultValue;

    std::int64_t retValue = std::get<std::int64_t>(this->getValue());

    return retValue;
}

double PDFNumber::asDouble(double defaultValue) const {
    if (!this->isReal())
        return defaultValue;

    double retValue = std::get<double>(this->getValue());

    return retValue;
}
