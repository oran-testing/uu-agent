{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShellNoCC {
    name = "cpp-development";

    buildInputs = [
      pkgs.gcc
      pkgs.cmake
      pkgs.gdb
      pkgs.clang-tools
      pkgs.lldb
      pkgs.boost
      pkgs.pkg-config
    ];

    shellHook = ''
      echo Welcome to c++ dsevelopment env
    '';

  }
