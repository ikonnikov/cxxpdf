// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "Tokeniser.hpp"
#include "PDFObject.h"
#include "PDFDictionary.h"
#include "PDFName.h"
#include "PDFString.h"
#include "PDFArray.h"
#include "PDFLiteral.h"
#include "PDFIndirectReference.h"
#include "PDFStream.h"
#include "PDFNumber.h"
#include "PDFMutator.hpp"

class XRef {
 private:
     struct XRefEntry {
         std::int64_t type;
         std::int64_t offset;
         std::int64_t gen;
         PDFObjectPtr pdfObject;
     };

 public:
    XRef() = delete;
    XRef(file_tokeniser& streamTokeniser);
    XRef(array_tokeniser& streamTokeniser);
    virtual ~XRef() noexcept;

 public:
     const PDFObjectPtr getPdfObject(std::int64_t xrefIndex) const;

 private:
    template<typename T>
    void readXRef(tokeniser_base* streamTokeniser);

    template<typename T>
    bool readXRefStream(tokeniser_base* streamTokeniser);

    template<typename T>
    void readPages();

    template<typename T>
    std::tuple<bool, std::int64_t, std::int64_t, PDFObjectPtr> readIndirectObject(tokeniser_base* streamTokeniser);

    template<typename T>
    PDFObjectPtr readObject(tokeniser_base* streamTokeniser);

    template<typename T>
    PDFDictionaryPtr readDictionary(tokeniser_base* streamTokeniser);

    template<typename T>
    PDFArrayPtr readArray(tokeniser_base* streamTokeniser);

    template<typename T>
    void readXRefObjects(tokeniser_base* streamTokeniser);

    template<typename T>
    bool readXRefSection(tokeniser_base* streamTokeniser);

 protected:
    mutable std::recursive_mutex m_mutex;
    
    std::map<std::int64_t, XRef::XRefEntry> m_xrefTable;  // key, value(type, offset, gen, object)
    PDFDictionaryPtr m_trailer;
};

template<typename T>
void XRef::readXRef(tokeniser_base* streamTokeniser) {
    std::unique_lock<std::recursive_mutex> localLocker(m_mutex);

    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    std::streamoff startXrefPos = pTokeniser->getStartXref(true);  // find startxref location and position there
    if (startXrefPos == std::streamoff(-1))
        throw std::exception("Failed to read a xref (startxref position don't found)");

    {
        auto [tokenValue, tokenType] = pTokeniser->nextToken();  // read a startxref-token

        if (!(tokenType == T::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "startxref"))
            throw std::exception("Failed to read a xref (startxref don't found)");
    }

    std::int64_t lastXrefPos = -1;
    {
        auto [tokenValue, tokenType] = pTokeniser->nextToken();  // read a numeric offset to a first located cross-reference table

        if (tokenType != T::Types::kTK_NUMBER_INT)
            throw std::exception("Failed to read a xref (startxref is not followed by a number)");

        lastXrefPos = std::any_cast<std::int64_t>(tokenValue);
    }

    if (lastXrefPos == -1)
        return;

    if (!readXRefStream<T>((tokeniser_base*)&pTokeniser->seek(lastXrefPos)))  // try to read a cross-reference stream (maybe a table)
        return;

    readXRefObjects<T>((tokeniser_base*)&pTokeniser->seek(0));
    readPages<T>();
}

template<typename T>
bool XRef::readXRefSection(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    // m_documentTokeniser
    return false;  // todo: impl it
}

template<typename T>
void XRef::readXRefObjects(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    for (auto& xrefItem : m_xrefTable) {
        std::int64_t offsetObj = xrefItem.second.offset;  // std::get<0>(xrefItem.second);
        std::int64_t field2 = xrefItem.second.gen;  // std::get<1>(xrefItem.second);
        PDFObjectPtr& pdfObject = xrefItem.second.pdfObject;  // std::get<2>(xrefItem.second);

        if (offsetObj <= 0 || field2 > 0)
            continue;

        auto [isReaded, xrefObjectNumber, xrefGenNumber, pdfObjectItem] = readIndirectObject<T>((tokeniser_base*)&pTokeniser->seek(offsetObj));

        if (!isReaded)
            continue;

        pdfObject = pdfObjectItem;
    }
}

