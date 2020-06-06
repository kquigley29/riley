# RileyData

## Process
The python script, `process.py`, converts incoming video frames into a list of every pixel's luma.
The list is written to a csv file.  

Luma is calculated as follows:

```python
luma = 0.2126*red + 0.7152*green + 0.0722*blue
```
#### Requirements
* OpenCV for python 

```
pip3 install opencv-python
```

## Store
Creates a database model to store data about videos and targets.
#### Requirements
* SqlAlchemy and its utils
```
pip3 install sqlalchemy sqlalchemy_utils
```

## Videos
A collection of videos in mp4 format which will be used to train Riley.