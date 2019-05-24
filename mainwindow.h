#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnAddFiles_clicked();
    void on_btnRemovesFiles_clicked();
    void on_btnBrowseOutputPath_clicked();
    void on_btnCompressFiles_clicked();

    void subProcessStatusChanged(QProcess::ProcessState state);
    void subProcessExitStatus(int exitCode, QProcess::ExitStatus exitStatus);
    void read7zaStandardOutput();

private:
    Ui::MainWindow *ui;
    QStringList inputFilesList;
    QString outputFilePath;
    QProcess subProcess;
    QString zipOutPutMessage;
};

#endif // MAINWINDOW_H
