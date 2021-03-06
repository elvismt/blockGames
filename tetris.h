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

#pragma once
#include "blockGame.h"
#include <QPen>
#include <QBrush>
#include <QMap>

struct Tetromino;
constexpr int kTetrisWidth = 24;
constexpr int kTetrisHeight = 32;

class Tetris: public BlockGame
{
    Q_OBJECT

public:

    Tetris(const QString &title="Tetris",
           int w=kTetrisWidth,
           int h=kTetrisHeight,
           int blockLen=kDefaultBlockLength);

protected:

    virtual void paintEvent(QPaintEvent *event) override;
    void drawTetromino(QPainter &painter, const Tetromino &tetr);
    void keyPressEvent(QKeyEvent *event) override;
    void newPiece();
    void fallUpperRows(int y);
    void addToLandscape();
    bool checkRowComplete(int y);
    bool fits(const Tetromino &tetr, const QPoint &pos);

protected slots:
    virtual void gameLoop() override;


protected:

    struct LandscapeBlock {
        QColor pen;
        QColor brush;
    };

    Tetromino *currentPiece_{nullptr};
    QMap<QPoint,LandscapeBlock> landscape_;
    int score_{0};
};
