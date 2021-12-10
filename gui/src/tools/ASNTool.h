#ifndef ASNTOOL_H
#define ASNTOOL_H

#include <QDialog>
#include "src/models/AsnModel.h"
#include "src/modules/osint/OsintModulesHeaders.h"

namespace Ui {
class ASNTool;
}

class ASNTool : public QDialog{
        Q_OBJECT

    public:
        explicit ASNTool(QWidget *parent = nullptr);
        ~ASNTool();

    public slots:
        void onResultsAsn(AsModelStruct results);
        void onResultsAsnPeers(AsModelStruct results);
        void onResultsAsnPrefixes(AsModelStruct results);
        //...
        void onEnumerationComplete();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonStart_clicked();

    private:
        Ui::ASNTool *ui;
        AsModel *m_model;
        ScanArgs *m_args = nullptr;
};

#endif // ASNTOOL_H
