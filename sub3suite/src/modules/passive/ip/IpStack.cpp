#include "IpStack.h"
#include "src/utils/Config.h"

#define BULK_IP_LOOKUP 0
#define REQUESTER_IP_LOOKUP 1
#define STANDARD_IP_LOOKUP 2

/*
 * 100 per month for free-user, uses http instead of https for free users...
 */
IpStack::IpStack(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "IpStack";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &IpStack::replyFinishedRawJson);
    ///
    /// get api key...
    ///
    
    m_key = APIKEY.value("ipstack").toString();
    
}
IpStack::~IpStack(){
    delete manager;
}

void IpStack::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case STANDARD_IP_LOOKUP:
            url.setUrl("http://api.ipstack.com/"+target+"?access_key="+m_key);
            break;
        case BULK_IP_LOOKUP:
            url.setUrl("http://api.ipstack.com/"+target+"?access_key="+m_key);
            break;
        case REQUESTER_IP_LOOKUP:
            url.setUrl("http://api.ipstack.com/check?access_key="+m_key);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }
}
