{
  inputs,
  self,
  version,
  ...
}: {
  perSystem = {pkgs, ...}: {
    packages = {
      inherit (pkgs) pollymc-qt5-unwrapped pollymc-qt5 pollymc-unwrapped pollymc;
      default = pkgs.pollymc;
    };
  };

  flake = {
    overlays.default = final: prev: let
      # Helper function to build prism against different versions of Qt.
      mkPrism = qt:
        qt.callPackage ./package.nix {
          inherit (inputs) libnbtplusplus;
          inherit self version;
        };
    in {
      pollymc-qt5-unwrapped = mkPrism final.libsForQt5;
      pollymc-qt5 = prev.pollymc-qt5.override {pollymc-unwrapped = final.pollymc-qt5-unwrapped;};
      pollymc-unwrapped = mkPrism final.qt6Packages;
      pollymc = prev.pollymc.override {inherit (final) pollymc-unwrapped;};
    };
  };
}
