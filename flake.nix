{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: {
    packages = nixpkgs.lib.genAttrs [ "x86_64-linux" ] (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in
    rec {
      web-server = pkgs.stdenv.mkDerivation {
        pname = "web-server";
        version = "1.0.0";

        src = ./.;

        nativeBuildInputs = with pkgs; [ 
          gcc
          cmake
          ninja
        ];

        buildInputs = with pkgs; [ 
          clang-tools
          cloudflared
          apacheHttpd # for testing
        ];

        cmakeFlags = [];
        makeTarget = "web-server";
        enableParallelBuilding = true;

        installPhase = ''
          mkdir -p $out/bin
          mv src/web-server $out/bin/
        '';
      };
    });

    defaultPackage = {
      x86_64-linux = self.packages.x86_64-linux.web-server;
    };
  };
}
