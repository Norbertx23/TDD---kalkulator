#pragma once

#include <QWidget>
#include <vector>

class BitwiseDisplay : public QWidget {
  Q_OBJECT

public:
  explicit BitwiseDisplay(QWidget *parent = nullptr);
  void setValue(unsigned long long value);
  void setDataSize(int bitCount);
  unsigned long long getValue() const;

signals:
  void bitToggled(int bitIndex);
  void valueChanged(unsigned long long newValue);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

private:
  unsigned long long m_value;
  int m_bitCount = 64;
  QRect getBitRect(int bitIndex) const;

  const int BIT_WIDTH = 15;
  const int BIT_HEIGHT = 20;
  const int GROUP_SPACING = 10;
  const int ROW_SPACING = 5;
};
