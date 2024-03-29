﻿// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"
#include "PDFArray.h"
#include "PDFNumber.h"
#include "PDFDictionary.h"
#include "PDFIndirectReference.h"

class PDFMutator {
 public:
     template<class _Ty1, class _Ty2>
     static const std::shared_ptr<_Ty1> xcast(const std::shared_ptr<_Ty2>&& _Other) noexcept {
         return std::make_shared<_Ty1>();
     };
};

template<>
inline const std::shared_ptr<PDFArray> PDFMutator::xcast(const std::shared_ptr<PDFObject>&& pdfObject) noexcept {
    if (pdfObject == nullptr)
        return std::make_shared<PDFArray>();

    if (pdfObject->isArray())
        return std::dynamic_pointer_cast<PDFArray>(pdfObject);

    return std::make_shared<PDFArray>();
}

template<>
inline const std::shared_ptr<PDFNumber> PDFMutator::xcast(const std::shared_ptr<PDFObject>&& pdfObject) noexcept {
    if (pdfObject == nullptr)
        return std::make_shared<PDFNumber>();

    if (pdfObject->isInteger() || pdfObject->isReal())
        return std::dynamic_pointer_cast<PDFNumber>(pdfObject);

    return std::make_shared<PDFNumber>();
}

template<>
inline const std::shared_ptr<PDFDictionary> PDFMutator::xcast(const std::shared_ptr<PDFObject>&& pdfObject) noexcept {
    if (pdfObject == nullptr)
        return std::make_shared<PDFDictionary>();

    if (pdfObject->isDictionary())
        return std::dynamic_pointer_cast<PDFDictionary>(pdfObject);

    return std::make_shared<PDFDictionary>();
}

template<>
inline const std::shared_ptr<PDFName> PDFMutator::xcast(const std::shared_ptr<PDFObject>&& pdfObject) noexcept {
    if (pdfObject == nullptr)
        return std::make_shared<PDFName>("");

    if (pdfObject->isName())
        return std::dynamic_pointer_cast<PDFName>(pdfObject);

    return std::make_shared<PDFName>("");
}

template<>
inline const std::shared_ptr<PDFIndirectReference> PDFMutator::xcast(const std::shared_ptr<PDFObject>&& pdfObject) noexcept {
    if (pdfObject == nullptr)
        return std::make_shared<PDFIndirectReference>();

    if (pdfObject->isIndirect())
        return std::dynamic_pointer_cast<PDFIndirectReference>(pdfObject);

    return std::make_shared<PDFIndirectReference>();
}
