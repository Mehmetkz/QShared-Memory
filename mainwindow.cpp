#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    smemory.setKey("CommonMemory");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFromFile()
{
    smemory.setKey("MemoryFromFile");
    if (smemory.isAttached()){
        detach();
        return;
    }

    ui->label->setText(tr("Select an image file"));
    QString fileName = QFileDialog::getOpenFileName(0, QString(), QString(), tr("Images (*.png *.xpm *.jpg)"));

    QImage image;
    image.load(fileName);

    if (!image.load(fileName)) {
        ui->label->setText(tr("Selected file is not an image, please select another."));
        return;
    }

    // load into shared memory
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << image;
    int size = 0;
    size = buffer.size();


    if (!smemory.create(size)) {
        ui->label->setText(tr("Unable to create shared memory segment."));
        return;
    }

    ui->label2->setPixmap(QPixmap::fromImage(image));
    smemory.lock();
    char *to = (char*)smemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(smemory.size(), size));
    smemory.attach();
    if(smemory.isAttached()){
        ui->label->setText(tr("Data is attached from the file."));
    }
    smemory.unlock();
}

void MainWindow::loadFromMemory()
{
    qDebug()<<smemory.key();
//    ui->label_memory->setText(smemory.key());

    if(smemory.isAttached()){
        ui->label_memory->setText(tr("Data is attached on the memory."));
    }
    else
        ui->label_memory->setText(tr("Unable to attach to shared memory segment.\n" \
                                     "Load an image first."));

    QBuffer buffer;
    QDataStream in(&buffer);
    QImage image;

    smemory.lock();
    buffer.setData((char*)smemory.constData(), smemory.size());
    buffer.open(QBuffer::ReadOnly);
    in >> image;
    smemory.unlock();

    ui->label_memory2->setPixmap(QPixmap::fromImage(image));
    smemory.detach();
}

void MainWindow::detach()
{
    if(smemory.isAttached()){
        ui->label->setText("Unable to detech from shared memory.");
        return;
    }
}

void MainWindow::on_file_clicked()
{
    loadFromFile();
}

void MainWindow::on_memory_clicked()
{
    loadFromMemory();
}
