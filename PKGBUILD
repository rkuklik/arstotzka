_name=arstotzka
pkgname=$_name
pkgver=1.0.0
pkgrel=1
pkgdesc='Simple border for KDE Plasma 5'
arch=(x86_64)
url="https://github.com/rkuklik/${_name}"
license=(GPLv3)
source=("${_name}::git+${url}")
makedepends=('cmake' 'extra-cmake-modules' 'kcoreaddons' 'kconfig' 'kdecoration')
sha256sums=('SKIP')

build() {
    cmake -S "$_name" -B 'build'
    cmake --build 'build'
}

package() {
    DESTDIR="$pkgdir" cmake --install "build"
}
