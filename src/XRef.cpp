// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "XRef.h"
#include "PDFObject.h"
#include "PDFNumber.h"

XRef::XRef(TokeniserPtr& documentTokeniser) {
    read_xref(documentTokeniser);
}

XRef::~XRef() noexcept {
    // dtor
}

void XRef::read_xref(TokeniserPtr& documentTokeniser) {
    std::unique_lock<std::recursive_mutex> localLocker(m_mutex);

    std::streamoff startXrefPos = documentTokeniser->getStartXref(true);  // find startxref location and position there
    if (startXrefPos == std::streamoff(-1))
        throw std::exception("Failed to read a xref (startxref position don't found)");

    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();  // read a startxref-token

        if (!(tokenType == Tokeniser::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "startxref"))
            throw std::exception("Failed to read a xref (startxref don't found)");
    }

    std::int64_t lastXrefPos = -1;
    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();  // read a numeric offset to a first located cross-reference table

        if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
            throw std::exception("Failed to read a xref (startxref is not followed by a integer number)");

        lastXrefPos = std::any_cast<std::int64_t>(tokenValue);
    }

    if (lastXrefPos == -1)
        return;

    documentTokeniser->seek(lastXrefPos);
    if (!read_xref_stream(documentTokeniser))  // try to read a cross-reference stream (maybe a table)
        return;

    int a = 1;  // todo: fuck it
}

bool XRef::read_xref_stream(TokeniserPtr& documentTokeniser) {
    /* exmple:
    * 11 0 obj % 11 = number, 0 = generation
    * .....
    * endobj
    */
    std::int64_t xrefObjectNumber = -1;
    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();  // read begin of cross-reference stream - Number of object

        if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
            return false;  // not stream! Looks like a cross-reference table

        xrefObjectNumber = std::any_cast<std::int64_t>(tokenValue);
    }

    if (xrefObjectNumber == -1)
        return false;

    std::int64_t xrefGenNumber = -1;
    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();  // read a generation of object

        if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
            return false;

        xrefGenNumber = std::any_cast<std::int64_t>(tokenValue);
    }

    if (xrefGenNumber == -1)
        return false;

    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();  // read a obj-token

        if (!(tokenType == Tokeniser::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "obj"))
            return false;
    }

    PDFObjectPtr pdfObject = read_object(documentTokeniser);

    // todo: read a stream too

    return false;  // todo
}

bool XRef::read_xref_section(TokeniserPtr& documentTokeniser) {
    // m_documentTokeniser
    return false;  // todo: impl it
}

