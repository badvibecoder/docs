package main

import "core:fmt"
import rl "vendor:raylib"

main :: proc() {
    // Initialize the window
    screen_width  : i32 = 800
    screen_height : i32 = 600
    rl.InitWindow(screen_width, screen_height, "Odin + Raylib: Code-Generated Pixel Character")
    defer rl.CloseWindow()

    rl.SetTargetFPS(60)

    // Define colors for our palette
    // T = Transparent, R = Red (Hat/Shirt), B = Blue (Overalls), S = Skin, H = Brown (Hair/Shoes)
    T :: 0
    R :: 1
    B :: 2
    S :: 3
    H :: 4

    // A classic 16x16 pixel layout for a retro character grid
    mario_grid := [16][16]i32{
        {T, T, T, T, T, R, R, R, R, R, T, T, T, T, T, T},
        {T, T, T, T, R, R, R, R, R, R, R, R, R, T, T, T},
        {T, T, T, T, H, H, H, S, S, B, S, T, T, T, T, T},
        {T, T, T, H, S, H, S, S, S, B, S, S, S, T, T, T},
        {T, T, T, H, S, H, H, S, S, S, B, S, S, H, T, T},
        {T, T, T, H, H, S, S, S, S, B, B, B, B, H, T, T},
        {T, T, T, T, T, S, S, S, S, S, S, S, T, T, T, T},
        {T, T, T, T, R, R, B, R, R, R, T, T, T, T, T, T},
        {T, T, T, R, R, R, B, R, R, B, R, R, R, T, T, T},
        {T, T, R, R, R, R, B, B, B, B, R, R, R, R, T, T},
        {T, T, S, S, R, B, S, B, B, S, B, R, S, S, T, T},
        {T, T, S, S, S, B, B, B, B, B, B, S, S, S, T, T},
        {T, T, S, S, B, B, B, B, B, B, B, B, S, S, T, T},
        {T, T, T, T, B, B, B, T, T, B, B, B, T, T, T, T},
        {T, T, T, H, H, H, T, T, T, T, H, H, H, T, T, T},
        {T, T, H, H, H, H, T, T, T, T, H, H, H, H, T, T},
    }

    // Convert the numeric grid into real RGBA pixels in RAM
    img := rl.GenImageColor(16, 16, rl.BLANK)
    
    for y: i32 = 0; y < 16; y += 1 {
        for x: i32 = 0; x < 16; x += 1 {
            color_index := mario_grid[y][x]
            pixel_color := rl.BLANK // Transparent by default
            
            switch color_index {
                case R: pixel_color = rl.RED
                case B: pixel_color = rl.BLUE
                case S: pixel_color = rl.BEIGE
                case H: pixel_color = rl.BROWN
            }
            
            rl.ImageDrawPixel(&img, x, y, pixel_color)
        }
    }

    // Upscale our 16x16 pixel drawing cleanly to a 40x40 size
    rl.ImageResizeNN(&img, 40, 40)
    defer rl.UnloadImage(img)

    // Load the final upscaled asset into your GPU
    texture := rl.LoadTextureFromImage(img)
    defer rl.UnloadTexture(texture)

    // Setup player position data
    block_pos  := rl.Vector2{400, 300}
    move_speed : f32 = 300.0

    // Main game loop
    for !rl.WindowShouldClose() {
        dt := rl.GetFrameTime()

        // Handle WASD Input
        if rl.IsKeyDown(.W) || rl.IsKeyDown(.UP)    do block_pos.y -= move_speed * dt
        if rl.IsKeyDown(.S) || rl.IsKeyDown(.DOWN)  do block_pos.y += move_speed * dt
        if rl.IsKeyDown(.A) || rl.IsKeyDown(.LEFT)  do block_pos.x -= move_speed * dt
        if rl.IsKeyDown(.D) || rl.IsKeyDown(.RIGHT) do block_pos.x += move_speed * dt

        // Render graphics
        rl.BeginDrawing()
        rl.ClearBackground(rl.RAYWHITE)

        rl.DrawText("Procedural Retro Character (No Outside Assets!)", 10, 10, 20, rl.DARKGRAY)
        
        // Draw the character
        rl.DrawTextureV(texture, block_pos, rl.WHITE)

        rl.EndDrawing()
    }
}
