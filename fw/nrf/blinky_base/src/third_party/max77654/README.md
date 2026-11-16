# MAX77654 Driver

Philipp Schilk
PBL, D-ITET, ETH Zürich

Generic (Hardware/OS agnostic) driver for the MAX77654 PMIC IC.

## Usage

```c
// Create a handle struct, providing all relevant hardware-specific functions
// and configuration:
struct max77654_h h = {
  .read_regs = read_regs,
  .write_regs = write_regs,
  .adc_read = adc_read,
  .log = drvrlog,
  // Configuration:
  .conf = {
    .cid = 0xA,
    .main_bias_force_enable = true,
    .... many more settings .....
  },
};

// Configure the device:
if (max77654_init(&h) != E_MAX77654_SUCCESS) {
  printf("Driver Init failed!\n");
  return -1;
}

```

## Usage: Development

 - The driver and unit tests can be compiled and run via make:

```bash
# Compile + Run unit tests:
$ make all

# Clean:
$ make clean
```
 - All unit tests are found in `test/`. Additional test sources need to be added to the makefile.
 - All code should be formatted according to the `.clang-format` file provided:

```bash
$ make format
```

 - For LSP support using `clangd`, a `compile_commands.json` can be generated using [bear](https://github.com/rizsotto/Bear):

```bash 
$ make clean
$ bear -- make
```
