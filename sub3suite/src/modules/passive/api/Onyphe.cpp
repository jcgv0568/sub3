#include "Onyphe.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define SIMPLE_BEST_GEOLOC 0
#define SIMPLE_BEST_INETNUM 1
#define SIMPLE_BEST_THREATLIST 2
#define SIMPLE_CTL 3
#define SIMPLE_DATAMD5 4
#define SIMPLE_DATASCAN 5
#define SIMPLE_E_DATASHOT 6
#define SIMPLE_E_ONIONSCAN 7
#define SIMPLE_E_ONIONSHOT 8
#define SIMPLE_E_TOPSITE 9
#define SIMPLE_E_VULNSCAN 10
#define SIMPLE_E_WHOIS 11
#define SIMPLE_FORWARD 12
#define SIMPLE_GEOLOC 13
#define SIMPLE_INETNUM 14
#define SIMPLE_PASTRIES 15
#define SIMPLE_RESOLVER 16
#define SIMPLE_REVERSE 17
#define SIMPLE_SNIFFER 18
#define SIMPLE_SYNSCAN 19
#define SIMPLE_THREATLIST 20
#define SIMPLE_E_BEST_WHOIS 21
#define SUMMARY_DOMAIN 22
#define SUMMARY_HOSTNAME 23
#define SUMMARY_IP 24
#define USER 25

/*
 * alot not yet implemented...
 * 2500 queries per month for free-api
 */
Onyphe::Onyphe(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "Onyphe";

    if(args.outputRaw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Onyphe::replyFinishedRawJson);
    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Onyphe::replyFinishedSubdomainIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Onyphe::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Onyphe::replyFinishedIp);
    if(args.outputAsn)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Onyphe::replyFinishedAsn);
    ///
    /// getting api key...
    ///
    
    m_key = APIKEY.value("onyphe").toString();
    
}
Onyphe::~Onyphe(){
    delete manager;
}

void Onyphe::start(){
    QNetworkRequest request;
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Authorization", "apikey "+m_key.toUtf8());

    QUrl url;
    if(args.outputRaw){
        switch (args.rawOption) {
        case USER:
            url.setUrl("https://www.onyphe.io/api/v2/user");
            break;
        case SUMMARY_IP:
            url.setUrl("https://www.onyphe.io/api/v2/summary/ip/"+target);
            break;
        case SUMMARY_DOMAIN:
            url.setUrl("https://www.onyphe.io/api/v2/summary/domain/"+target);
            break;
        case SUMMARY_HOSTNAME:
            url.setUrl("https://www.onyphe.io/api/v2/summary/hostname/"+target);
            break;
        case SIMPLE_GEOLOC:
            url.setUrl("https://www.onyphe.io/api/v2/simple/geoloc/"+target);
            break;
        case SIMPLE_INETNUM:
            url.setUrl("https://www.onyphe.io/api/v2/simple/inetnum/"+target);
            break;
        case SIMPLE_PASTRIES:
            url.setUrl("https://www.onyphe.io/api/v2/simple/pastries/"+target);
            break;
        case SIMPLE_RESOLVER:
            url.setUrl("https://www.onyphe.io/api/v2/simple/resolver/"+target);
            break;
        case SIMPLE_SNIFFER:
            url.setUrl("https://www.onyphe.io/api/v2/simple/sniffer/"+target);
            break;
        case SIMPLE_SYNSCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/synscan/"+target);
            break;
        case SIMPLE_THREATLIST:
            url.setUrl("https://www.onyphe.io/api/v2/simple/threatlist/"+target);
            break;
        case SIMPLE_CTL:
            url.setUrl("https://www.onyphe.io/api/v2/simple/ctl/"+target);
            break;
        case SIMPLE_DATASCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/datascan/"+target);
            break;
        case SIMPLE_DATAMD5:
            url.setUrl("https://www.onyphe.io/api/v2/simple/datascan/datamd5/"+target);
            break;
        case SIMPLE_FORWARD:
            url.setUrl("https://www.onyphe.io/api/v2/simple/resolver/forward/"+target);
            break;
        case SIMPLE_REVERSE:
            url.setUrl("https://www.onyphe.io/api/v2/simple/resolver/reverse/"+target);
            break;
        case SIMPLE_E_DATASHOT:
            url.setUrl("https://www.onyphe.io/api/v2/simple/datashot/"+target);
            break;
        case SIMPLE_E_ONIONSHOT:
            url.setUrl("https://www.onyphe.io/api/v2/simple/onionshot/"+target);
            break;
        case SIMPLE_E_TOPSITE:
            url.setUrl("https://www.onyphe.io/api/v2/simple/topsite/"+target);
            break;
        case SIMPLE_E_VULNSCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/vulnscan/"+target);
            break;
        case SIMPLE_E_WHOIS:
            url.setUrl("https://www.onyphe.io/api/v2/simple/whois/"+target);
            break;
        case SIMPLE_E_ONIONSCAN:
            url.setUrl("https://www.onyphe.io/api/v2/simple/onionscan/"+target);
            break;
        case SIMPLE_E_BEST_WHOIS:
            url.setUrl("https://www.onyphe.io/api/v2/simple/whois/best/"+target);
            break;
        case SIMPLE_BEST_GEOLOC:
            url.setUrl("https://www.onyphe.io/api/v2/simple/geoloc/best/"+target);
            break;
        case SIMPLE_BEST_INETNUM:
            url.setUrl("https://www.onyphe.io/api/v2/simple/inetnum/best/"+target);
            break;
        case SIMPLE_BEST_THREATLIST:
            url.setUrl("https://www.onyphe.io/api/v2/simple/threatlist/best/"+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args.inputDomain){
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Onyphe::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}

void Onyphe::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}

void Onyphe::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}

void Onyphe::replyFinishedAsn(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    end(reply);
}
