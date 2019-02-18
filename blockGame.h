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

inline bool qMapLessThanKey(const QPoint &key1, const QPoint &key2)
{
    int dx = key1.x() - key2.x();
    int dy = key1.y() - key2.y();
    return (dx < 0) || (dx == 0 && dy < 0);
}

class BlockGame : public QWidget {
        Q_OBJECT
public:

    static constexpr int kDefaultBlockLength = 10;
    static constexpr int kDefaultWidth = 600/kDefaultBlockLength;
    static constexpr int kDefaultHeight = 500/kDefaultBlockLength;

    BlockGame(const QString &title,
              int w=kDefaultWidth,
              int h=kDefaultHeight,
              int blockLen=kDefaultBlockLength);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    void drawBlock(QPainter &painter, const QPoint pos, int len=1);


protected slots:
    virtual void gameLoop();
    virtual void gameStart();
    virtual void gamePause();

protected:
    QColor gameBackgroundColor_{Qt::white};
    QTimer gameLoopTimer_;
    int gamePeriod_{100};
    int gameBlockLen_ = kDefaultBlockLength;
    bool gamePaused_{false};
};
