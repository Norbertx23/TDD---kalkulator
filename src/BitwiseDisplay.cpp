#include "BitwiseDisplay.hpp"
#include <QMouseEvent>
#include <QPainter>

BitwiseDisplay::BitwiseDisplay(QWidget *parent) : QWidget(parent), m_value(0) {
  setMinimumHeight(160);
  setMinimumWidth(300);
}

void BitwiseDisplay::setValue(unsigned long long value) {
  if (m_value != value) {
    m_value = value;
    update();
    emit valueChanged(m_value);
  }
}

unsigned long long BitwiseDisplay::getValue() const { return m_value; }

void BitwiseDisplay::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QFont font = painter.font();
  font.setPointSize(10);
  painter.setFont(font);

  int startX = 10;
  int startY = 10;

  for (int row = 0; row < 4; ++row) {
    int baseBit = (3 - row) * 16;

    int currentX = startX;
    int currentY =
        startY + row * (BIT_HEIGHT + ROW_SPACING + 15);


    for (int i = 15; i >= 0; --i) {
      int bitIndex = baseBit + i;
      bool isSet = (m_value >> bitIndex) & 1;
      bool isActive = bitIndex < m_bitCount;

      QRect bitRect(currentX, currentY, BIT_WIDTH, BIT_HEIGHT);

      if (isActive) {
        if (isSet) {
          painter.setPen(Qt::white);
          painter.setFont(QFont("Monospace", 10, QFont::Bold));
        } else {
          painter.setPen(Qt::gray);
          painter.setFont(QFont("Monospace", 10, QFont::Normal));
        }
      } else {
        painter.setPen(QColor(60, 60, 60));
        painter.setFont(QFont("Monospace", 10, QFont::Normal));
      }
      painter.drawText(bitRect, Qt::AlignCenter, isSet ? "1" : "0");

      currentX += BIT_WIDTH;

      if (i % 4 == 0 && i != 0) {
        currentX += GROUP_SPACING;
      }
    }

    painter.setPen(Qt::darkGray);
    QFont smallFont = font;
    smallFont.setPointSize(8);
    painter.setFont(smallFont);

    painter.drawText(startX - 20, currentY + BIT_HEIGHT / 2 + 4,
                     QString::number(baseBit + 15));

    painter.drawText(currentX + 5, currentY + BIT_HEIGHT / 2 + 4,
                     QString::number(baseBit));

    painter.setFont(font);
  }
}

void BitwiseDisplay::mousePressEvent(QMouseEvent *event) {
  int x = event->pos().x();
  int y = event->pos().y();

  int startX = 10;
  int startY = 10;

  for (int row = 0; row < 4; ++row) {
    int rowTop = startY + row * (BIT_HEIGHT + ROW_SPACING + 15);
    int rowBottom = rowTop + BIT_HEIGHT;

    if (y >= rowTop && y <= rowBottom) {
      int baseBit = (3 - row) * 16;
      int currentX = startX;

      for (int i = 15; i >= 0; --i) {
        int groupSpacing = (i % 4 == 0 && i != 0) ? GROUP_SPACING : 0;
        int width = BIT_WIDTH + groupSpacing;
        int paintX = startX;
        for (int j = 15; j > i; --j) {
          paintX += BIT_WIDTH;
          if (j % 4 == 0 && j != 0)
            paintX += GROUP_SPACING;
        }

        if (x >= paintX && x < paintX + BIT_WIDTH) {
          int bitIndex = baseBit + i;
          m_value ^= (1ULL << bitIndex);
          update();
          emit bitToggled(bitIndex);
          emit valueChanged(m_value);
          return;
        }
      }
    }
  }
}

void BitwiseDisplay::setDataSize(int bitCount) {
  if (m_bitCount != bitCount) {
    m_bitCount = bitCount;
    update();
  }
}
