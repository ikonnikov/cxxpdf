// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "XRef.h"
#include "PDFObject.h"
#include "PDFNumber.h"

XRef::XRef(std::unique_ptr<Tokeniser>& documentTokeniser) : m_documentTokeniser(documentTokeniser) {
    read_xref();
}

XRef::~XRef() noexcept {
    // dtor
}

void XRef::read_xref() {
    std::unique_lock<std::recursive_mutex> localLocker(m_mutex);

    std::streamoff startXrefPos = m_documentTokeniser->getStartXref(true);  // find startxref location and position there
    if (startXrefPos == std::streamoff(-1))
        throw std::exception("Failed to read a xref (startxref position don't found)");

    {
        auto [tokenValue, tokenType] = m_documentTokeniser->nextToken();  // read a startxref-token

        if (!(tokenType == Tokeniser::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "startxref"))
            throw std::exception("Failed to read a xref (startxref don't found)");
    }

    int64_t lastXrefPos = -1;
    {
        auto [tokenValue, tokenType] = m_documentTokeniser->nextToken();  // read a numeric offset to a first located cross-reference table

        if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
            throw std::exception("Failed to read a xref (startxref is not followed by a integer number");

        lastXrefPos = std::any_cast<int64_t>(tokenValue);
    }

    if (lastXrefPos == -1)
        return;

    m_documentTokeniser->seek(lastXrefPos);
    if (!read_xref_stream())  // try to read a cross-reference stream (maybe a table)
        return;

    int a = 1;  // todo: fuck it
}

bool XRef::read_xref_stream() {
    /* exmple:
    * 11 0 obj % 11 = number, 0 = generation
    * .....
    * endobj
    */
    int64_t xrefObjectNumber = -1;
    {
        auto [tokenValue, tokenType] = m_documentTokeniser->nextToken();  // read begin of cross-reference stream - Number of object

        if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
            return false;  // not stream! Looks like a cross-reference table

        xrefObjectNumber = std::any_cast<int64_t>(tokenValue);
    }

    if (xrefObjectNumber == -1)
        return false;

    int64_t xrefGenNumber = -1;
    {
        auto [tokenValue, tokenType] = m_documentTokeniser->nextToken();  // read a generation of object

        if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
            return false;

        xrefGenNumber = std::any_cast<int64_t>(tokenValue);
    }

    if (xrefGenNumber == -1)
        return false;

    {
        auto [tokenValue, tokenType] = m_documentTokeniser->nextToken();  // read a obj-token

        if (!(tokenType == Tokeniser::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "obj"))
            return false;
    }

    std::shared_ptr<PDFObject> pdfObject = read_object();

    // todo: read a stream too

    return false;  // todo
}

bool XRef::read_xref_section() {
    // m_documentTokeniser
    return false;  // todo: impl it
}

std::shared_ptr<PDFObject> XRef::read_object() {
    auto [tokenValue, tokenType] = m_documentTokeniser->nextValidToken();

    std::shared_ptr<PDFObject> pdfObject(nullptr);

    switch (tokenType) {
    case Tokeniser::Types::kTK_START_DIC:
        pdfObject = read_dictionary();
        //} ¿’“”Õ√
        break;

    case Tokeniser::Types::kTK_START_ARRAY:
        pdfObject = read_array();
        break;

    case Tokeniser::Types::kTK_NUMBER_INT:
        pdfObject = std::make_shared<PDFNumber>(std::any_cast<int64_t>(tokenValue));
        break;

    case Tokeniser::Types::kTK_NUMBER_REAL:
        pdfObject = std::make_shared<PDFNumber>(std::any_cast<double>(tokenValue));
        break;

    case Tokeniser::Types::kTK_STRING:
        pdfObject = std::make_shared<PDFString>(std::any_cast<std::string>(tokenValue));
        break;

    case Tokeniser::Types::kTK_NAME:
        pdfObject = std::make_shared<PDFName>(std::any_cast<std::string>(tokenValue));
        break;

    case Tokeniser::Types::kTK_REF:
        pdfObject = std::make_shared<PDFIndirectReference>(std::any_cast<std::pair<int64_t, int64_t>>(tokenValue));
        break;

    case Tokeniser::Types::kTK_END_ARRAY:
        pdfObject = std::make_shared<PDFLiteral>(PDFLiteral::Types::kLIT_END_ARRAY);
        break;

    case Tokeniser::Types::kTK_END_DIC:
        pdfObject = std::make_shared<PDFLiteral>(PDFLiteral::Types::kLIT_END_DIC);
        break;

    case Tokeniser::Types::kTK_ENDOFFILE:
        // todo: impl it
        break;

    default:
        break;
    }

    return pdfObject;
}

std::shared_ptr<PDFDictionary> XRef::read_dictionary() {
    std::shared_ptr<PDFDictionary> pdfDictionary = std::make_shared<PDFDictionary>();

    while (true) {
        auto [tokenValue, tokenType] = m_documentTokeniser->nextValidToken();

        if (tokenType == Tokeniser::Types::kTK_ENDOFFILE)
            break;
        else if (tokenType == Tokeniser::Types::kTK_END_DIC)
            break;
        else if (tokenType != Tokeniser::Types::kTK_NAME)
            throw std::exception("Failed to read a xref (dictionary key is not a name");

        std::shared_ptr<PDFObject> pdfObject = read_object();

        if (pdfObject->getType() == PDFObject::Types::kLITERAL) {
            PDFLiteral* pdfLiteral = dynamic_cast<PDFLiteral*>(pdfObject.get());

            if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_DIC)
                throw std::exception("Failed to read a xref (unexpected gt gt");

            if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_ARRAY)
                throw std::exception("Failed to read a xref (unexpected close bracket");

            break;
        }

        pdfDictionary->put(std::make_shared<PDFName>(std::any_cast<std::string>(tokenValue)), pdfObject);
    }

    return pdfDictionary;
}

std::shared_ptr<PDFArray> XRef::read_array() {
    std::shared_ptr<PDFArray> pdfArray = std::make_shared<PDFArray>();

    while (true) {
        std::shared_ptr<PDFObject> pdfObject = read_object();

        if (pdfObject->getType() == PDFObject::Types::kLITERAL) {
            PDFLiteral* pdfLiteral = dynamic_cast<PDFLiteral*>(pdfObject.get());

            if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_DIC)
                throw std::exception("Failed to read a xref (unexpected gt gt");

            break;
        }

        pdfArray->add(pdfObject);
    }

    return pdfArray;
}
