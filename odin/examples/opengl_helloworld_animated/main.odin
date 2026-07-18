package main

import "core:fmt"
import "core:math"
import "core:strings"
import "vendor:glfw"
import gl "vendor:OpenGL"

vertex_shader_source := `#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
`

// Fragment Shader: Combines vertex colors with a time-based sine wave animation
fragment_shader_source := `#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform float u_time;

void main() {
    // Generate shifting color values between 0.5 and 1.0 based on time
    float r_shift = sin(u_time) * 0.25 + 0.75;
    float g_shift = cos(u_time) * 0.25 + 0.75;
    float b_shift = sin(u_time + 1.5) * 0.25 + 0.75;

    // Multiply the original vertex colors by our shifting waves
    vec3 final_color = ourColor * vec3(r_shift, g_shift, b_shift);
    FragColor = vec4(final_color, 1.0);
}
`

main :: proc() {
    if glfw.Init() != glfw.TRUE {
        fmt.println("Failed to initialize GLFW")
        return
    }
    defer glfw.Terminate()

    glfw.WindowHint(glfw.CONTEXT_VERSION_MAJOR, 3)
    glfw.WindowHint(glfw.CONTEXT_VERSION_MINOR, 3)
    glfw.WindowHint(glfw.OPENGL_PROFILE, glfw.OPENGL_CORE_PROFILE)

    window := glfw.CreateWindow(800, 600, "Odin Animated Triangle", nil, nil)
    if window == nil {
        fmt.println("Failed to create GLFW window")
        return
    }
    glfw.MakeContextCurrent(window)

    gl.load_up_to(3, 3, glfw.gl_set_proc_address)

    vertex_shader := compile_shader(vertex_shader_source, gl.VERTEX_SHADER)
    fragment_shader := compile_shader(fragment_shader_source, gl.FRAGMENT_SHADER)

    shader_program := gl.CreateProgram()
    gl.AttachShader(shader_program, vertex_shader)
    gl.AttachShader(shader_program, fragment_shader)
    gl.LinkProgram(shader_program)

    gl.DeleteShader(vertex_shader)
    gl.DeleteShader(fragment_shader)

    // Look up the uniform location (strings must be null-terminated C-strings)
    time_location := gl.GetUniformLocation(shader_program, "u_time")

    vertices := [?]f32{
        // Positions         // Colors
         0.0,  0.5, 0.0,     1.0, 0.0, 0.0, 
         0.5, -0.5, 0.0,     0.0, 1.0, 0.0, 
        -0.5, -0.5, 0.0,     0.0, 0.0, 1.0, 
    }

    VBO, VAO: u32
    gl.GenVertexArrays(1, &VAO)
    gl.GenBuffers(1, &VBO)

    gl.BindVertexArray(VAO)

    gl.BindBuffer(gl.ARRAY_BUFFER, VBO)
    gl.BufferData(gl.ARRAY_BUFFER, size_of(vertices), &vertices, gl.STATIC_DRAW)

    stride := i32(6 * size_of(f32))

    gl.VertexAttribPointer(0, 3, gl.FLOAT, gl.FALSE, stride, uintptr(0))
    gl.EnableVertexAttribArray(0)

    color_offset := uintptr(3 * size_of(f32))
    gl.VertexAttribPointer(1, 3, gl.FLOAT, gl.FALSE, stride, color_offset)
    gl.EnableVertexAttribArray(1)

    gl.BindBuffer(gl.ARRAY_BUFFER, 0)
    gl.BindVertexArray(0)

    for glfw.WindowShouldClose(window) == glfw.FALSE {
        if glfw.GetKey(window, glfw.KEY_ESCAPE) == glfw.PRESS {
            glfw.SetWindowShouldClose(window, glfw.TRUE)
        }

        gl.ClearColor(0.15, 0.15, 0.15, 1.0)
        gl.Clear(gl.COLOR_BUFFER_BIT)

        gl.UseProgram(shader_program)

        // 1. Get the current application runtime clock from GLFW
        current_time := glfw.GetTime()
        
        // 2. Pass the time value down to the active shader program
        gl.Uniform1f(time_location, f32(current_time))

        gl.BindVertexArray(VAO)
        gl.DrawArrays(gl.TRIANGLES, 0, 3)

        glfw.SwapBuffers(window)
        glfw.PollEvents()
    }

    gl.DeleteVertexArrays(1, &VAO)
    gl.DeleteBuffers(1, &VBO)
    gl.DeleteProgram(shader_program)
}

compile_shader :: proc(source: string, type: u32) -> u32 {
    shader := gl.CreateShader(type)
    c_str := strings.clone_to_cstring(source)
    defer delete(c_str)
    
    gl.ShaderSource(shader, 1, &c_str, nil)
    gl.CompileShader(shader)
    
    success: i32
    gl.GetShaderiv(shader, gl.COMPILE_STATUS, &success)
    if success == 0 {
        info_log: [512]u8
        gl.GetShaderInfoLog(shader, 512, nil, &info_log[0])
        fmt.eprintf("Shader Compilation Error: %s\n", string(info_log[:]))
    }
    
    return shader
}
