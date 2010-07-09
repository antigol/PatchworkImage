#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

Widget::Widget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::Widget)
{
    ui->setupUi(this);

    for (int i = 0; i < 2; ++i) {
        _threads.append(new Thread(this));
        connect(_threads.last(), SIGNAL(onemore(int)), ui->progressBar, SLOT(increment(int)));
    }

    connect(ui->pushButton_append, SIGNAL(clicked()), this, SLOT(appendPatches()));
    connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(loadImage()));
}

Widget::~Widget()
{
    for (int i = 0; i < 2; ++i)
        _threads[i]->wait();

    delete ui;
}

void Widget::appendPatches()
{
    QFileDialog fileDialog(this, tr("Patches"));
    fileDialog.setFilter("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setOptions(QFileDialog::ReadOnly);

    if (fileDialog.exec() == QDialog::Accepted) {
        ui->progressBar->setMaximum(0);

        QStringList fileNames = fileDialog.selectedFiles();

        for (int i = 0; i < fileNames.size(); ++i) {
            const QString &file = fileNames[i];

            Patch *patch = new Patch(file);
            _patches.append(patch);

            QListWidgetItem *item = new QListWidgetItem(QIcon(QPixmap::fromImage(*patch)),
                                                        QFileInfo(file).baseName());
            ui->listWidget->addItem(item);

            _threads[i % _threads.size()]->addPatch(patch);
        }

        ui->progressBar->setMaximum(fileNames.size());

        for (int i = 0; i < _threads.size(); ++i) {
            _threads[i]->start();
        }
    }
}

void Widget::loadImage()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Working image"), QString(), "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");

    if (!file.isEmpty()) {
        workingImage.load(file);
        ui->label_image->setPixmap(QPixmap::fromImage(workingImage));
    }
}

void Widget::patch()
{
    QList<QImage> scaledPatches;

    int pWidth = ui->spinBox_width->value();
    int pHeight = ui->spinBox_height->value();

    for (int i = 0; i < _patches.size(); ++i) {
        scaledPatches.append(_patches[i]->scaled(pWidth, pHeight, Qt::KeepAspectRatioByExpanding));
    }

    QImage result = workingImage.scaled(workingImage.size() * ui->doubleSpinBox_scale->value());

    for (int x = 0; x < result.width(); x += pWidth) {
        for (int y = 0; y < result.height(); x += pHeight) {

        }
    }
}




void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