template<typename T>
bool XRef::readXRefStream(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    auto [isReaded, xrefObjectNumber, xrefGenNumber, pdfObject] = readIndirectObject<T>(streamTokeniser);

    return false;
}

template<typename T>
void XRef::readPages() {
    if (m_trailer == nullptr)
        return;

    const PDFIndirectReferencePtr objectRoot = PDFMutator::xcast<PDFIndirectReference>(m_trailer->get("Root"));
    const PDFDictionaryPtr pdfCatalog = PDFMutator::xcast<PDFDictionary>(getPdfObject(objectRoot->getNumber()));

    const PDFIndirectReferencePtr objectPages = PDFMutator::xcast<PDFIndirectReference>(pdfCatalog->get("Pages"));
    //const PDFDictionaryPtr objectCatalog = PDFMutator::xcast<PDFDictionary>(getPdfObject(objectPages->getNumber()));
    //АХТУНГ по индексу 9 лежит Pages - его нужно начитать в m_xrefTable (как-то не явно начитывается, нужно вернуться к readXRefObjects())
    //const PDFDictionaryPtr pdfPages = PDFMutator::xcast<PDFDictionary>(objectCatalog->get("Pages"));

    auto qqq = 1;
}

template<typename T>
std::tuple<bool, std::int64_t, std::int64_t, PDFObjectPtr> XRef::readIndirectObject(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    std::int64_t xrefObjectNumber = -1;
    {
        auto [tokenValue, tokenType] = pTokeniser->nextToken();  // read begin of cross-reference stream - Number of object

        if (tokenType != T::Types::kTK_NUMBER_INT)
            return std::make_tuple(false, -1, -1, nullptr);  // not stream! Looks like a cross-reference table

        xrefObjectNumber = std::any_cast<std::int64_t>(tokenValue);
    }

    if (xrefObjectNumber == -1)
        return std::make_tuple(false, -1, -1, nullptr);

    std::int64_t xrefGenNumber = -1;
    {
        auto [tokenValue, tokenType] = pTokeniser->nextToken();  // read a generation of object

        if (tokenType != T::Types::kTK_NUMBER_INT)
            return std::make_tuple(false, -1, -1, nullptr);

        xrefGenNumber = std::any_cast<std::int64_t>(tokenValue);
    }

    if (xrefGenNumber == -1)
        return std::make_tuple(false, -1, -1, nullptr);

    {
        auto [tokenValue, tokenType] = pTokeniser->nextToken();  // read a obj-token

        if (!(tokenType == T::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "obj"))
            return std::make_tuple(false, -1, -1, nullptr);
    }

    PDFObjectPtr pdfObject = readObject<T>(pTokeniser);

    return std::make_tuple(true, xrefObjectNumber, xrefGenNumber, pdfObject);
}

template<typename T>
PDFObjectPtr XRef::readObject(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    auto [tokenValue, tokenType] = pTokeniser->nextValidToken();

    PDFObjectPtr pdfObject(nullptr);

    switch (tokenType) {
    case T::Types::kTK_START_DIC:
        pdfObject = readDictionary<T>(pTokeniser);
        break;

    case T::Types::kTK_START_ARRAY:
        pdfObject = readArray<T>(pTokeniser);
        break;

    case T::Types::kTK_NUMBER_INT:
        pdfObject = std::make_shared<PDFNumber>(std::any_cast<std::int64_t>(tokenValue));
        break;

    case T::Types::kTK_NUMBER_REAL:
        pdfObject = std::make_shared<PDFNumber>(std::any_cast<double>(tokenValue));
        break;

    case T::Types::kTK_STRING:
        pdfObject = std::make_shared<PDFString>(std::any_cast<std::string>(tokenValue), false);
        break;

    case T::Types::kTK_STRING_HEX:
        pdfObject = std::make_shared<PDFString>(std::any_cast<std::string>(tokenValue), true);
        break;

    case T::Types::kTK_NAME:
        pdfObject = std::make_shared<PDFName>(std::any_cast<std::string>(tokenValue));
        break;

    case T::Types::kTK_REF:
        pdfObject = std::make_shared<PDFIndirectReference>(std::any_cast<std::pair<std::int64_t, std::int64_t>>(tokenValue));
        break;

    case T::Types::kTK_END_ARRAY:
        pdfObject = std::make_shared<PDFLiteral>(PDFLiteral::Types::kLIT_END_ARRAY);
        break;

    case T::Types::kTK_END_DIC:
        pdfObject = std::make_shared<PDFLiteral>(PDFLiteral::Types::kLIT_END_DIC);
        break;

    case T::Types::kTK_ENDOFFILE:
        // todo: impl it
        break;

    default:
        break;
    }

    return pdfObject;
}