PDFObjectPtr XRef::read_object(TokeniserPtr& documentTokeniser) {
    auto [tokenValue, tokenType] = documentTokeniser->nextValidToken();

    PDFObjectPtr pdfObject(nullptr);

    switch (tokenType) {
    case Tokeniser::Types::kTK_START_DIC:
        pdfObject = read_dictionary(documentTokeniser);
        break;

    case Tokeniser::Types::kTK_START_ARRAY:
        pdfObject = read_array(documentTokeniser);
        break;

    case Tokeniser::Types::kTK_NUMBER_INT:
        pdfObject = std::make_shared<PDFNumber>(std::any_cast<std::int64_t>(tokenValue));
        break;

    case Tokeniser::Types::kTK_NUMBER_REAL:
        pdfObject = std::make_shared<PDFNumber>(std::any_cast<double>(tokenValue));
        break;

    case Tokeniser::Types::kTK_STRING:
        pdfObject = std::make_shared<PDFString>(std::any_cast<std::string>(tokenValue), false);
        break;
    
    case Tokeniser::Types::kTK_STRING_HEX:
        pdfObject = std::make_shared<PDFString>(std::any_cast<std::string>(tokenValue), true);
        break;

    case Tokeniser::Types::kTK_NAME:
        pdfObject = std::make_shared<PDFName>(std::any_cast<std::string>(tokenValue));
        break;

    case Tokeniser::Types::kTK_REF:
        pdfObject = std::make_shared<PDFIndirectReference>(std::any_cast<std::pair<std::int64_t, std::int64_t>>(tokenValue));
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

PDFDictionaryPtr XRef::read_dictionary(TokeniserPtr& documentTokeniser) {
    PDFDictionaryPtr pdfDictionary = std::make_shared<PDFDictionary>();
    std::streamoff streamBegPos(-1);

    while (true) {
        auto [tokenValue, tokenType] = documentTokeniser->nextValidToken();

        if (tokenType == Tokeniser::Types::kTK_COMMENT)
            continue;

        else if (tokenType == Tokeniser::Types::kTK_ENDOFFILE)
            break;

        else if (tokenType == Tokeniser::Types::kTK_END_DIC) {
            std::streamoff dictEndPos = documentTokeniser->tell();

            // let's look at the next token
            while (true) {
                auto [nextTokenValue, nextTokenType] = documentTokeniser->nextValidToken();

                if (nextTokenType == Tokeniser::Types::kTK_COMMENT)
                    continue;

                if (nextTokenType == Tokeniser::Types::kTK_OTHER)
                    if (std::any_cast<std::string>(nextTokenValue) == "stream")
                        streamBegPos = dictEndPos;

                break;
            }
            documentTokeniser->seek(dictEndPos);  // rewind to end of dict
            break;

        }
        else if (tokenType != Tokeniser::Types::kTK_NAME)
            throw std::exception("Failed to read a xref (dictionary key is not a name)");

        PDFObjectPtr pdfObject = read_object(documentTokeniser);

        if (pdfObject->getType() == PDFObject::Types::kLITERAL) {
            PDFLiteralPtr pdfLiteral = std::dynamic_pointer_cast<PDFLiteral>(pdfObject);

            if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_DIC)
                throw std::exception("Failed to read a xref (unexpected gt gt)");

            else if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_ARRAY)
                throw std::exception("Failed to read a xref (unexpected close bracket)");

            break;
        }

        pdfDictionary->put(std::make_shared<PDFName>(std::any_cast<std::string>(tokenValue)), pdfObject);
    }

    if (streamBegPos == -1)
        return pdfDictionary;

    // read dictionary stream
    // ...
    // stream
    // ...
    // endstream
    PDFStreamPtr pdfStream = std::make_shared<PDFStream>(pdfDictionary);
    std::streamoff streamPos(-1);

    while (true) {
        auto [tokenValue, tokenType] = documentTokeniser->nextValidToken();

        if (tokenType == Tokeniser::Types::kTK_COMMENT)
            continue;

        else if (tokenType != Tokeniser::Types::kTK_OTHER)
            break;

        if (std::any_cast<std::string>(tokenValue) == "endstream")
            break;

        if (std::any_cast<std::string>(tokenValue) == "stream") {
            documentTokeniser->skipWhitespaces();
            streamPos = documentTokeniser->tell();

            PDFObjectPtr lengthToken = pdfDictionary->get(std::make_shared<PDFName>("Length"));

            std::size_t length = 0;
            if (lengthToken->getType() == PDFObject::Types::kNUMBER_INT)
                length = std::get<std::int64_t>(std::dynamic_pointer_cast<PDFNumber>(lengthToken)->getValue());

            if (length == 0) {
                // todo: impl recalculate stream lenght
            }

            //pdfStream->setLength(length);

            std::string buffer(length, '\0');
            if (!documentTokeniser->readStream(buffer, length))
                throw std::exception("Failed to tokenize a document (start xref position didn't read)");

            pdfStream->setStreamBytes(buffer.c_str(), length);
        }
    }

    return pdfStream;
}

PDFArrayPtr XRef::read_array(TokeniserPtr& documentTokeniser) {
    PDFArrayPtr pdfArray = std::make_shared<PDFArray>();

    while (true) {
        PDFObjectPtr pdfObject = read_object(documentTokeniser);

        if (pdfObject->getType() == PDFObject::Types::kLITERAL) {
            PDFLiteralPtr pdfLiteral = std::dynamic_pointer_cast<PDFLiteral>(pdfObject);

            if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_DIC)
                throw std::exception("Failed to read a xref (unexpected >>)");

            break;
        }

        pdfArray->add(pdfObject);
    }

    return pdfArray;
}
