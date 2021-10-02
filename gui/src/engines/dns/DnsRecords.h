#ifndef DNSRECORDS_H
#define DNSRECORDS_H

#include "src/utils/utils.h"
#include "src/engines/AbstractEngine.h"
#include "DnsRecordsScanner.h"

namespace Ui {
    class DnsRecords;
}

class DnsRecords : public AbstractEngine{
    Q_OBJECT

    public:
        DnsRecords(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr);
        ~DnsRecords();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(records::Results);

    private slots:
        void onSaveResultsDnsRecords(CHOICE);
        void onSaveResultsDnsRecords(QItemSelectionModel*);
        void onCopyResultsDnsRecords(CHOICE);
        void onCopyResultsDnsRecords(QItemSelectionModel*);
        void onSaveResultsSrvRecords(CHOICE);
        void onSaveResultsSrvRecords(QItemSelectionModel*);
        void onCopyResultsSrvRecords(CHOICE);
        void onCopyResultsSrvRecords(QItemSelectionModel*);
        ///
        /// ....
        ///
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_tableViewSRV_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::DnsRecords *ui;
        records::ScanArguments* m_scanArguments;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        void clearResults();
        void loadSrvWordlist();
};

#endif // DNSRECORDS_H
