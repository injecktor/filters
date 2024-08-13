#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fs(10000)
    , N(1000)
    , m_coefs_count(50)
{
    ui->setupUi(this);
    charts_init();
    series_init();
    charts_config();
    QVector<double> coefs;
    coefs.resize(m_coefs_count);
    for (int var = 0; var < m_coefs_count; ++var) {
        double shift = var - m_coefs_count / 2;
        double sinc;
        if (shift == 0) {
            sinc = 1;
        }
        else {
            sinc = sin(shift) / shift;
        }
        coefs[var] = sinc * sin(2 * PI * fs / 4. * (double)var / fs);
    }
    m_coefsls->replace(make_points(coefs));
    auto coefsfr = ft::dft_m(coefs);
    m_coefsfrls->replace(make_points(coefsfr));
    QVector<double> signal;
    signal.resize(N);
    for (int var = 0; var < N; ++var) {
        signal[var] = sin(2 * PI * 1000 * (double)var / fs);
    }
    m_inputls->replace(make_points(signal));
    auto filtered_signal = ft::convolution(signal, coefs);
    m_outputls->replace(make_points(filtered_signal));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* /*event*/) {
    for (int var = 0; var < m_charts.size(); ++var) {
        charts_resize(m_charts[var]);
    }
    ui->resetbtn->pressed();
}

void MainWindow::charts_config()
{
    Q_ASSERT(m_charts.size() == m_series.size());

    for (int var = 0; var < m_series.size(); ++var) {
        m_charts[var]->setRenderHint(QPainter::Antialiasing);
        m_charts[var]->chart()->addSeries(m_series[var]);
        m_charts[var]->chart()->createDefaultAxes();
        m_charts[var]->chart()->zoomReset();
        m_charts[var]->chart()->legend()->hide();
        m_charts[var]->setDragMode(QGraphicsView::NoDrag);
        m_charts[var]->setRubberBand(QChartView::RectangleRubberBand);
    }
}

void MainWindow::charts_resize(QChartView* a_chartview)
{
    a_chartview->resize(a_chartview->parentWidget()->size());
}

QVector<QPointF> MainWindow::make_points(QVector<double> a_values)
{
    QVector<QPointF> points;
    points.reserve(a_values.size());
    for (qsizetype var = 0; var < a_values.size(); ++var) {
        points.append({static_cast<double>(var), a_values[var]});
    }
    return points;
}

void MainWindow::on_resetbtn_pressed()
{
    m_inputcv->chart()->axes(Qt::Horizontal).back()->setRange(0, N);
    m_inputcv->chart()->axes(Qt::Vertical).back()->setRange(-2, 2);

    m_outputcv->chart()->axes(Qt::Horizontal).back()->setRange(0, N);
    m_outputcv->chart()->axes(Qt::Vertical).back()->setRange(-2, 2);

    m_coefscv->chart()->axes(Qt::Horizontal).back()->setRange(0, m_coefs_count);
    m_coefscv->chart()->axes(Qt::Vertical).back()->setRange(-1, 2);

    m_coefsfrcv->chart()->axes(Qt::Horizontal).back()->setRange(0, m_coefs_count);
    m_coefsfrcv->chart()->axes(Qt::Vertical).back()->setRange(0, m_coefs_count / 20);
}

void MainWindow::charts_init()
{
    m_inputcv = new QChartView(new QChart(), ui->input_widget);
    m_outputcv = new QChartView(new QChart(), ui->output_widget);
    m_coefscv = new QChartView(new QChart(), ui->coefs_widget);
    m_coefsfrcv = new QChartView(new QChart(), ui->coefsfr_widget);

    m_charts.append(m_inputcv);
    m_charts.append(m_outputcv);
    m_charts.append(m_coefscv);
    m_charts.append(m_coefsfrcv);
}

void MainWindow::series_init()
{
    m_inputls = new QLineSeries;
    m_outputls = new QLineSeries;
    m_coefsls = new QLineSeries;
    m_coefsfrls = new QLineSeries;

    m_series.append(m_inputls);
    m_series.append(m_outputls);
    m_series.append(m_coefsls);
    m_series.append(m_coefsfrls);
}












