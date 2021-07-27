#ifndef DNS_H
#define DNS_H

// headers...
#include "src/engines/Base.h"
#include "lib-dns.h"
#include "DnsEnumerator.h"

namespace Ui {
    class Dns;
}

class Dns : public BaseClass{
    Q_OBJECT

    private:
        Ui::Dns *ui;

    private:
        void startScan();
        void loadSrvWordlist();
        //...
        ResultsModel *m_resultsModel;
        //...
        ScanStatus *m_scanStatus;
        ScanConfig *m_scanConfig;
        record::ScanArguments* m_scanArguments;
        record::ScanResults* m_scanResults;

    public:
        explicit Dns(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Dns();

    public slots:
        void scanThreadEnded();

    private slots:
        void on_pushButton_action_clicked();
        void on_pushButton_start_clicked();
        void on_pushButton_pause_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        void on_comboBox_option_currentIndexChanged(int index);
        //...
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_addTargets_clicked();
        void on_lineEdit_targets_returnPressed();
        //...
        void on_pushButton_removeSrvWordlist_clicked();
        void on_pushButton_clearSrvWordlist_clicked();
        void on_pushButton_loadSrvWordlist_clicked();
        void on_pushButton_addSrvWordlist_clicked();
        void on_lineEdit_srvWordlist_returnPressed();
        //...
        void on_pushButton_clearResults_clicked();
        void on_treeView_results_customContextMenuRequested(const QPoint &pos);
        void on_tableView_srv_customContextMenuRequested(const QPoint &pos);
};

#endif // DNS_H
