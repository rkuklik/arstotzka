#include "decoration.hpp"

#include <QPainter>

#include <KDecoration2/DecoratedClient>
#include <KDecoration2/DecorationSettings>
#include <KPluginFactory>
#include <KSharedConfig>

K_PLUGIN_FACTORY_WITH_JSON(ArstotzkaDecorationFactory,
                           "metadata.json",
                           registerPlugin<Arstotzka::Decoration>();)

namespace Arstotzka {
Decoration::Decoration(QObject *parent, const QVariantList &args)
    : KDecoration2::Decoration(parent, args) {
}

void Decoration::init() {
    m_kdeglobalsWatcher = KConfigWatcher::create(KSharedConfig::openConfig("kdeglobals"));

    setBorderSizes();
    connectEvents();
    updateColors();
}

void Decoration::paint(QPainter *painter, const QRect &repaintRegion) {
    if (!painter) { return; }

    auto client = this->client().lock();
    auto windowRect = rect();

    painter->save();
    painter->setPen(Qt::NoPen);

    if (client->isActive()) {
        painter->setBrush(m_activeColor);
    } else {
        painter->setBrush(m_inactiveColor);
    }

    painter->drawRect(windowRect);
    painter->restore();
}

void Decoration::updateColors() {
    auto colorsConfig = KSharedConfig::openConfig("kdeglobals");
    auto group = colorsConfig->group("Colors:Window");
    m_activeColor = group.readEntry("DecorationFocus", QColor(255, 0, 0));
    m_inactiveColor = group.readEntry("BackgroundNormal", QColor(0, 0, 0));
}

void Decoration::setBorderSizes() {
    const auto client = this->client().lock();

    const int left = (settings()->borderSize() == KDecoration2::BorderSize::NoSides)
        ? 0
        : borderSize();
    const int right = (settings()->borderSize() == KDecoration2::BorderSize::NoSides)
        ? 0
        : borderSize();
    const int top = borderSize();
    const int bottom = borderSize();

    setBorders(QMargins(left, top, right, bottom));
}

void Decoration::connectEvents() {
    auto clientPtr = this->client().lock().data();
    auto settingsPtr = settings().data();

    connect(clientPtr,
            &KDecoration2::DecoratedClient::activeChanged,
            this,
            [this](bool value) { this->update(); });

    connect(settingsPtr,
            &KDecoration2::DecorationSettings::borderSizeChanged,
            this,
            &Decoration::setBorderSizes);

    connect(m_kdeglobalsWatcher.data(),
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

int Decoration::borderSize() const {
    const int baseSize = settings()->smallSpacing();
    switch (settings()->borderSize()) {
    case KDecoration2::BorderSize::Oversized:
        return baseSize * 10;
    case KDecoration2::BorderSize::VeryHuge:
        return baseSize * 6;
    case KDecoration2::BorderSize::Huge:
        return baseSize * 5;
    case KDecoration2::BorderSize::VeryLarge:
        return baseSize * 4;
    case KDecoration2::BorderSize::Large:
        return baseSize * 3;
    case KDecoration2::BorderSize::NoSides:
    case KDecoration2::BorderSize::Normal:
        return baseSize * 2;
    case KDecoration2::BorderSize::None:
        return 0;
    case KDecoration2::BorderSize::Tiny:
    default:
        return baseSize;
    }
}
}

#include "decoration.moc"
