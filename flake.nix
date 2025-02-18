{
  description = "KDE Plasma Window Decoration consisting of single colorful rectangle";

  outputs =
    {
      self,
      nixpkgs,
    }:
    let
      systems = [
        "aarch64-linux"
        "x86_64-linux"
      ] ++ nixpkgs.lib.optional (builtins ? currentSystem) builtins.currentSystem;

      build =
        f: attrs: system:
        let
          config = f system;
          merge =
            attrs: key:
            let
              base = (attrs.${key} or { });
              moved = {
                ${system} = config.${key};
              };
              new = {
                ${key} = base // moved;
              };
            in
            attrs // new;
        in
        builtins.foldl' merge attrs (builtins.attrNames config);

      eachSystem = f: builtins.foldl' (build f) { } systems;
    in
    eachSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        arstotzka = pkgs.callPackage ./package.nix { };
      in
      {
        packages = {
          inherit arstotzka;
          default = arstotzka;
        };
        devShells = {
          default = import ./shell.nix { inherit pkgs arstotzka; };
        };
      }
    );

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };
}
