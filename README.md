# CVLearning000

### principles
- [some LaTex BUG](http://qindomitable.top/2018/11/14/CalibrationLearning.html)
### build
- prerequisites
   - OpenCV >= 2.4`(make sure it has been installed in PATH)`
   - MinGW 7.1.0`(MSYS)`
- build
   - `cmake -G "MSYS Makefiles"`
   - `make`
- run
   - `work <capture> or <calibrate>`

### sth.
- `image/*.png` is capturing result
- `after_image/*.png` is undistorting result
- `calibration.result` is parameters of the camera
