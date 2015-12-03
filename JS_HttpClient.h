#ifndef JS_HttpClient_h
#define JS_HttpClient_h

#include <HttpClient.h>

class JS_HttpClient : public HttpClient
{
public:
    int isChunk;
    String header_line;
    
    JS_HttpClient(Client& aClient);
    
    int readHeader();
    int skipResponseHeaders();
};

#endif //JS_HttpClient_h