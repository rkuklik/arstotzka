{
  stdenv,
  lib,
  cmake,
  kdePackages,
  qt6,
}:
stdenv.mkDerivation rec {
  pname = "arstotzka";
  version = "0.0.0";
  src = lib.cleanSource ./.;

  nativeBuildInputs = [
    cmake
    kdePackages.extra-cmake-modules
    qt6.wrapQtAppsHook
  ];
  buildInputs = with kdePackages; [
    qt6.qtbase
    kconfig
    kconfigwidgets
    kcoreaddons
    kdecoration
  ];
  cmakeFlags = [
    "-DQT_MAJOR_VERSION=6"
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
  ];

  meta = {
    description = "Simple border for KDE Plasma 6";
    homepage = "https://github.com/rkuklik/${pname}";
    license = lib.licenses.gpl3Plus;
    inherit (kdePackages.kdecoration.meta) platforms;
  };
}
