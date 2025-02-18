#include "decoration.hpp"

#include <KDecoration3/DecoratedWindow>
#include <KDecoration3/DecorationSettings>
#include <KPluginFactory>
#include <QPainter>

K_PLUGIN_FACTORY_WITH_JSON(ArstotzkaDecorationFactory,
                           "metadata.json",
                           registerPlugin<Arstotzka::Decoration>();)

using namespace std;
using namespace KDecoration3;

namespace Arstotzka {
Decoration::Decoration(QObject* parent, const QVariantList& args)
    : KDecoration3::Decoration(parent, args) {}

bool
Decoration::init() {
    watcher = KConfigWatcher::create(KSharedConfig::openConfig("kdeglobals"));

    this->connectEvents();
    this->setBorderSizes();
    this->updateColors();

    return true;
}

void
Decoration::paint(QPainter* painter, const QRectF& repaintRegion) {
    if (!painter)
        return;

    const DecoratedWindow* window = this->window();
    const QRectF rect = this->rect();

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(window->isActive() ? active : inactive);
    painter->drawRect(rect);
    painter->restore();
}

void
Decoration::updateColors() {
    const KSharedConfig::Ptr colors = KSharedConfig::openConfig("kdeglobals");
    const KConfigGroup group = colors->group("Colors:Window");
    active = group.readEntry("DecorationFocus", QColor(255, 0, 0));
    inactive = group.readEntry("BackgroundNormal", QColor(0, 0, 0));
}

inline constexpr int
sizeToInt(const BorderSize size) noexcept {
    switch (size) {
        case BorderSize::Oversized:
            return 10;
        case BorderSize::VeryHuge:
            return 6;
        case BorderSize::Huge:
            return 5;
        case BorderSize::VeryLarge:
            return 4;
        case BorderSize::Large:
            return 3;
        case BorderSize::NoSides:
        case BorderSize::Normal:
            return 2;
        case BorderSize::None:
            return 0;
        case BorderSize::Tiny:
        default:
            return 1;
    }
}

void
Decoration::setBorderSizes() {
    const DecorationSettings* settings = this->settings().get();
    const BorderSize desired = settings->borderSize();
    const int base = settings->smallSpacing();
    const int size = base * sizeToInt(desired);
    const int sides = (BorderSize::NoSides == desired) ? 0 : size;

    this->setBorders(QMargins(sides, size, sides, size));
}

const QString General = QStringLiteral("General");
const QByteArray Scheme = QByteArrayLiteral("ColorScheme");
const QByteArray Accent = QByteArrayLiteral("AccentColor");

void
Decoration::connectEvents() {
    const DecoratedWindow* window = this->window();
    const DecorationSettings* settings = this->settings().get();
    const KConfigWatcher* const config = watcher.data();

    connect(
        window,
        &DecoratedWindow::activeChanged,
        this,
        [this](bool _______) { this->update(); }
    );

    connect(
        settings,
        &DecorationSettings::borderSizeChanged,
        this,
        &Decoration::setBorderSizes
    );

    connect(
        config,
        &KConfigWatcher::configChanged,
        this,
        [this](const KConfigGroup& group, const QByteArrayList& names) {
            bool general = group.name() == General;
            bool section = names.contains(Scheme) || names.contains(Accent);

            if (!general || !section)
                return;

            this->updateColors();
            this->update();
        }
    );
}
}  // namespace Arstotzka

#include "decoration.moc"
