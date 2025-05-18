# MeshLite

**MeshLite** is a powerful yet lightweight Windows desktop application designed for seamless 3D model handling. Built with **C++** and **OpenGL**, it specializes in loading, visualizing, and efficiently simplifying complex 3D models. At its core, MeshLite employs the advanced **Quadric Error Metrics (QEM)** algorithm to significantly reduce mesh complexity, transforming high-polygon models into optimized low-polygon versions. This makes MeshLite an invaluable tool for developers working on performance-critical applications like games, simulations, or interactive experiences where model optimization is paramount.

---

## Key Features

### 3D Model Handling

* **Standard .obj Support:** Easily load your 3D models saved in the widely adopted **.obj format**.
* **Intuitive Import:** Utilizes the familiar **Windows File Dialog** with a dedicated **.obj file filter** for a quick and seamless import experience.
* **Automatic Scaling & Positioning:** Imported models are automatically scaled to fit the scene appropriately and positioned for optimal viewing, ensuring they are always ready for manipulation.

### Visualization Settings

* **Configurable Rendering:** Adjust how your 3D models are displayed with a range of customizable rendering options.
* **Dynamic Color Control:** Change the model's primary color directly via the integrated GUI.
* **Adjustable Lighting:** Take full control of the scene's illumination:
    * **Light Source Positioning:** Precisely control the virtual light source's position to highlight specific model features.
    * **Real-time Intensity Control:** Modify the light's intensity on the fly to achieve desired visual effects.
* **Real-time Updates:** The renderer, powered by **GLSL shaders**, dynamically updates to reflect all changes in color, lighting angle, and intensity, providing accurate real-time visualization.

### Model Simplification

* **QEM Algorithm:** Leverage the highly efficient **Quadric Error Metrics (QEM)** algorithm to intelligently simplify complex, high-polygon 3D models.
* **Target Vertex Control:** Define the desired level of simplification by setting a specific target number of vertices for the output model.
* **Parameter-Driven Simplification:** Initiate the simplification process based on user-defined parameters, such as a maximum edge length, giving you fine-grained control over the optimization.
* **Optimized Output:** Automatically generates a new, highly optimized low-polygon version of your original mesh, ready for performance-sensitive environments.

### User Interface

* **ImGui Integration:** Features an intuitive and lightweight user interface built with the powerful **ImGui** library.
* **Clean & Simple:** All controls for loading, visualizing, tweaking, and simplifying models are accessible via a clean, uncluttered interface, enabling a streamlined workflow with just a few clicks.

---

## Demonstrations

Vase 3d models with 4k, 1k, 100 and 20 points:

![image](https://github.com/user-attachments/assets/d642e7d7-4917-4a11-b5b6-245ace9aa1a9)
![image](https://github.com/user-attachments/assets/77020b5f-8707-47d3-8d1c-2a3846f49ff8)

---

## Tech Stack

* **C++:** The core programming language for the application.
* **OpenGL:** For powerful 3D graphics rendering.
* **GLFW:** A lightweight, robust library for creating windows with OpenGL contexts and handling input.
* **GLAD:** Used for loading OpenGL function pointers.
* **glm:** A header-only C++ mathematics library for graphics software.
* **GLSL (OpenGL Shading Language):** For writing shaders to control rendering pipeline stages.
* **Assimp:** A portable library to import various 3D model formats.
* **ImGui:** A bloat-free graphical user interface library for C++.
