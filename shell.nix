{
  pkgs ? import <nixpkgs>,
  arstotzka ? pkgs.callPackage ./package.nix { },
}:
pkgs.mkShell {
  inputsFrom = [
    arstotzka
  ];
  packages = with pkgs; [
    clang-tools
  ];

  TARGET = "build";
  shellHook = ''
    cmake -S . -B "$TARGET"
  '';
}
