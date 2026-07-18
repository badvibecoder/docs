package main

import "core:fmt"
import "core:strings"
import "vendor:glfw"
import gl "vendor:OpenGL"

// Vertex Shader: Passes the color attribute through to the Fragment Shader
vertex_shader_source := `#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
`

// Fragment Shader: Colors the pixels using interpolated vertex colors
fragment_shader_source := `#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor, 1.0);
}
`

main :: proc() {
    // 1. Initialize GLFW
    if glfw.Init() != glfw.TRUE {
        fmt.println("Failed to initialize GLFW")
        return
    }
    defer glfw.Terminate()

    glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

    window := glfw.CreateWindow(800, 600, "Odin Colored Triangle", nil, nil)
    if window == nil {
        fmt.println("Failed to create GLFW window")
        return
    }
    glfw.MakeContextCurrent(window)

    // Load OpenGL functions
    gl.load_up_to(3, 3, glfw.gl_set_proc_address)

    // 2. Build and compile the shader program
    vertex_shader := compile_shader(vertex_shader_source, gl.VERTEX_SHADER)
    fragment_shader := compile_shader(fragment_shader_source, gl.FRAGMENT_SHADER)

    shader_program := gl.CreateProgram()
    gl.AttachShader(shader_program, vertex_shader)
    gl.AttachShader(shader_program, fragment_shader)
    gl.LinkProgram(shader_program)

    // Clean up individual shaders once linked
    gl.DeleteShader(vertex_shader)
    gl.DeleteShader(fragment_shader)

    // 3. Set up Vertex Data (X, Y, Z coordinates followed by R, G, B colors)
    vertices := [?]f32{
        // Positions         // Colors
         0.0,  0.5, 0.0,     1.0, 0.0, 0.0, // Top vertex (Red)
         0.5, -0.5, 0.0,     0.0, 1.0, 0.0, // Bottom Right vertex (Green)
        -0.5, -0.5, 0.0,     0.0, 0.0, 1.0, // Bottom Left vertex (Blue)
    }

    VBO, VAO: u32
    gl.GenVertexArrays(1, &VAO)
    gl.GenBuffers(1, &VBO)

    gl.BindVertexArray(VAO)

    gl.BindBuffer(gl.ARRAY_BUFFER, VBO)
    gl.BufferData(gl.ARRAY_BUFFER, size_of(vertices), &vertices, gl.STATIC_DRAW)

    // Stride is the size of 6 f32 values (3 for position, 3 for color)
    stride := i32(6 * size_of(f32))

    // Position attribute pointer (location = 0)
    // Fixed: Must be explicitly cast to uintptr instead of passing nil
    gl.VertexAttribPointer(0, 3, gl.FLOAT, gl.FALSE, stride, uintptr(0))
    gl.EnableVertexAttribArray(0)

    // Color attribute pointer (location = 1)
    // Offset begins after the first 3 position coordinates
    color_offset := uintptr(3 * size_of(f32))
    gl.VertexAttribPointer(1, 3, gl.FLOAT, gl.FALSE, stride, color_offset)
    gl.EnableVertexAttribArray(1)

    // Unbind buffers safely
    gl.BindBuffer(gl.ARRAY_BUFFER, 0)
    gl.BindVertexArray(0)

    // 4. Render Loop
    for glfw.WindowShouldClose(window) == glfw.FALSE {
        if glfw.GetKey(window, glfw.KEY_ESCAPE) == glfw.PRESS {
            glfw.SetWindowShouldClose(window, glfw.TRUE)
        }

        // Dark grey background to make colors pop
        gl.ClearColor(0.15, 0.15, 0.15, 1.0)
        gl.Clear(gl.COLOR_BUFFER_BIT)

        // Draw our shape
        gl.UseProgram(shader_program)
        gl.BindVertexArray(VAO)
        gl.DrawArrays(gl.TRIANGLES, 0, 3)

        glfw.SwapBuffers(window)
        glfw.PollEvents()
    }

    // Clean up graphics memory before exiting
    gl.DeleteVertexArrays(1, &VAO)
    gl.DeleteBuffers(1, &VBO)
    gl.DeleteProgram(shader_program)
}

// Helper procedure to build shaders natively
compile_shader :: proc(source: string, type: u32) -> u32 {
    shader := gl.CreateShader(type)
    c_str := strings.clone_to_cstring(source)
    defer delete(c_str)
    
    gl.ShaderSource(shader, 1, &c_str, nil)
    gl.CompileShader(shader)
    
    // Check for compilation errors
    success: i32
    gl.GetShaderiv(shader, gl.COMPILE_STATUS, &success)
    if success == 0 {
        info_log: [512]u8
        gl.GetShaderInfoLog(shader, 512, nil, &info_log[0])
        fmt.eprintf("Shader Compilation Error: %s\n", string(info_log[:]))
    }
    
    return shader
}
