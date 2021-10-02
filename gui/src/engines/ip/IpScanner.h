#ifndef IPENUMERATOR_H
#define IPENUMERATOR_H

#include "../AbstractScanner.h"
#include <QHostInfo>

namespace ip{

struct ScanArguments{
    QListWidget *targetList;
    QStandardItemModel *model_results;
    //...
    int progress;
    int currentTargetToEnumerate;
};
typedef struct ScanArguments ScanArguments;


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(ScanConfig *scanConfig, ip::ScanArguments *scanArguments);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished(QHostInfo info);

    signals:
        void anotherLookup();
        void scanResult(QString subdomain, QString ipAddress);

    private:
        int m_currentTargetToEnumerate = 0;
        //...
        ScanConfig *m_scanConfig;
        ip::ScanArguments *m_scanArguments;
        //...
        QHostInfo *hostInfo;
};
}
#endif // IPENUMERATOR_H
