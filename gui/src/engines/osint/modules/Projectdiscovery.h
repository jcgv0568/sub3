#ifndef PROJECTDISCOVERY_H
#define PROJECTDISCOVERY_H

#include "AbstractModule.h"

class Projectdiscovery: public AbstractModule{

    public:
        Projectdiscovery(QString target = nullptr, QObject *parent = nullptr);
        ~Projectdiscovery() override;

    public slots:
        void start() override;
        void replyFinished(QNetworkReply *) override;
};

#endif // PROJECTDISCOVERY_H
