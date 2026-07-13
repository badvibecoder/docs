import cv2
import numpy as np

# 1. Configuration & File Paths
INPUT_FILE = "/home/user/Dev/python-general/video-test/input.mp4"
OUTPUT_FILE = "/home/user/Dev/python-general/video-test/output.mp4"

# --- NEW VARIABLE TO SET MAX DURATION ---
# Set to None to process the entire video, or an integer/float for a cap
MAX_DURATION_SECONDS = 15  

# Initialize video capture engine
cap = cv2.VideoCapture(INPUT_FILE)

if not cap.isOpened():
    print(f"Error: Could not open or find input video: {INPUT_FILE}")
    exit()

# Extract properties from the original video
fps = int(cap.get(cv2.CAP_PROP_FPS))
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))     # Should be 1920
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))   # Should be 1080
total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

# --- CALCULATE TARGET FRAME LIMIT ---
if MAX_DURATION_SECONDS is not None:
    target_frames = int(fps * MAX_DURATION_SECONDS)
    # Don't try to process more frames than the source video actually has
    total_frames_to_process = min(target_frames, total_frames)
    print(f"Limiting output to {MAX_DURATION_SECONDS} seconds ({total_frames_to_process} frames).")
else:
    total_frames_to_process = total_frames

print(f"Processing: {width}x{height} @ {fps} FPS. Target frames: {total_frames_to_process}")

# Setup video writer to output matching file specifications
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
out = cv2.VideoWriter(OUTPUT_FILE, fourcc, fps, (width, height))

# 2. Define Box Layout Grid (6 top horizontal, 3 left vertical)
BOX_SIZE = 200
BOX_POSITIONS = [
    (50, 50), (300, 50), (550, 50), (800, 50), (1050, 50), (1300, 50),
    (50, 300), (50, 550), (50, 800)
]

# Static metadata configuration for each sensor box
SENSOR_METADATA = {
    0: {"label": "TEMP",  "unit": "C"},
    1: {"label": "HUMID", "unit": "%"},
    2: {"label": "PRESS", "unit": "hPa"},
    3: {"label": "SPEED", "unit": "m/s"},
    4: {"label": "VOLTS", "unit": "V"},
    5: {"label": "AMPS",  "unit": "A"},
    6: {"label": "RPM",   "unit": "rpm"},
    7: {"label": "FLOW",  "unit": "L/m"},
    8: {"label": "CO2",   "unit": "ppm"}
}

# BGR color palette for container backdrops
BOX_BG_COLOR = (35, 35, 35)      
BOX_BORDER_COLOR = (180, 180, 180) 
TEXT_LABEL_COLOR = (0, 215, 255)   
TEXT_VALUE_COLOR = (255, 255, 255) 

frame_count = 0

# 3. Main Processing Frame-by-Frame Loop
while cap.isOpened():
    # --- CHECK DURATION LIMIT ---
    if frame_count >= total_frames_to_process:
        print(f"Reached targeted length cap of {MAX_DURATION_SECONDS} seconds.")
        break

    ret, frame = cap.read()
    if not ret:
        break  

    # --- Live Data Simulation ---
    live_readings = {
        0: 22.5 + np.sin(frame_count * 0.01) * 2.1,         
        1: 50.0 + np.cos(frame_count * 0.015) * 5.0,        
        2: 1011 + int(np.sin(frame_count * 0.005) * 4),     
        3: 8.2 + np.sin(frame_count * 0.03) * 3.5,          
        4: 12.1 + np.sin(frame_count * 0.002) * 0.3,        
        5: 2.5 + np.abs(np.cos(frame_count * 0.02) * 1.8),  
        6: 3200 + int(np.sin(frame_count * 0.04) * 250),    
        7: 1.4 + np.sin(frame_count * 0.025) * 0.4,         
        8: 410 + int(np.cos(frame_count * 0.008) * 20)      
    }

    # --- Draw Overlays on the Frame ---
    for idx, (x, y) in enumerate(BOX_POSITIONS):
        top_left = (x, y)
        bottom_right = (x + BOX_SIZE, y + BOX_SIZE)
        
        cv2.rectangle(frame, top_left, bottom_right, BOX_BG_COLOR, cv2.FILLED)
        cv2.rectangle(frame, top_left, bottom_right, BOX_BORDER_COLOR, 1)

        meta = SENSOR_METADATA[idx]
        val = live_readings[idx]
        
        title_text = meta["label"]
        if isinstance(val, float):
            data_text = f"{val:.1f} {meta['unit']}"
        else:
            data_text = f"{val} {meta['unit']}"

        cv2.putText(
            img=frame, 
            text=title_text, 
            org=(x + 15, y + 40), 
            fontFace=cv2.FONT_HERSHEY_SIMPLEX, 
            fontScale=0.6, 
            color=TEXT_LABEL_COLOR, 
            thickness=2
        )

        (text_w, text_h), baseline = cv2.getTextSize(data_text, cv2.FONT_HERSHEY_DUPLEX, 0.7, 2)
        center_x = x + int((BOX_SIZE - text_w) / 2) 
        
        cv2.putText(
            img=frame, 
            text=data_text, 
            org=(center_x, y + 115), 
            fontFace=cv2.FONT_HERSHEY_DUPLEX, 
            fontScale=0.7, 
            color=TEXT_VALUE_COLOR, 
            thickness=2
        )

    out.write(frame)
    frame_count += 1
    
    # Adjusted progress math to reflect the early cutoff percentage correctly
    if frame_count % (fps * 5) == 0:
        print(f"Render progress: {int((frame_count / total_frames_to_process) * 100)}% done...")

# 4. Clean up memory allocations and close files safely
cap.release()
out.release()
cv2.destroyAllWindows()

print(f"Success! Finished rendering video stream layout to -> {OUTPUT_FILE}")
