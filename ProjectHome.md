Project _Recognition of overlapping 2D objects_ for Machine Vision class.

# Usage #
From command line:
```
overlaping_obj_det.exe --modelsdir ../baza/ --scenepath \
../baza/0102.jpg -m 0101.jpg,1201.jpg
```
Required arguments:
  * path to database dir (`modelsdir` or `b`),
  * path to scene images (`scenepath` or `s`),
  * list of model images (`models` or `m`).
To get list of all arguments, run:
```
overlaping_obj_det.exe -h
```
Optional arguments have predefined values.

**NOTE (1):** Implementation requires OpenCV2.1 library installed at your system. If you don't want to install it, then just copy dll files:
cv210.dll, cv210d.dll, cvaux210.dll, cvaux210d.dll, cxcore210.dll,
cxcore210d.dll, cxts210.dll, highgui210.dll, highgui210d.dll, ml210.dll, ml210d.dll,
opencv\_ffmpeg210.dll, opencv\_ffmpeg210d.dll, in `c:\Windows\System32` dir.

**NOTE (2):** Implementation is tested with WinXP OS. It should be able to run on newer Windows versions.
If you want to run it on Linux, you'll need to fix code so `g++` can compile it :(.

## Output ##
Output contains probabilites that scene contains certain model, for example:
```
1.)0101.jpg 1
2.)1201.jpg 0.335778
```

# Notes for making executables #
Current implementation is made for MS Visual Studio compiler.

Requirements:
  * OpenCV 2.1
  * Boost 1.4
  * TNT library
  * JAMA library
Remember to include those libraries in your VS project! (there are detailed instructions in project documentation, but they are written in Croatian language)