# How to import

To import with flakes use

```nix
{
  inputs = {
    polymc.url = "github:fn2006/PollyMC";
  };

...

  nixpkgs.overlays = [ inputs.pollymc.overlay ]; ## Within configuration.nix
  environment.systemPackages = with pkgs; [ pollymc ]; ##
}
```

To import without flakes use channels:

```sh
nix-channel --add https://github.com/fn2006/PollyMC/archive/master.tar.gz pollymc
nix-channel --update pollymc
nix-env -iA pollymc
```

or alternatively you can use

```nix
{
  nixpkgs.overlays = [
    (import (builtins.fetchTarball "https://github.com/fn2006/PollyMC/archive/develop.tar.gz")).overlay
  ];

  environment.systemPackages = with pkgs; [ pollymc ];
}
```