template<typename T>
PDFDictionaryPtr XRef::readDictionary(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    PDFDictionaryPtr pdfDictionary = std::make_shared<PDFDictionary>();
    std::streamoff streamBegPos(-1);

    while (true) {
        auto [tokenValue, tokenType] = pTokeniser->nextValidToken();

        if (tokenType == T::Types::kTK_COMMENT) {
            continue;

        }
        else if (tokenType == T::Types::kTK_ENDOFFILE) {
            break;

        }
        else if (tokenType == T::Types::kTK_END_DIC) {
            std::streamoff dictEndPos = pTokeniser->tell();

            // let's look at the next token
            while (true) {
                auto [nextTokenValue, nextTokenType] = pTokeniser->nextValidToken();

                if (nextTokenType == T::Types::kTK_COMMENT)
                    continue;

                if (nextTokenType == T::Types::kTK_OTHER)
                    if (std::any_cast<std::string>(nextTokenValue) == "stream")
                        streamBegPos = dictEndPos;

                break;
            }
            pTokeniser->seek(dictEndPos);  // rewind to end of dict
            break;

        }
        else if (tokenType != T::Types::kTK_NAME) {
            throw std::exception("Failed to read a xref (dictionary key is not a name)");
        }

        PDFObjectPtr pdfObject = readObject<T>(pTokeniser);

        if (pdfObject->isLiteral()) {
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
        auto [tokenValue, tokenType] = pTokeniser->nextValidToken();

        if (tokenType == T::Types::kTK_COMMENT)
            continue;

        else if (tokenType != T::Types::kTK_OTHER)
            break;

        if (std::any_cast<std::string>(tokenValue) == "endstream")
            break;

        if (std::any_cast<std::string>(tokenValue) == "stream") {
            pTokeniser->skipWhitespaces();
            streamPos = pTokeniser->tell();

            std::size_t length = PDFMutator::xcast<PDFNumber>(pdfDictionary->get("Length"))->asInteger(0);

            if (length == 0) {
                // todo: impl recalculate stream lenght
            }

            std::string buffer(length, '\0');
            if (!pTokeniser->readStream(buffer, length))
                throw std::exception("Failed to tokenize a document (start xref position didn't read)");

            pdfStream->setRawStreamBytes(buffer.c_str(), length);
        }
    }

    return pdfStream;
}

template<typename T>
PDFArrayPtr XRef::readArray(tokeniser_base* streamTokeniser) {
    auto pTokeniser = dynamic_cast<T*>(streamTokeniser);

    PDFArrayPtr pdfArray = std::make_shared<PDFArray>();

    while (true) {
        PDFObjectPtr pdfObject = readObject<T>(pTokeniser);

        if (pdfObject->isLiteral()) {
            PDFLiteralPtr pdfLiteral = std::dynamic_pointer_cast<PDFLiteral>(pdfObject);

            if (pdfLiteral->getLiteralType() == PDFLiteral::Types::kLIT_END_DIC)
                throw std::exception("Failed to read a xref (unexpected >>)");

            break;
        }

        pdfArray->add(pdfObject);
    }

    return pdfArray;
}
