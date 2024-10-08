#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fs(10000)
    , N(1000)
    , m_coefs_count(129)
{
    ui->setupUi(this);
    charts_init();
    series_init();
    charts_config();
    QVector<double> coefs = ft::hilbert_coefs(m_coefs_count, 1);
    draw(coefs, m_coefscv);
    auto coefsfr = ft::dft_module(ft::dft(coefs));
    draw(coefsfr, m_coefsfrcv);
    QVector<double> signal;
    QVector<double> signal2;
    signal.resize(N);
    signal2.resize(N);
    for (int var = 0; var < N; ++var) {
        signal[var] = sin(2 * PI * 1000 * (double)var / fs);
        signal2[var] = sin(2 * PI * 1000 * (double)var / fs + PI / 2);
    }
    draw(signal, m_inputcv);
    auto filtered_signal = ft::convolution(signal, coefs);
    draw(filtered_signal, m_outputcv);
    draw(signal2, m_outputcv, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* /*event*/) {
    for (int var = 0; var < m_charts.size(); ++var) {
        charts_resize(m_charts[var]);
    }
    emit ui->resetbtn->pressed();
}

void MainWindow::charts_config()
{
    // Real serias must be added before complex ones
    m_inputcv->chart()->addSeries(m_inputls);
    m_inputcv->chart()->addSeries(m_inputcls);
    m_outputcv->chart()->addSeries(m_outputls);
    m_outputcv->chart()->addSeries(m_outputcls);
    m_coefscv->chart()->addSeries(m_coefsls);
    m_coefsfrcv->chart()->addSeries(m_coefsfrls);

    for (int var = 0; var < m_charts.size(); ++var) {
        m_charts[var]->setRenderHint(QPainter::Antialiasing);
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

quint8 MainWindow::draw(QVector<double> a_values, QChartView* a_chartview, int a_series_index)
{
    if (a_chartview->chart()->series().size() <= a_series_index) {
        return 1;
    }
    static_cast<QLineSeries*>(a_chartview->chart()->series()[a_series_index])->replace(make_points(a_values));
    return 0;
}

quint8 MainWindow::draw(QVector<std::complex<double>> a_values, QChartView* a_chartview)
{
    // check whether there are series for real and imaginary parts
    if (a_chartview->chart()->series().size() != 2) {
        return 1;
    }
    auto N = a_values.size();
    QVector<double> real;
    QVector<double> imag;
    real.resize(N);
    imag.resize(N);
    for (int var = 0; var < N; ++var) {
        real[var] = a_values[var].real();
        imag[var] = a_values[var].imag();
    }
    static_cast<QLineSeries*>(a_chartview->chart()->series()[0])->replace(make_points(real));
    static_cast<QLineSeries*>(a_chartview->chart()->series()[1])->replace(make_points(imag));
    return 0;
}

void MainWindow::on_resetbtn_pressed()
{
    m_inputcv->chart()->axes(Qt::Horizontal).back()->setRange(0, N);
    m_inputcv->chart()->axes(Qt::Vertical).back()->setRange(-2, 2);

    m_outputcv->chart()->axes(Qt::Horizontal).back()->setRange(0, N);
    m_outputcv->chart()->axes(Qt::Vertical).back()->setRange(-2, 2);

    m_coefscv->chart()->axes(Qt::Horizontal).back()->setRange(0, m_coefs_count - 1);
    m_coefscv->chart()->axes(Qt::Vertical).back()->setRange(-1, 2);

    m_coefsfrcv->chart()->axes(Qt::Horizontal).back()->setRange(0, m_coefs_count - 1);
    m_coefsfrcv->chart()->axes(Qt::Vertical).back()->setRange(0, 2);
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
    m_inputcls = new QLineSeries;
    m_outputls = new QLineSeries;
    m_outputcls = new QLineSeries;
    m_coefsls = new QLineSeries;
    m_coefsfrls = new QLineSeries;

    m_inputls->setColor(QColor(0, 0, 255));
    m_inputcls->setColor(QColor(255, 0, 0));
    m_outputls->setColor(QColor(0, 0, 255));
    m_outputcls->setColor(QColor(255, 0, 0));
    m_coefsls->setColor(QColor(0, 0, 255));
    m_coefsfrls->setColor(QColor(0, 0, 255));
}












