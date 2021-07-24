#ifndef ACTIVEENUMERATOR_H
#define ACTIVEENUMERATOR_H

#include "lib-active.h"

/*************************************************************************************
                              ACTIVE_SUBDOMAINS ENUMERATOR
*************************************************************************************/
class ActiveEnumerator : public QObject{
    Q_OBJECT

    private:
        int m_currentTargetToEnumerate = 0;
        //...
        ScanConfig *m_scanConfig;
        active::ScanArguments *m_scanArguments;
        //...
        QDnsLookup *m_dns;
        QTcpSocket *m_socket;

    public:
        ActiveEnumerator(ScanConfig *scanConfig, active::ScanArguments *scanArguments);
        ~ActiveEnumerator();
        //...
        void enumerate(QThread *cThread);

    public slots:
        void onStop();

    private slots:
        void lookup();
        void lookupFinished();

    signals:
        void performAnotherLookup();
        //...
        void progress(int value);
        void scanResult(QString subdomain, QString ipAddress);
        void scanLog(QString log);
        //...
        void quitThread();
};
#endif // ACTIVEENUMERATOR_H