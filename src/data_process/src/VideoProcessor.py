# venv/bin/python3

import cv2

name = input("Video name: ")
video_path = "/home/riley/Projects/riley/data/videos/" + name + ".mp4"
data_path = "/home/riley/Projects/riley/data/store/" + name + ".txt"

cap = cv2.VideoCapture(video_path)

okay, frame = cap.read()
frame_number = 0
print(f'Video appears to be {len(frame[0])} x {len(frame)}')


# Process each frame
while okay and frame_number < 20:
    frame_number += 1
    data = ""

    # Print frame number to keep updated on time to completion
    print(f"Frame number: {frame_number}")

    for row in frame:
        for pixel in row:
            luma = round(0.2126*pixel[0] + 0.7152*pixel[1] + 0.0722*pixel[2], 2)
            data += f"{luma},"
            # print(f'{frame_number}: {pixel[0]}, {pixel[1]}, {pixel[2]}')

    # Remove the last comma and add a new line
    data = data[:-1] + "\n"
    with open(data_path, "a") as file:
        file.write(data)

    okay, frame = cap.read()
