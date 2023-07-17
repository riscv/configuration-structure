# Configuration Structure C Library

Install dependencies:
```
sudo apt install asn1c crossbuild-essential-riscv64 ninja-build
pip3 install --user meson
```

To build for RISCV-64:
```
meson setup build64 --cross-file riscv64-unknown-elf
cd build64
ninja
```

Native build:
```
meson setup build
cd build
ninja
```

The native build includes an example program that reads a file in UPER format,
and prints its contains using ASN.1 Value Notation to stdout. Run it like this:
```
./example ../../example.uper
```
