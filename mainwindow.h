#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineSeries>
#include <QChart>
#include <QChartView>
#include <QPointF>
#include <QColor>

#include "dsp.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showEvent(QShowEvent* event);

private slots:
    void on_resetbtn_pressed();

private:
    Ui::MainWindow *ui;

    qint32 fs;
    qint32 N;
    qint32 m_coefs_count;

    // Chart's vector
    QVector<QChartView*> m_charts;

    // input
    QChartView* m_inputcv;
    QLineSeries* m_inputls;
    QLineSeries* m_inputcls;

    // output
    QChartView* m_outputcv;
    QLineSeries* m_outputls;
    QLineSeries* m_outputcls;

    // coefs
    QChartView* m_coefscv;
    QLineSeries* m_coefsls;

    // fr
    QChartView* m_coefsfrcv;
    QLineSeries* m_coefsfrls;

    void charts_config();
    void charts_resize(QChartView* a_chartview);
    QVector<QPointF> make_points(QVector<double> a_values);
    // returns 0 if correct
    quint8 draw_complex(QVector<std::complex<double>> a_values, QChartView* a_chartview);
    void series_init();
    void charts_init();
};
#endif // MAINWINDOW_H
