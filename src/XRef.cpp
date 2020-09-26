#include "XRef.h"

XRef::XRef(std::unique_ptr<Tokeniser>& documentTokeniser) {
    std::cout << u8"this is a ctor::XRef" << std::endl;  // todo: cout delete it

    read_xref(documentTokeniser);
}

XRef::~XRef() noexcept {
    std::cout << u8"this is a dtor::XRef" << std::endl;  // todo: cout delete it
}

void XRef::read_xref(std::unique_ptr<Tokeniser>& documentTokeniser) {
    std::unique_lock<std::recursive_mutex> localLocker(m_mutex);

    std::streamoff xrefPos = documentTokeniser->getStartXref(true);
    if (xrefPos == std::streamoff(-1))
        throw std::exception("Failed to read a xref (start xref position don't found)");

    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();

        if (!(tokenType == Tokeniser::TokenTypes::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "startxref"))
            throw std::exception("Failed to read a xref (startxref don't found)");
    }

    int64_t lastXrefPos = -1;
    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();

        if (tokenType != Tokeniser::TokenTypes::kTK_NUMBER_INT)
            throw std::exception("Failed to read a xref (startxref is not followed by a integer number");

        lastXrefPos = std::any_cast<int64_t>(tokenValue);
    }

    if (lastXrefPos == -1)
        return;

    documentTokeniser->seek(lastXrefPos);
    if (!read_xref_stream(documentTokeniser))
        return;

    int a = 1;  // todo: fuck it

    /*
    if (tokenType == Tokeniser::TokenTypes::kTK_STRING || tokenType == Tokeniser::TokenTypes::kTK_OTHER)  // todo: cout delete it
        std::cout << tokenValue.type().name() << ": " << std::any_cast<std::string>(tokenValue) << std::endl;  // todo: cout delete it
    else  // todo: cout delete it
        std::cout << ": N/A" << std::endl;  // todo: cout delete it
    */
}

bool XRef::read_xref_stream(std::unique_ptr<Tokeniser>& documentTokeniser) {
    int64_t thisStream = -1;
    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();

        if (tokenType != Tokeniser::TokenTypes::kTK_NUMBER_INT)
            return false;

        thisStream = std::any_cast<int64_t>(tokenValue);
    }

    if (thisStream == -1)
        return false;

    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();
        
        if (tokenType != Tokeniser::TokenTypes::kTK_NUMBER_INT)
            return false;
    }

    {
        auto [tokenValue, tokenType] = documentTokeniser->nextToken();

        if (!(tokenType == Tokeniser::TokenTypes::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "obj"))
            return false;
    }

    //read_object();




    return false;  // todo
}