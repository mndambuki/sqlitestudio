#ifndef FUNCTIONSEDITOR_H
#define FUNCTIONSEDITOR_H

#include "mdichild.h"
#include "common/extactioncontainer.h"
#include "config.h"
#include <QItemSelection>
#include <QSortFilterProxyModel>

namespace Ui {
class FunctionsEditor;
}

class FunctionsEditorModel;
class SqlFunctionPlugin;
class SyntaxHighlighterPlugin;
class DbTreeItem;
class QTreeWidgetItem;
class QSyntaxHighlighter;

class FunctionsEditor : public MdiChild, public ExtActionContainer
{
    Q_OBJECT

    public:
        enum Action
        {
            COMMIT,
            ROLLBACK,
            ADD,
            DELETE,
            ARG_ADD,
            ARG_EDIT,
            ARG_DEL,
            ARG_MOVE_UP,
            ARG_MOVE_DOWN
        };

        explicit FunctionsEditor(QWidget *parent = 0);
        ~FunctionsEditor();

        bool restoreSessionNextTime();

    protected:
        QVariant saveSession();
        bool restoreSession(const QVariant &sessionValue);
        QString getIconNameForMdiWindow();
        QString getTitleForMdiWindow();
        void createActions();
        void setupDefShortcuts();

    private:
        class DbModel : public QSortFilterProxyModel
        {
            public:
                explicit DbModel(QObject *parent = 0);

                QVariant data(const QModelIndex& index, int role) const;
                bool setData(const QModelIndex& index, const QVariant& value, int role);
                Qt::ItemFlags flags(const QModelIndex& index) const;

                void setDatabases(const QStringList& databases);
                QStringList getDatabases() const;

            protected:
                bool filterAcceptsRow(int srcRow, const QModelIndex& srcParent) const;

            private:
                DbTreeItem* getItemForProxyIndex(const QModelIndex& index) const;

                QStringList checkedDatabases;
        };

        void init();
        QString getCurrentFunctionName() const;
        void functionDeselected(const QString& name);
        void functionSelected(const QString& name);
        void clearEdits();
        void selectFunction(const QString& name);
        QModelIndex getSelectedArg() const;
        QStringList getCurrentArgList() const;
        QStringList getCurrentDatabases() const;

        Ui::FunctionsEditor *ui;
        FunctionsEditorModel* model;
        bool currentModified = false;
        QHash<QString,SqlFunctionPlugin*> functionPlugins;
        QHash<QString,SyntaxHighlighterPlugin*> highlighterPlugins;
        DbModel* dbListModel;
        QString currentHighlighterLang;
        QSyntaxHighlighter* currentHighlighter = nullptr;

    private slots:
        void commit();
        void rollback();
        void newFunction();
        void deleteFunction();
        void updateModified();
        void updateState();
        void updateCurrentFunctionState();
        void functionSelected(const QItemSelection& selected, const QItemSelection& deselected);
        void validateName();
        void addFunctionArg();
        void editFunctionArg();
        void delFunctionArg();
        void moveFunctionArgUp();
        void moveFunctionArgDown();
        void updateArgsState();
};

#endif // FUNCTIONSEDITOR_H