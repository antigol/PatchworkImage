#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QSettings>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    _scaleAndPaste(0)
{
    ui->setupUi(this);

    for (int i = 0; i < cpucount(); ++i) {
        _averagePatches.append(new Thread(this));
        connect(_averagePatches.last(), SIGNAL(onemore(int)), ui->progressBar, SLOT(increment(int)));
        connect(_averagePatches.last(), SIGNAL(finished()), this, SLOT(averageFinished()));
    }

    connect(ui->pushButton_append, SIGNAL(clicked()), this, SLOT(appendPatches()));
    connect(ui->pushButton_load, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(ui->pushButton_patch, SIGNAL(clicked()), this, SLOT(patch()));

    loadSettings();
}

Widget::~Widget()
{
    saveSettings();

    for (int i = 0; i < _averagePatches.size(); ++i) {
        _averagePatches[i]->wait();
    }

    if (_scaleAndPaste != 0) {
        _scaleAndPaste->stop();
        _scaleAndPaste->wait();
    }

    delete ui;
}

void Widget::loadSettings()
{
    QSettings settings;
    ui->spinBox_width->setValue(settings.value("width_of_patch", 32).toInt());
    ui->spinBox_height->setValue(settings.value("height_of_patch", 32).toInt());
    ui->doubleSpinBox_scale->setValue(settings.value("scale_value", 1.0).toDouble());
}

void Widget::saveSettings()
{
    QSettings settings;
    settings.setValue("width_of_patch", ui->spinBox_width->value());
    settings.setValue("height_of_patch", ui->spinBox_height->value());
    settings.setValue("scale_value", ui->doubleSpinBox_scale->value());
}

void Widget::appendPatches()
{
    QFileDialog fileDialog(this, tr("Patches"));
    fileDialog.setDirectory(QSettings().value("patches_directory", QDir::homePath()).toString());
    fileDialog.setFilter("Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    fileDialog.setOptions(QFileDialog::ReadOnly);

    if (fileDialog.exec() == QDialog::Accepted) {
        ui->progressBar->setMaximum(0);

        QSettings().setValue("patches_directory", fileDialog.directory().path());

        QStringList fileNames = fileDialog.selectedFiles();

        for (int i = 0; i < fileNames.size(); ++i) {
            const QString &file = fileNames[i];

            Patch *patch = new Patch(file);
            _patches.append(patch);

            QListWidgetItem *item = new QListWidgetItem(/*QIcon(QPixmap::fromImage(*patch)),*/
                                                        QFileInfo(file).baseName());
            ui->listWidget->addItem(item);

            _averagePatches[i % _averagePatches.size()]->addPatch(patch);

            // qDebug("add patch...");
        }

        ui->progressBar->setMaximum(fileNames.size());
        ui->progressBar->setValue(1);

        ui->pushButton_append->setEnabled(false);
        ui->pushButton_patch->setEnabled(false);
        for (int i = 0; i < _averagePatches.size(); ++i) {
            _averagePatches[i]->start();
        }
    }
}

void Widget::averageFinished()
{
    for (int i = 0; i < _averagePatches.size(); ++i) {
        if (_averagePatches[i]->isRunning())
            return;
    }
    ui->pushButton_append->setEnabled(true);
    ui->pushButton_patch->setEnabled(true);
}

void Widget::loadImage()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Working image"), QSettings().value("image_path", QDir::homePath()).toString(), "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");

    if (!file.isEmpty()) {
        if (workingImage.load(file)) {
            QSettings().setValue("image_path", file);
            ui->label_image->setPixmap(QPixmap::fromImage(workingImage).scaled(ui->label_image->geometry().size(), Qt::KeepAspectRatio));
        }
    }
}

void Widget::patch()
{
    if (_scaleAndPaste != 0)
        return;

    const QSize patchSize(ui->spinBox_width->value(), ui->spinBox_height->value());
    const QSize workSize((workingImage.width() * ui->doubleSpinBox_scale->value()) / patchSize.width(),
                         (workingImage.height() * ui->doubleSpinBox_scale->value()) / patchSize.height());
    const int works = workSize.width() * workSize.height();

    if (patchSize.isNull() || workSize.isNull())
        return;

    ui->progressBar->setMaximum(works);
    ui->pushButton_patch->setDisabled(true);

    _scaleAndPaste = new ScaleAndPaste(_patches, workSize, patchSize, this);

    connect(_scaleAndPaste, SIGNAL(finished()), this, SLOT(patchfinished()));
    connect(_scaleAndPaste, SIGNAL(progress(int)), ui->progressBar, SLOT(setValue(int)));

    const QImage miniwork = workingImage.scaled(workSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label_image->setPixmap(QPixmap::fromImage(miniwork));

    const int compareThreads = qMax(1, cpucount() - 1);
    const int part = works / compareThreads;

    for (int i = 0; i < compareThreads; ++i) {
        CompareAverage *captr = new CompareAverage(miniwork, _patches, i * part, (i + 1 == compareThreads) ? (works) : ((i + 1) * part), patchSize, _scaleAndPaste, this);
        //connect(captr, SIGNAL(toPaste(int,QPoint)), _scaleAndPaste, SLOT(paste(int,QPoint)));

        captr->start();
        _compareAverages.append(captr);
    }

    _scaleAndPaste->start();
}

void Widget::patchfinished()
{
//    qDebug("finished");
    ui->label_image->setPixmap(QPixmap::fromImage(_scaleAndPaste->finalImage()).scaled(ui->label_image->geometry().size(), Qt::KeepAspectRatio));
    //ui->listWidget->clear();
    ui->pushButton_patch->setEnabled(true);

    QString file = QFileDialog::getSaveFileName(this, tr("Working image"), QSettings().value("image_save", QDir::homePath()).toString(), "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm)");

    if (!file.isEmpty()) {
        QSettings().setValue("image_save", file);
        if (!_scaleAndPaste->finalImage().save(file))
            QMessageBox::critical(this, tr("Saving error"), tr("Cannot save the file"));
    }

    for (int i = 0; i < _compareAverages.size(); ++i) {
        _compareAverages.first()->wait();
        delete _compareAverages.takeFirst();
    }
    delete _scaleAndPaste; _scaleAndPaste = 0;
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

int cpucount()
{
    return QThread::idealThreadCount();
}
