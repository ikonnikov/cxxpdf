// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFObject.h"

PDFObject::PDFObject() : m_type(PDFObject::Types::kNA) {
    std::cout << __FUNCSIG__ << std::endl;
}

PDFObject::~PDFObject() {
    std::cout << __FUNCSIG__ << std::endl;
}

PDFObject::Types PDFObject::getType() const {
    return m_type;
}