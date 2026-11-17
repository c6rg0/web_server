{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }: {
    packages = nixpkgs.lib.genAttrs [ "x86_64-linux" ] (system:
    let
      pkgs = import nixpkgs { inherit system; };
      lib = nixpkgs.lib;
      tests = true;
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
          gtest
        ];

        buildInputs = with pkgs; [ 
          clang-tools
          cloudflared
          apacheHttpd # for testing
        ];

        cmakeFlags = [
          (lib.optional (tests) "-DENABLE_TESTS=1")
        ];
        makeTarget = "web-server";
        enableParallelBuilding = true;

        installPhase = ''
          mkdir -p $out/bin
          mv src/web-server $out/bin/

          if [ -f "src/tests/tests" ]; then
            mv src/tests/tests $out/bin/
          fi
        '';
      };
    });

    defaultPackage = {
      x86_64-linux = self.packages.x86_64-linux.web-server;
    };
  };
}
