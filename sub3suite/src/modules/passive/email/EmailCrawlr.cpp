#include "EmailCrawlr.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define ACCOUNT 0
#define DOMAIN_LOOKUP 1
#define EMAIL_LOOKUP 2
#define EMAIL_VERIFICATION 3

/*
 * returns a BadGateway Error
 */
EmailCrawlr::EmailCrawlr(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_EMAILCRAWLR;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &EmailCrawlr::replyFinishedRawJson);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &EmailCrawlr::replyFinishedEmail);

    /* getting api-key */
    m_key = APIKEY.value(OSINT_MODULE_EMAILCRAWLR).toString();
}
EmailCrawlr::~EmailCrawlr(){
    delete manager;
}

void EmailCrawlr::start(){
    this->checkAPIKey(m_key);

    QNetworkRequest request;
    request.setRawHeader("x-api-key", m_key.toUtf8());
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case ACCOUNT:
            url.setUrl("https://api.emailcrawlr.com/v2/account");
            break;
        case DOMAIN_LOOKUP:
            url.setUrl("https://api.emailcrawlr.com/v2/domain?domain="+target);
            break;
        case EMAIL_LOOKUP:
            url.setUrl("https://api.emailcrawlr.com/v2/email?email="+target);
            break;
        case EMAIL_VERIFICATION:
            url.setUrl("https://api.emailcrawlr.com/v2/email/verify?email="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_Domain){
        if(args.output_Email){
            url.setUrl("https://api.emailcrawlr.com/v2/domain?domain="+target);
            request.setUrl(url);
            manager->get(request);
        }
    }
}

void EmailCrawlr::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray emails = document.object()["emails"].toArray();

    foreach(const QJsonValue &value, emails){
        QString mail = value.toObject()["email"].toString();
        emit resultEmail(mail);
        log.resultsCount++;
    }

    this->end(reply);
}
