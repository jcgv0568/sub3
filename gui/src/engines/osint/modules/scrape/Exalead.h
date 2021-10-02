#ifndef EXALEAD_H
#define EXALEAD_H

#include "src/engines/osint/modules/AbstractOsintModule.h"
#include "gumbo-parser/src/gumbo.h"

class Exalead: public AbstractOsintModule{

    public:
        Exalead(QString target = nullptr, QObject *parent = nullptr);
        ~Exalead() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;

    private:
        int m_page = 0;
        int activeRequests = 0;
        void getLinks(GumboNode *node);
        GumboNode *getBody(GumboNode *node);
};
#endif // EXALEAD_H
