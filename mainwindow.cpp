#include "mainWindow.h"

// TODO Arrows of digraph

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);

    for (int i = 0; i < MAXNUM; ++i)
        positions[i] = QPoint(0, 0);

    pixmap = QPixmap(width(), height());
    pixmap.fill();

    linePen.setWidth(3);
    linePen.setColor(Qt::darkGray);
    linePen.setCapStyle(Qt::RoundCap);
    linePen.setStyle(Qt::SolidLine);
    linePen.setJoinStyle(Qt::RoundJoin);

    font.setFamily("consolas");
    font.setPointSize(15);

    circularBrush.setColor("#5599FF");
    circularBrush.setStyle(Qt::SolidPattern);
}

void MainWindow::reinitPixmap() {
    pixmap.fill();
    repaint();

    for (int i = 0; i < MAXNUM; ++i)
        positions[i] = QPoint(0, 0);
}

void MainWindow::displayOriginalMatrix(const Relation r) {
    ui.originalTextBrowser->insertPlainText("The original adjacency matrix in " + fileName + " is: \n");
    for (int row = 0; row < r.getDimension(); ++row) {
        for (int column = 0; column < r.getDimension(); ++column)
            ui.originalTextBrowser->insertPlainText(QString::number(r(row, column)) + "   ");
        ui.originalTextBrowser->insertPlainText("\n");
    }
}

void MainWindow::showMessageBox(const QString title, const QString text) {
    QMessageBox msg(this);
    msg.setWindowTitle(title);
    msg.setText(text);
    msg.setIcon(QMessageBox::Warning);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.exec();
}

void MainWindow::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

void MainWindow::drawOriginalNodes(const int dimension) {
    int count = 0, x, y;

    QPainter painter(&pixmap);
    painter.setFont(font);
    painter.setBrush(circularBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    y = 515;

    while (count < dimension) {
        if (1 == count % 2)
            y -= 100;

        // Bottom Node
        if (0 == count) {
            x = (X2 - X1) / 2 + 100;
            y -= 100;
            painter.drawText(QPoint(x - 10, y + 30), QString::number(count + 1));
            painter.drawEllipse(x, y, 20, 20);
            positions[count] = QPoint(x + 10, y + 10);
            ++count;
            continue;
        }

        if (0 == count % 2)
            x = (X2 - X1) / 2;
        else
            x = (X2 - X1) / 2 + 200;

        if ((dimension - 1 == count) && (1 == count % 2))
            x = (X2 - X1) / 2 + 100;

        painter.drawText(QPoint(x - 10, y), QString::number(count + 1));
        painter.drawEllipse(x, y, 20, 20);
        positions[count] = QPoint(x + 10, y + 10);

        ++count;
    }
}

void MainWindow::drawResultNodes(const int dimension) {
    int count = 0, x, y;

    QPainter painter(&pixmap);
    painter.setFont(font);
    painter.setBrush(circularBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    y = 980;

    // TODO Draw self loop

    while (count < dimension) {
        if (1 == count % 2)
            y -= 100;

        // Bottom Node
        if (0 == count) {
            x = (X2 - X1) / 2 + 100;
            y -= 100;
            painter.drawText(QPoint(x - 10, y + 30), QString::number(count + 1));
            painter.drawEllipse(x, y, 20, 20);
            positions[count] = QPoint(x + 10, y + 10);
            ++count;
            continue;
        }

        if (0 == count % 2)
            x = (X2 - X1) / 2;
        else
            x = (X2 - X1) / 2 + 200;

        if ((dimension - 1 == count) && (1 == count % 2))
            x = (X2 - X1) / 2 + 100;

        painter.drawText(QPoint(x - 10, y), QString::number(count + 1));
        painter.drawEllipse(x, y, 20, 20);
        positions[count] = QPoint(x + 10, y + 10);

        ++count;
    }
}

void MainWindow::drawHasseNodes(const int dimension, const int* result) {
    int number, x, y, block;
    bool zeroIndegree;
    int levelCount = 1;
    int horizontalCount = 1;
    bool* isAdded = new bool[dimension];
    QStack<int> stack;

    QPainter painter(&pixmap);
    painter.setFont(font);
    painter.setBrush(circularBrush);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int* matrix = new int[dimension * dimension];
    for (int row = 0; row < dimension; ++row)
        for (int column = 0; column < dimension; ++column)
            matrix[row * dimension + column] = result[row * dimension + column];

    for (int i = 0; i < dimension; ++i)
        isAdded[i] = false;

    while (levelCount <= dimension) {
        for (int column = 0; column < dimension; ++column) {
            zeroIndegree = true;
            for (int row = 0; row < dimension; ++row) {
                if (1 == matrix[row * dimension + column]) {
                    zeroIndegree = false;
                    break;
                }
            }

            if (zeroIndegree) {
                // If the indegree is 0, then push column-th node into stack
                if (false == isAdded[column])
                    stack.push(column);
                isAdded[column] = true;
            }
        }

        number = stack.size();
        while (!stack.isEmpty()) {
            int temp = stack.pop();
            // Delete all edges starting from No.temp node
            for (int i = 0; i < dimension; ++i)
                matrix[temp * dimension + i] = 0;

            block = (X2 - X1) / (number + 1);
            x = block * horizontalCount + 100;
            y = 800 - 60 * (levelCount - 1);

            painter.drawText(QPoint(x - 10, y), QString::number(temp + 1));
            painter.drawEllipse(x, y, 20, 20);
            positions[temp] = QPoint(x + 10, y + 10);

            ++horizontalCount;
        }
        horizontalCount = 1;
        ++levelCount;
    }

    delete[] matrix;
    matrix = nullptr;
    delete[] isAdded;
    isAdded = nullptr;
}

void MainWindow::drawLines(const int dimension, const int* result) {
    QPainter painter(&pixmap);
    painter.setPen(linePen);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    for (int row = 0; row < dimension; ++row)
        for (int column = 0; column < dimension; ++column)
            if (1 == result[row * dimension + column])
                painter.drawLine(positions[row], positions[column]);
}


void MainWindow::on_hasseBtn_clicked() {
    reinitPixmap();
    ui.originalTextBrowser->setPlainText("");
    ui.resultTextBrowser->setPlainText("");

    QString temp;
    temp = QFileDialog::getOpenFileName(this, "Choose the adjacency matrix file", QDir::currentPath(), "txt files(*.txt)");

    filePath = temp.toLocal8Bit();
    fileName = QFileInfo(temp).fileName();

    Relation r(filePath);
    displayOriginalMatrix(r);

    drawOriginalNodes(r.getDimension());
    drawLines(r.getDimension(), r.getMatrix());

    if (r.isPartial()) {
        Relation rr = r.hasse();
        int* result = rr.getMatrix();

        ui.resultTextBrowser->insertPlainText("The adjacency matrix of Hasse graph is: \n");
        for (int i = 0; i < rr.getDimension(); ++i) {
            for (int j = 0; j < rr.getDimension(); ++j) {
                ui.resultTextBrowser->insertPlainText(QString::number(rr(i, j)) + "   ");
            }
            ui.resultTextBrowser->insertPlainText("\n");
        }

        drawHasseNodes(rr.getDimension(), result);
        drawLines(rr.getDimension(), result);
    }
    else
        showMessageBox("Warning", "The relation is not a partial ordering relation!");
}
