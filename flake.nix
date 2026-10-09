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
          clang 
          gnumake
        ];

        buildInputs = with pkgs; [ 
          clang-tools
        ];

        buildPhase = '' 
          make -f server/Makefile BUILD_DIR="$TMPDIR/server"

          runHook postBuild
        '';

        installPhase = ''
          runHook preInstall

          mkdir -p $out/bin
          mv $TMPDIR/server/web-server $out/bin/
        '';
      };
    });

    defaultPackage = {
      x86_64-linux = self.packages.x86_64-linux.web-server;
    };
  };
}
