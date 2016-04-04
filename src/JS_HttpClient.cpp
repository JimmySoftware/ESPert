#include "JS_HttpClient.h"

JS_HttpClient::JS_HttpClient(Client& aClient)
 : HttpClient(aClient)
{
    isChunk = false;
}

int JS_HttpClient::skipResponseHeaders()
{
    isChunk = 0;
    header_line = "";
    
    //Serial.println( "You are at good place\n" );
    // Just keep reading until we finish reading the headers or time out
    unsigned long timeoutStart = millis();
    // Whilst we haven't timed out & haven't reached the end of the headers
    while ((!endOfHeadersReached()) && 
           ( (millis() - timeoutStart) < iHttpResponseTimeout ))
    {
        if (available())
        {
            
            (void)readHeader();
            // We read something, reset the timeout counter
            timeoutStart = millis();
        }
        else
        {
            // We haven't got any data, so let's pause to allow some to
            // arrive
            delay(kHttpWaitForDataDelay);
        }
    }
    if (endOfHeadersReached())
    {
        // Success
        return HTTP_SUCCESS;
    }
    else
    {
        // We must've timed out
        return HTTP_ERROR_TIMED_OUT;
    }
}


int JS_HttpClient::readHeader()
{
    char c = read();
    header_line = header_line + c;

    if (endOfHeadersReached())
    {
        // We've passed the headers, but rather than return an error, we'll just
        // act as a slightly less efficient version of read()
        return c;
    }

    // Whilst reading out the headers to whoever wants them, we'll keep an
    // eye out for the "Content-Length" header
    switch(iState)
    {
    case eStatusCodeRead:
        // We're at the start of a line, or somewhere in the middle of reading
        // the Content-Length prefix
        if (*iContentLengthPtr == c)
        {
            // This character matches, just move along
            iContentLengthPtr++;
            if (*iContentLengthPtr == '\0')
            {
                // We've reached the end of the prefix
                iState = eReadingContentLength;
                // Just in case we get multiple Content-Length headers, this
                // will ensure we just get the value of the last one
                iContentLength = 0;
            }
        }
        else if ((iContentLengthPtr == kContentLengthPrefix) && (c == '\r'))
        {
            // We've found a '\r' at the start of a line, so this is probably
            // the end of the headers
            iState = eLineStartingCRFound;
        }
        else
        {
            // This isn't the Content-Length header, skip to the end of the line
            iState = eSkipToEndOfHeader;
        }
        break;
    case eReadingContentLength:
        if (isdigit(c))
        {
            iContentLength = iContentLength*10 + (c - '0');
        }
        else
        {
            // We've reached the end of the content length
            // We could sanity check it here or double-check for "\r\n"
            // rather than anything else, but let's be lenient
            iState = eSkipToEndOfHeader;
        }
        break;
    case eLineStartingCRFound:
        if (c == '\n')
        {
            iState = eReadingBody;
        }
        break;
    default:
        // We're just waiting for the end of the line now
        
        break;
    };

    if ( (c == '\n') && !endOfHeadersReached() )
    {
        if( header_line.startsWith( "Transfer-Encoding: chunked") )
        {
            //Serial.println( "CHUNK!!!" );
            isChunk = 1;
        }
        //Serial.print( header_line );
        header_line = "";
        
        // We've got to the end of this line, start processing again
        iState = eStatusCodeRead;
        iContentLengthPtr = kContentLengthPrefix;
    }
    // And return the character read to whoever wants it
    return c;
}

