#pragma once

#include <KConfigWatcher>
#include <KDecoration2/Decoration>

namespace Arstotzka {
class Decoration : public KDecoration2::Decoration {
    Q_OBJECT
public:
    // clang-format off
    explicit Decoration(QObject *parent = nullptr, const QVariantList &args = QVariantList());
    // clang-format on

    /**
     * Provides the rendering.
     *
     * The painter is set up to paint on an internal QPaintDevice. The painting is
     * implicitly double buffered.
     */
    void paint(QPainter *painter, const QRect &repaintRegion) override;

public Q_SLOTS:
    /**
     * This method gets invoked once the Decoration is created and completely setup.
     *
     * All initialization must be performed in this method instead of the constructor.
     */
    bool init() override;

private:
    void updateColors();
    void setBorderSizes();
    void changeActive();
    void connectEvents();

    QColor activeColor;
    QColor inactiveColor;

    KConfigWatcher::Ptr configWatcher;
};
}
