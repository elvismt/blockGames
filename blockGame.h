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
#include <QWidget>
#include <QColor>
#include <QTimer>

class BlockGame : public QWidget {
        Q_OBJECT
public:

    static constexpr int DEFAULT_BLOCK_LEN = 10;
    static constexpr int DEFAULT_WIDTH = 600/DEFAULT_BLOCK_LEN;
    static constexpr int DEFAULT_HEIGHT = 500/DEFAULT_BLOCK_LEN;

    BlockGame(const QString &title,
              int w=DEFAULT_WIDTH,
              int h=DEFAULT_HEIGHT,
              int blockLen=DEFAULT_BLOCK_LEN);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    void drawBlock(QPainter &painter, const QPoint pos, int len=1);


protected slots:
    virtual void gameLoop();
    virtual void gameStart();

protected:
    QColor backgroundColor_{Qt::white};
    QTimer gameLoopTimer_;
    int period_{100};
    int blockLen_ = DEFAULT_BLOCK_LEN;
};
