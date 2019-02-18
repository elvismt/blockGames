// Copyright © 2019 Elvis Teixeira
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http:// www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "tetris.h"
#include <QPainter>
#include <QKeyEvent>
#include <QDateTime>

static const int TETR_PEN_WIDTH = 1;

struct Tetromino {
    QPoint pos;
    QPen pen;
    QBrush brush;
    int rotation;
    const char *data;


    int posToIndx(const QPoint &pos) const
    {
        if (rotation == 0) {
            return  pos.y() * 4 + pos.x();
        } else if (rotation == 1) {
            return 12 + pos.y() - 4 * pos.x();
        } else if (rotation == 2) {
            return 15 - 4 * pos.y() - pos.x();
        } else if (rotation == 3) {
            return 3 - pos.y() + 4 * pos.x();
        }

        Q_ASSERT(false);
        return -1;
    }


    // Tells if the square at position pos is filled
    // Takes into account rotation
    bool operator[](const QPoint &pos) const {
        return data[posToIndx(pos)] != '0';
    }
};

static Tetromino TETRS[] = {
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::red),
        0,
        "00#0"
        "00#0"
        "0##0"
        "0000"
    },
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::green),
        0,
        "00#0"
        "0##0"
        "00#0"
        "0000"
    },
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::yellow),
        0,
        "0##0"
        "0##0"
        "0000"
        "0000"
    },
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::magenta),
        0,
        "00#0"
        "00#0"
        "00#0"
        "0000"
    }
};


void Tetris::drawTetromino(QPainter &painter, const Tetromino &tetr)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            QPoint pos(i, j);
            if (tetr[pos]) {
                painter.setPen(tetr.pen);
                painter.setBrush(tetr.brush);
                pos += tetr.pos;
                drawBlock(painter, pos);
            }
        }
    }
}

bool Tetris::fits(const Tetromino &tetr, const QPoint &iPos)
{
    const auto size = this->size();
    const int W = size.width() / gameBlockLen_;
    const int H = size.height() / gameBlockLen_;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            const QPoint basePos(i, j);

            if (tetr[basePos]) {
                const QPoint pos = iPos + basePos;

                // Check for colision with a landscape block
                auto iter = landscape_.constBegin();
                for (; iter != landscape_.constEnd(); ++iter) {
                    if (iter.key() == pos) {
                        return false;
                    }
                }

                // Check for colision with the walls or ground
                if (pos.x() < 0 || pos.x() >= W
                        || pos.y() < 0 || pos.y() >= H) {
                    return false;
                }
            }
        }
    }

    return true;
}

Tetris::Tetris(const QString &title, int w, int h, int blockLen)
    : BlockGame (title, w, h, blockLen)
{
    newPiece();
    gamePeriod_ = 500;
    qsrand(QDateTime::currentDateTime().toTime_t());
    gameStart();
}

void Tetris::paintEvent(QPaintEvent *event)
{
    BlockGame::paintEvent(event);
    QPainter painter(this);

    auto iter = landscape_.constBegin();
    for (; iter != landscape_.constEnd(); ++iter) {
        painter.setPen(QPen(iter.value().pen));
        painter.setBrush(QBrush(iter.value().brush));
        drawBlock(painter, iter.key());
    }

    // Draw the current moving tetromino
    drawTetromino(painter, *currentPiece_);

    // Draw the score
    painter.setPen(Qt::black);
    painter.drawText(QPoint(10, 20), QString("Score: %1").arg(score_));
}

void Tetris::newPiece()
{
    const auto windowSize = this->size();

    int tetrId = qrand() % 4;
    int tetrRot = qrand() % 4;

    currentPiece_ = &TETRS[tetrId];
    currentPiece_->rotation = tetrRot;

    int x = windowSize.width() / gameBlockLen_ / 2;
    currentPiece_->pos = QPoint(x - 2, 0);
}

void Tetris::addToLandscape()
{
    const auto &tetr = *currentPiece_;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            QPoint pos(i, j);
            if (tetr[pos]) {
                QPoint landPos = tetr.pos + pos;
                landscape_.insert(landPos, {tetr.pen.color(), tetr.brush.color()});
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        int y = tetr.pos.y() + i;
        while (checkRowComplete(y));
    }
}

void Tetris::fallUpperRows(int y) {
    const int W = this->width() / gameBlockLen_;

    for (; y >= 0; --y) {
        for (int x = 0; x < W; ++x) {
            auto iter = landscape_.find(QPoint(x, y));
            if (iter != landscape_.end()) {
                auto block = *iter;
                landscape_.erase(iter);
                landscape_.insert(QPoint(x, y + 1), block);
            }
        }
    }
}

bool Tetris::checkRowComplete(int y) {
    const int W = this->width() / gameBlockLen_;
    int found = 0;

    for (int x = 0; x < W; ++x) {
        auto iter = landscape_.constFind(QPoint(x, y));
        if (iter != landscape_.constEnd()) {
            found += 1;
        }
    }

    if (found == W) {
        // complete row, remove its blocks
        // and make the upper ones fall
        for (int x = 0; x < W; ++x) {
            landscape_.remove(QPoint(x, y));
        }
        score_ += 1;
        fallUpperRows(y-1);
        return true;
    }

    return false;
}

void Tetris::gameLoop()
{
    Q_ASSERT(currentPiece_);
    QPoint nextPos = currentPiece_->pos + QPoint(0, 1);

    if (fits(*currentPiece_, nextPos)) {
        currentPiece_->pos = nextPos;
    } else {
        addToLandscape();
        newPiece();
    }

    repaint();
}

void Tetris::keyPressEvent(QKeyEvent *event)
{
    const auto key = event->key();
    BlockGame::keyPressEvent(event);

    if (gamePaused_) {
        return;
    }

    if (key == Qt::Key_Up) {
        const int oldRot = currentPiece_->rotation;
        const QPoint pos = currentPiece_->pos;
        currentPiece_->rotation = ((oldRot + 1) % 4);
        if (!fits(*currentPiece_, pos)) {
            // Must check if this rotation puts the
            // tetromino out of bounds
            currentPiece_->rotation = oldRot;
        }
        repaint();
    } else if (key == Qt::Key_Left) {
        auto nextPos = currentPiece_->pos + QPoint(-1, 0);
        if (fits(*currentPiece_, nextPos)) {
            currentPiece_->pos = nextPos;
            repaint();
        }
    } else if (key == Qt::Key_Right) {
        auto nextPos = currentPiece_->pos + QPoint(1, 0);
        if (fits(*currentPiece_, nextPos)) {
            currentPiece_->pos = nextPos;
            repaint();
        }
    } else if (key == Qt::Key_Down) {
        auto nextPos = currentPiece_->pos + QPoint(0, 1);
        if (fits(*currentPiece_, nextPos)) {
            currentPiece_->pos = nextPos;
            repaint();
        }
    }
}

// tetris.cpp
