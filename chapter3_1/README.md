
# FreeRTOS on a Raspberry Pico with Multicore support

From [PicoFreertosBlink](https://github.com/ghubcoder/PicoFreertosBlink) but brought here for convenience

# Setup 

Checkout repo and do on submodules
```
git submodule update --init
```

# Building

```sh
mkdir build && cd build/
cmake ..
make
```

Blink code is a modified version of the code available in [this](https://github.com/yunkya2/pico-freertos-sample) repository.

More [here](https://ghubcoder.github.io/posts/using-multiple-cores-pico-freertos/)
