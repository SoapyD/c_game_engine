
# Section 2 Notes

## 4.1.2 - OpenGL Initialization

In this section we added the `openGL` library to the `main.cpp` file, which is accessed via the `glew` library.

We initialised the `openGL` version to be used then bound the `window` pointer to it.

### Colours

`OpenGL` colours are stored in `4 bytes`, 4 channels containing `1 bytes (8 bits)` each, resulting in `256` values per channel.

![alt text](images/2-4.1.2-colours.png "Colours")

We set each channel value using `floats` as these are then converted in the package to `ints`. This apparently is because it's easier for us to think of these values in `OpenGL` as `on or off` values instead. And as OpenGL is a math API and, it's dealing with `blending, interpolation, lighting and shading`; which are all floating-point math.

### Double Buffering

`double-buffering` is a technique used in graphics APIs to stop flickering and visual artifacts during rendering.

All drawing is done on the back buffer, then once everything is ready we swap the buffers.

![alt text](images/2-4.1.2-double_buffering.png "Double Buffering")

We do this by running:

```
glfwSwapBuffers(window);
```