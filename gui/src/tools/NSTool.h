#ifndef NSTOOL_H
#define NSTOOL_H

#include <QDialog>
#include "src/models/NSModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"


namespace Ui {
class NSTool;
}

class NSTool : public QDialog{
        Q_OBJECT

    public:
        explicit NSTool(QWidget *parent = nullptr);
        ~NSTool();

    public slots:
        void onResultsNS(NSModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonStart_clicked();

    private:
        Ui::NSTool *ui;
        NSModel *m_model;
        ScanArgs *m_args = nullptr;
};

#endif // NSTOOL_H
