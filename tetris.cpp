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

static const int TETR_PEN_WIDTH = 1;

struct Tetromino {
    QPoint pos;
    QPen pen;
    QBrush brush;
    int rotation;
    const char *data;
};

static Tetromino TETRS[] = {
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::red),
        0,
        "..x."
        "..x."
        ".xx."
        "...."
    },
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::green),
        0,
        "..x."
        ".xx."
        "..x."
        "...."
    },
    {
        QPoint(0,0),
        QPen(QBrush(Qt::blue), TETR_PEN_WIDTH),
        QBrush(Qt::magenta),
        0,
        ".xx."
        ".xx."
        "...."
        "...."
    }
};

bool tetrominoPos(QPoint &pos, const Tetromino &tetr, int k)
{
    const char *data = tetr.data;

    if (data[k] == 'x'){
        pos = QPoint(tetr.pos.x() + k % 4, tetr.pos.y() + k / 4);
        return true;
    }

    return false;
}


void Tetris::drawTetromino(QPainter &painter, int tetrId)
{
    auto &tetromino = TETRS[tetrId];

    for (int k = 0; k < 16; ++k) {
        QPoint pos;
        if (tetrominoPos(pos, tetromino, k)) {
            painter.setPen(tetromino.pen);
            painter.setBrush(tetromino.brush);
            drawBlock(painter, pos);
        }
    }
}


Tetris::Tetris(const QString &title, int w, int h, int blockLen)
    : BlockGame (title, w, h, blockLen)
{
}

void Tetris::paintEvent(QPaintEvent *event)
{
    BlockGame::paintEvent(event);
    QPainter painter(this);

    // Example tetromino drawing

    TETRS[0].pos = QPoint{-1, 0};
    drawTetromino(painter, 0);

    TETRS[1].pos = QPoint{4, 4};
    drawTetromino(painter, 1);

    TETRS[2].pos = QPoint{4, 8};
    drawTetromino(painter, 2);
}
