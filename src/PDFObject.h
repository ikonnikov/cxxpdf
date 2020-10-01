// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <stdint.h>
#include <memory>
#include <iostream>  // todo: delete it

class PDFObject {
 public:
    enum class Types {
        kNA = 0,
        kBOOLEAN,
        kNUMBER_INT,
        kNUMBER_REAL,
        kSTRING,
        kNAME,
        kARRAY,
        kDICTIONARY,
        kSTREAM,
        kNULL,
        kINDIRECT,
        kLITERAL
    };

    PDFObject();
    virtual ~PDFObject() noexcept;

    virtual size_t getLength() const = 0;
    virtual PDFObject::Types getType() const;

 protected:
    PDFObject::Types m_type;
};
