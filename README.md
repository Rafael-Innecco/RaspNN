
# RaspNN

This project aims to develop a Neural Network in the C programming language using specific Raspberry Pi hardware resources to accelerate training and inference.

The hardware acceleration chosen for this project is ARM NEON, which is an ARM vector processor capable of processing 128 bits in parallel (in the v7 version). To use this, the project utilizes the `<arm_neon.h>` library (also known as ARM Intrinsics), which contains several wrappers for the assembly instructions of this vector processor.

To simplify porting this project to other accelerators, the ARM NEON logic was encapsulated in the files `rasp/src/source/matrix.c` and `rasp/src/source/math_func.c`.

The Neural Network developed for this project has only two layers (input and output, with no hidden layers) and its goal is to recognize digits from the MNIST dataset. Its logic is contained in the file `rasp/src/source/neural_network.c`.

Furthermore, a Client-Server Application (using the TCP protocol) was created to enable communication between the Raspberry Pi and a Computer. In this way, any computer can send images and labels to the Raspberry Pi so it can perform the network’s training/inference.

This project was tested on a Raspberry Pi 3B+, but it is believed that any ARM processor supporting ARM NEON v7 will be able to run this project without issues. However, pay attention to the compiler used, as it has been verified that several cross-compilers did not generate the correct NEON machine instructions. Therefore, it is recommended to use native compilers, and it was particularly noted that the compiler from Raspbian OS Lite generates the expected binaries.

## Usage Instructions

To compile and run the Raspberry Pi server application as well as the Desktop client application, simply use the correct targets in the Makefile located in the root of this directory.

First, create the `build` directories inside the `desktop` and `rasp` directories, as shown below:

```bash
mkdir rasp/build/
mkdir desktop/build/
```

To compile and run the server on the Raspberry Pi, use the command:

```bash
make
```

To compile the server on the Raspberry Pi:

```bash
make compile
```

To test the functions of the matrix and math_func on the Raspberry Pi:

```bash
make tests
```

To run the server on the Raspberry Pi:

```bash
make run
```

To run the server on the Raspberry Pi with [Valgrind](https://valgrind.org/), use the command:

```bash
make check
```

To generate the assembly language files for the Raspberry Pi, use the command:

```bash
make assembly
```

To compile the client on the desktop:

```bash
make build_client
```

To clean up the created executables:

```bash
make clean
```

## Instructions for Running the Comparative Neural Network

Inside the `examples/slow_neural_network` folder, there is a simplified version of the client-server setup which can be executed on common computers for performance comparison purposes (a Raspberry Pi 3B+ took only 3.7 seconds to run the optimized version!).

For this, first navigate to the `examples/slow_neural_network/server` folder and run the command to start the server application in the terminal.

```bash
make run
```

Then, in another terminal, navigate to the `examples/slow_neural_network/client` folder and execute the same command

```bash
make run
```

With this, the training will automatically start without any additional action, and shortly, the terminal where the server was started will display the training execution time. After the training is complete, both programs will automatically terminate.
