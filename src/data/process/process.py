# riley/venv/bin/python3


import cv2

video_path = "/Users/keane/Projects/riley/roulette/data/videos/" + input("Video name: ")
data_path = "/Users/keane/Projects/riley/roulette/data/data_store/" + input("CSV file name: ")

cap = cv2.VideoCapture(video_path)

okay, frame = cap.read()
frame_number = 0
print(f'Video appears to be {len(frame[0])} x {len(frame)}')


# Process each frame
while okay:
    frame_number += 1
    csv_data = ""

    # Print frame number to keep updated on time to completion
    print(f"Frame number: {frame_number}")

    for row in frame:
        for pixel in row:
            luma = 0.2126*pixel[0] + 0.7152*pixel[1] + 0.0722*pixel[2]
            csv_data += f"{luma},"
            # print(f'{frame_number}: {pixel[0]}, {pixel[1]}, {pixel[2]}')

    # Remove the last comma and add a new line
    csv_data = csv_data[:-1] + "\n"
    with open(data_path, "a") as file:
        file.write(csv_data)

    okay, frame = cap.read()