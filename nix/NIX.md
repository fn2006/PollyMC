# Running on Nix

## Putting it in your system configuration

### On flakes-enabled nix

#### Directly installing

The `pollymc` flake provides a package which you can install along with
the rest of your packages

```nix
# In your flake.nix:
{
  inputs = {
    pollymc.url = "github:fn2006/PollyMC";
  };
}
```

```nix
# And in your system configuration:
environment.systemPackages = [ pollymc.packages.${pkgs.system}.pollymc ];

# Or in your home-manager configuration:
home.packages = [ pollymc.packages.${pkgs.system}.pollymc ];
```

#### Using the overlay

Alternatively, you can overlay the pollymc version in nixpkgs which will
allow you to install using `pkgs` as you normally would while also using the
latest version

```nix
# In your flake.nix:
{
  inputs = {
    pollymc.url = "github:fn2006/PollyMC";
  };
}
```

```nix
# And in your system configuration:
nixpkgs.overlays = [ inputs.pollymc.overlay ];
environment.systemPackages = [ pkgs.pollymc ];

# Or in your home-manager configuration:
config.nixpkgs.overlays = [ inputs.pollymc.overlay ];
home.packages = [ pkgs.pollymc ];
```

### Without flakes-enabled nix

#### Using channels

```sh
nix-channel --add https://github.com/fn2006/PollyMC/archive/master.tar.gz pollymc
nix-channel --update pollymc
nix-env -iA pollymc
```

#### Using the overlay

```nix
# In your configuration.nix:
{
  nixpkgs.overlays = [
    (import (builtins.fetchTarball "https://github.com/fn2006/PollyMC/archive/develop.tar.gz")).overlay
  ];

  environment.systemPackages = with pkgs; [ pollymc ];
}
```

## Running ad-hoc

If you're on a flakes-enabled nix you can run the launcher in one-line

```sh
nix run github:fn2006/PollyMC
```
