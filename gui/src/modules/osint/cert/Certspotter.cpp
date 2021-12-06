#include "Certspotter.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ISSUEANCES 0


Certspotter::Certspotter(ScanArgs *args) : AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "CertSpotter";

    if(args->outputRaw)
        connect(manager, &NetworkAccessManager::finished, this, &Certspotter::replyFinishedRawJson);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Certspotter::replyFinishedSubdomain);
    if(args->outputSSLCert)
        connect(manager, &NetworkAccessManager::finished, this, &Certspotter::replyFinishedSSLCert);
    ///
    /// getting api key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("certspotter").toString();
    Config::generalConfig().endGroup();
}
Certspotter::~Certspotter(){
    delete manager;
}

void Certspotter::start(){
    QNetworkRequest request;
    request.setRawHeader("Authorization", "Bearer "+m_key.toUtf8());

    QUrl url;
    if(args->outputRaw){
        switch (args->rawOption) {
        case ISSUEANCES:
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&include_subdomains=true&expand=cert&expand=issuer&expand=dns_names");
            break;
        }
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
        return;
    }

    if(args->inputDomain){
        if(args->outputSubdomain){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&include_subdomains=true&expand=dns_names");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args->outputSSLCert){
            url.setUrl("https://api.certspotter.com/v1/issuances?domain="+args->target+"&include_subdomains=true&expand=cert");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Certspotter::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonArray dns_names = value.toObject()["dns_names"].toArray();
        foreach(const QJsonValue &domain, dns_names){
            emit subdomain(domain.toString());
            log.resultsCount++;
        }
    }

    end(reply);
}

void Certspotter::replyFinishedSSLCert(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    foreach(const QJsonValue &value, document.array())
    {
        QJsonObject cert = value.toObject()["cert"].toObject();
        QString sha256 = cert["sha256"].toString();
        /* ... */
        emit sslCert(sha256);
        log.resultsCount++;
    }

    end(reply);
}

