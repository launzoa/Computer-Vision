# Digital Image Processing

A standard Digital Image Processing (DIP) project developed entirely in modern C++ (C++17). This project is heavily focused on implementing low-level mathematical image operations from scratch. **OpenCV** is utilized strictly as a backend tool for reading and writing image matrices, while **Qt6** powers a highly interactive, responsive Graphical User Interface.



---



## UI/UX Design

The interface is deliberately designed taking inspiration from professional editing software workflows: The processed image (Output) is always firmly positioned on the right. The left panel (Input) serves as a dynamic preview window of the original image, displaying real-time overlays that illustrate the mathematical effects of the currently selected tool. This allows users to experiment with parameters and receive immediate visual feedback without irreversibly altering the original image.

For algorithms that require specific initial conditions (such as Mean/Median temporal filters that specifically target noise, or Histogram Equalization which operates on grayscale), the application dynamically injects artificial Salt & Pepper noise or grayscale tones into the left preview panel. This is purely for real-time illustrative purposes, helping the user visualize *why* and *how* the algorithm works.

By simply hovering the mouse over the processed result, the system tracks the cursor's coordinates and instantly displays a wealth of data about that exact pixel:

* Mapped original position `(X, Y)`

* `RGB` values

* Real-time `HSV` conversion (matching OpenCV standards)

* `Luminance` (Grayscale intensity)

  

---



## Filters and Operations

All core image processing logic is meticulously written from scratch in `src/operations.cpp`, utilizing modern C++ nested namespaces for architectural clarity.



### Color Transformations & Contrast Adjustments
* **Gray Scale:** Converts the image to grayscale using weighted luminance values: `R·0.299 + G·0.587 + B·0.114`.
* **Binarization:** Absolute black-and-white thresholding using a fixed midpoint (`127`) as the universal cutoff.
* **Limiarization (Thresholding):** Customizable black-and-white binarization based on a user-defined numeric parameter.
* **Gamma Correction:** Applies a non-linear brightness adjustment (Power-Law transformation) by customizing the constant `C` and the scaling factor `γ`.
* **RGB to HSV / HSV to RGB:** Complete mathematical conversions between absolute color spaces (Hue, Saturation, and Value/Luminosity) without relying on library shortcuts.
* **Histogram Equalization:** Uniform redistribution of pixel intensities based on the Cumulative Distribution Function (CDF) to maximize global contrast.



### Noise Reduction (Pixel-wise & Spatial)
* **Mean (pixel-wise):** Generates a stack of 10 images corrupted with Salt and Pepper noise and calculates the temporal average of each pixel to restore the image.
* **Median (pixel-wise):** Similarly stacks 10 corrupted images, but utilizes a partial sorting algorithm to find the exact temporal median, effectively eliminating noise spikes.
* **Mean Convolution:** A spatial sliding-window filter utilizing an adjustable `N×N` uniform kernel to smooth the image.



### Edge Detection
* **Sobel Edge Detection:** Calculates directional gradients (`Gx` and `Gy`) using specific convolution kernels to highlight vertical and horizontal edges.
* **Laplacian:** Utilizes a second-order derivative in convolution (where the kernel sum equals 0) to detect rapid intensity transitions across all directions.



### Spatial Interpolation (Resizing)
* **Nearest Neighbor:** A highly efficient interpolation method based on strict geometric distance, snapping output pixels to the closest source pixel.
* **Bilinear:** A weighted interpolation technique that evaluates a `2x2` neighborhood of source pixels to compute a smooth expansion.



---



## Compile and Execute



### Dependency Installation
Ensure you have the following prerequisites installed on your system before building the project:
* **C++ Compiler** (C++17 support highly recommended)
* **CMake** (3.10 or higher)
* **OpenCV** (Version 4.x)
* **Qt** (Version 6.x)



### Linux Build Instructions

You can clone, build, and run the project using the standard CMake workflow:

```bash
# 1. Clone the repository
git clone [https://github.com/launzoa/Computer-Vision.git](https://github.com/launzoa/Computer-Vision.git)

# 2. Navigate to the C++ project directory
cd Computer-Vision/Digital-Image-Processing/C++

# 3. Create a build directory
mkdir build
cd build

# 4. Configure the project with CMake
cmake ..

# 5. Compile the executable
cmake --build .

# 6. Run the application
./ImageFiltering
