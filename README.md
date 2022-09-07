# 2019-PhotogrammetryAlgorithms

This coursework used Qt and C++ to desigh photogrammetry algorithms and to build the interface framework, including QMainWindow, QWidget, QFileDialog, Qimage, and QMessageBox.

## FILE DESCRIPTION:

### main.cpp, mainwindow.cpp, mainwindow.h

Wake up MainWindow class object.


### EXoperator.cpp, EXoperator.h
Two Point feature extraction algorithm.
#### 1. Moravec

![image](https://github.com/SongxiYoung/2019-PhotogrammetryAlgorithms/blob/main/img_folder/Moravec_result.png)

#### 2. Harris

![image](https://github.com/SongxiYoung/2019-PhotogrammetryAlgorithms/blob/main/img_folder/Harris_result.png)

### matching.cpp, matching.h
Match points of the same location on two photos.

![image](https://github.com/SongxiYoung/2019-PhotogrammetryAlgorithms/blob/main/img_folder/lpix.png)
![image](https://github.com/SongxiYoung/2019-PhotogrammetryAlgorithms/blob/main/img_folder/rpix.png)

### orientation.cpp, orientation.h


### resection.cpp, resection.h
Space resection, seeking external orientation elements.

### intersection.cpp, intersection.h
Space intersection, seeking ground photography coordinates.
