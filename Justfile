set shell := ["bash", "-uc"]

target := "build"
lsp:= "compile_commands.json"
name := "arstotzka"

_default:
    @just --list

install: build
    sudo cmake --install {{target}}

uninstall:
    sudo rm /usr/lib/qt/plugins/org.kde.kdecoration2/{{name}}.so

build: setup
    cmake --build {{target}}

setup:
    cmake -S . -B {{target}} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

clean:
    -rm -rf {{target}} .cache {{lsp}}

dev: build
    ln -sf {{target}}/{{lsp}} {{lsp}}
