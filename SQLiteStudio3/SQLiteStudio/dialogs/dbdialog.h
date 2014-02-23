#ifndef DBDIALOG_H
#define DBDIALOG_H

#include "db/db.h"
#include "db/dbpluginoption.h"
#include <QDialog>
#include <QList>
#include <QHash>
#include <QStringList>

class DbPlugin;
class QGridLayout;
struct DbPluginOption;

namespace Ui {
    class DbDialog;
}

// TODO do validation (name, path/url) when clicked "ok"

class DbDialog : public QDialog
{
    Q_OBJECT

    public:
        enum Mode
        {
            ADD,
            EDIT
        };

        DbDialog(Mode mode, QWidget *parent = 0);
        ~DbDialog();

        void setDb(Db* db);
        void setPermanent(bool perm);

        QString getPath();
        QString getName();
        QHash<QString,QVariant> collectOptions();
        bool isPermanent();

    protected:
        void changeEvent(QEvent *e);
        void showEvent(QShowEvent* e);

    private slots:
        void typeChanged(int index);
        void valueForNameGenerationChanged();
        void nameChanged();
        void browseForFile();
        void on_generateCheckBox_toggled(bool checked);
        void on_fileEdit_textChanged(const QString &arg1);
        void on_browseLocalButton_clicked();
        void on_browseRemoteButton_clicked();
        void on_testConnButton_clicked();
        void propertyChanged();
        void on_typeCombo_activated(int index);
        void on_nameEdit_textChanged(const QString &arg1);

    private:
        void init();
        void updateOptions();
        void addOption(const DbPluginOption& option, int row);
        QWidget* getEditor(DbPluginOption::Type type, QWidget *&editorHelper);
        QVariant getValueFrom(DbPluginOption::Type type, QWidget* editor);
        void setValueFor(DbPluginOption::Type type, QWidget* editor, const QVariant& value);
        void updateType();
        Db* getDb();
        bool testDatabase();
        bool validate();
        void updateState();

        Ui::DbDialog *ui;
        Mode mode;
        QStringList existingDatabaseNames;
        Db* db = nullptr;
        QList<DbPlugin*> dbPlugins;
        QList<QWidget*> optionWidgets;
        QHash<QString,QWidget*> optionKeyToWidget;
        QHash<QString,DbPluginOption::Type> optionKeyToType;
        QHash<QWidget*,QString> helperToKey;
};

#endif // DBDIALOG_H