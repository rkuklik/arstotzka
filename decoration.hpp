#pragma once

#include <KConfigWatcher>
#include <KDecoration3/Decoration>

namespace Arstotzka {
class Decoration : public KDecoration3::Decoration {
        Q_OBJECT
    public:
        explicit Decoration(
            QObject* parent = nullptr,
            const QVariantList& args = QVariantList()
        );

        /**
         * Provides the rendering.
         *
         * The painter is set up to paint on an internal QPaintDevice. The
         * painting is implicitly double buffered.
         */
        void paint(QPainter* painter, const QRectF& repaintRegion) override;

    public Q_SLOTS:
        /**
         * This method gets invoked once the Decoration is created and
         * completely setup.
         *
         * All initialization must be performed in this method instead of the
         * constructor.
         */
        bool init() override;

    private:
        void updateColors();
        void setBorderSizes();
        void connectEvents();

        QColor active;
        QColor inactive;
        KConfigWatcher::Ptr watcher;
};
}  // namespace Arstotzka
