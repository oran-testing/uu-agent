{
  description = "UU Interface Agent";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: 
  let
    pkgs = import nixpkgs { system = "x86_64-linux"; };
    nativeBuildInputs = with pkgs; [
      gcc
      cmake
      boost
      czmq
      zeromq
      uhd
      yaml-cpp
      pkg-config
      ninja
    ];
  in
  {
    inherit nativeBuildInputs;

    packages.x86_64-linux.default = pkgs.stdenv.mkDerivation {
      pname = "uuagent";
      version = "1.0";

      src = ./.;
      inherit nativeBuildInputs;

      installPhase = ''
        mkdir -p $out/bin
        cp uuagent/uuagent $out/bin
      '';
    };

    devShells.x86_64-linux.default = pkgs.mkShell {
      inherit nativeBuildInputs;
    };

  };
}
