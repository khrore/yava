{
  description = "yava development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell rec {
          nativeBuildInputs = [ pkgs.pkg-config ];
          buildInputs = with pkgs; [
            llvmPackages.clangWithLibcAndBasicRtAndLibcxx
            mold
            llvmPackages.bintools

            alsa-lib
            wayland
            xorg.libX11
            xorg.libXcursor
            xorg.libXrandr
            xorg.libXi
            xorg.libXinerama
            wayland-scanner
            libxkbcommon
            libffi

            cmake
            ninja

            glfw
            glm
            libGL
            vulkan-headers
            vulkan-loader
            vulkan-tools
            vulkan-tools-lunarg
            vulkan-extension-layer
            vulkan-validation-layers
          ];

          EDITOR = "nvim";
          CC = "clang";
          CXX = "clang++";
        };
      }
    );
}
