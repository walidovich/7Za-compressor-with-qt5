#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connecting signal and slots
    connect(&subProcess, &QProcess::stateChanged, this, &MainWindow::subProcessStatusChanged);
    connect(&subProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            this, &MainWindow::subProcessExitStatus);
    connect(&subProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::read7zaStandardOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAddFiles_clicked()
{
    inputFilesList = QFileDialog::getOpenFileNames(this,
                                                   tr("Browse files"),
                                                   QDir::currentPath(),
                                                   tr("All files (*.*)"));

    if( !inputFilesList.isEmpty() )
    {
        //Printing the list of files
        ui->plainTextEditInputFiles->setPlainText(inputFilesList.join("\n"));
    }
}

void MainWindow::on_btnRemovesFiles_clicked()
{
    //Emptying list of files
    inputFilesList.clear();
    ui->plainTextEditInputFiles->setPlainText("");
}

void MainWindow::on_btnBrowseOutputPath_clicked()
{
    outputFilePath =QFileDialog::getSaveFileName(this,
                                                 tr("Save file as"),
                                                 QDir::currentPath(),
                                                 tr("Zip files (*.7z)"));

    ui->lineEditOutputPath->setText(outputFilePath);
}

void MainWindow::on_btnCompressFiles_clicked()
{
    if(inputFilesList.isEmpty() )
    {
        QMessageBox::warning(this, tr("7za"), QString("You must select at least 1 file to compress"));
        return;
    } else if(outputFilePath.isEmpty())
    {
        QMessageBox::warning(this, tr("7za"), QString("You must select the output file path"));
        return;
    }

    // Escape spaces in outputPath
    QString zipOutputString(outputFilePath= "\"" + outputFilePath + "\"");

    // Escape functional spaces in inputFilesList
    QString zipInputString("\"" +inputFilesList.join("\" \"")+ "\"");

    // Running 7za.exe with option "a" and the formatted zipInput and zipOut strings
    subProcess.start("7za.exe a "  + zipOutputString + " " + zipInputString);
}

void MainWindow::subProcessStatusChanged(QProcess::ProcessState state)
{
    // Checking the status of the sub process.
    switch (state)
    {
        case QProcess::Running:
        case QProcess::Starting:
            ui->labelStatus->setText("Compressing files...");
            break;

        case QProcess::NotRunning:
        default:
            ui->labelStatus->setText("Ready...");
            break;
    }
}

void MainWindow::subProcessExitStatus(int exitCode, QProcess::ExitStatus exitStatus)
{
    // Printing 7za exit status code and its details message
    switch (exitStatus)
    {
        case QProcess::Crashed:
            QMessageBox::critical(this, tr("7za"), QString("crashed"));
            break;

        case QProcess::NormalExit:
            QMessageBox::information(this,
                                     tr("7za"),
                                     QString("7za process finished with exit status code "
                                             +QString::number(exitCode)
                                             +".\n"
                                             + zipOutPutMessage));
            break;
     }
}

void MainWindow::read7zaStandardOutput()
{
    // Reading 7za standard output message
    zipOutPutMessage=subProcess.readAllStandardOutput();
}
