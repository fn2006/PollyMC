{
  inputs,
  self,
  ...
}: {
  perSystem = {
    lib,
    pkgs,
    ...
  }: {
    packages = let
      ourPackages = lib.fix (final: self.overlays.default ({inherit (pkgs) darwin;} // final) pkgs);
    in {
      inherit
        (ourPackages)
        pollymc-qt5-unwrapped
        pollymc-qt5
        pollymc-unwrapped
        pollymc
        ;
      default = ourPackages.pollymc;
    };
  };

  flake = {
    overlays.default = final: prev: let
      version = builtins.substring 0 8 self.lastModifiedDate or "dirty";

      # common args for prismlauncher evaluations
      unwrappedArgs = {
        inherit (inputs) libnbtplusplus;
        inherit (final.darwin.apple_sdk.frameworks) Cocoa;
        inherit self version;
      };
    in {
      pollymc-qt5-unwrapped = prev.libsForQt5.callPackage ./pkg unwrappedArgs;
      pollymc-qt5 = prev.libsForQt5.callPackage ./pkg/wrapper.nix {
        pollymc-unwrapped = final.pollymc-qt5-unwrapped;
      };
      pollymc-unwrapped = prev.qt6Packages.callPackage ./pkg unwrappedArgs;
      pollymc = prev.qt6Packages.callPackage ./pkg/wrapper.nix {inherit (final) pollymc-unwrapped;};
    };
  };
}
