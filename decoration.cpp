#include "decoration.hpp"

#include <QPainter>

#include <KDecoration2/DecoratedClient>
#include <KDecoration2/DecorationSettings>
#include <KPluginFactory>
#include <KSharedConfig>

K_PLUGIN_FACTORY_WITH_JSON(ArstotzkaDecorationFactory,
                           "metadata.json",
                           registerPlugin<Arstotzka::Decoration>();)

using namespace std;
using namespace KDecoration2;

namespace Arstotzka {
Decoration::Decoration(QObject *parent, const QVariantList &args)
    : KDecoration2::Decoration(parent, args) {
}

bool Decoration::init() {
    configWatcher = KConfigWatcher::create(KSharedConfig::openConfig("kdeglobals"));

    setBorderSizes();
    connectEvents();
    updateColors();

    return true;
}

void Decoration::paint(QPainter *painter, const QRect &repaintRegion) {
    if (!painter) { return; }

    DecoratedClient *client = this->client();
    QRect windowRect = rect();

    painter->save();
    painter->setPen(Qt::NoPen);

    if (client->isActive()) {
        painter->setBrush(activeColor);
    } else {
        painter->setBrush(inactiveColor);
    }

    painter->drawRect(windowRect);
    painter->restore();
}

void Decoration::updateColors() {
    KSharedConfig::Ptr colorsConfig = KSharedConfig::openConfig("kdeglobals");
    KConfigGroup group = colorsConfig->group("Colors:Window");
    activeColor = group.readEntry("DecorationFocus", QColor(255, 0, 0));
    inactiveColor = group.readEntry("BackgroundNormal", QColor(0, 0, 0));
}

inline constexpr int sizeToInt(const BorderSize size, const int base) {
    switch (size) {
    case BorderSize::Oversized:
        return base * 10;
    case BorderSize::VeryHuge:
        return base * 6;
    case BorderSize::Huge:
        return base * 5;
    case BorderSize::VeryLarge:
        return base * 4;
    case BorderSize::Large:
        return base * 3;
    case BorderSize::NoSides:
    case BorderSize::Normal:
        return base * 2;
    case BorderSize::None:
        return 0;
    case BorderSize::Tiny:
    default:
        return base;
    }
}

void Decoration::setBorderSizes() {
    const DecoratedClient *client = this->client();
    const shared_ptr<DecorationSettings> config = settings();
    const BorderSize desired = config->borderSize();
    const int base = config->smallSpacing();
    const int size = sizeToInt(desired, base);
    const int sides = (desired == BorderSize::NoSides) ? 0 : size;

    setBorders(QMargins(sides, size, sides, size));
}

void Decoration::connectEvents() {
    DecoratedClient *clientPtr = this->client();
    DecorationSettings *settingsPtr = settings().get();

    connect(clientPtr,
            &DecoratedClient::activeChanged,
            this,
            [this](bool thisIsLongToPreventFmtCollapse) { this->update(); });

    connect(settingsPtr,
            &DecorationSettings::borderSizeChanged,
            this,
            &Decoration::setBorderSizes);

    connect(configWatcher.data(),
            &KConfigWatcher::configChanged,
            this,
            [this](const KConfigGroup &group, const QByteArrayList &names) {
                if (group.name() != QStringLiteral("General")) { return; }
                if (false // I want nice alignment
                    || names.contains(QByteArrayLiteral("ColorScheme"))
                    || names.contains(QByteArrayLiteral("AccentColor"))) {
                    updateColors();
                    this->update();
                }
            });
}
}

#include "decoration.moc"
