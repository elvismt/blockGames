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

#include "blockGame.h"
#include <QIcon>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>

BlockGame::BlockGame(const QString &title, int w, int h, int blockLen)
{
    setWindowTitle(title);
    setWindowIcon(QIcon(":icons/Pacman1.ico"));

    gameBlockLen_ = blockLen;
    w *= gameBlockLen_;
    h *= gameBlockLen_;
    setMinimumSize(w, h);
    setMaximumSize(w, h);
    setAutoFillBackground(false);

    QObject::connect(&gameLoopTimer_, SIGNAL(timeout()), this, SLOT(gameLoop()));
}

void BlockGame::paintEvent(QPaintEvent *)
{
    const QSize size = this->size();
    QPainter painter(this);

    // Make background white
    painter.fillRect(QRect({0,0}, size), QBrush(gameBackgroundColor_));
}

void BlockGame::drawBlock(QPainter &painter, const QPoint iPos, int len)
{
    len *= gameBlockLen_;
    int penWid = painter.pen().width() / 2;
    int twoLenWid = 2 * penWid;
    QPoint pos = QPoint(iPos.x()*gameBlockLen_+penWid, iPos.y()*gameBlockLen_+penWid);
    painter.drawRect(QRect(pos, QSize(len-twoLenWid, len-twoLenWid)));
}

void BlockGame::gameStart()
{
    if (!gameLoopTimer_.isActive()) {
        gameLoopTimer_.start(gamePeriod_);
        gamePaused_ = false;
    }
}

void BlockGame::gamePause()
{
    if (gameLoopTimer_.isActive()) {
        gameLoopTimer_.stop();
        gamePaused_ = true;
    }
}

void BlockGame::gameLoop()
{
    // pass
}

void BlockGame::keyPressEvent(QKeyEvent *event)
{
    const auto key = event->key();

    if (key == Qt::Key_P) {
        if (gamePaused_) {
            gameStart();
        } else {
            gamePause();
        }
    }

    QWidget::keyPressEvent(event);
}
