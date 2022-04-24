# 3D_Scanner
3D Scanner that able to scan objects with laser lidar that gets objects point cloud.



![scanner_gif](https://user-images.githubusercontent.com/79113497/164955075-7298cbb2-7960-4304-b98f-d7ef0cc742fc.gif)


# Working Principle
Laser distance sensor gets object distance and every each detection of object stepper motor takes one step, when stepper motor reaches to 360 degree
other stepper motor (Y Axis) takes one step for the raise laser sensor. With this method its repeat itself up to max Y axis distance or no object detection. Scanning
results write to SD card with module as txt file in xyz format. All commands managed with arduino boards.
