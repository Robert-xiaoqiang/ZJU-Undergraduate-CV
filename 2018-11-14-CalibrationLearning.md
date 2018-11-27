---
title: CalibrationLearning
key: 201811140
tags: OpenCV CV
---

# CalibrationLearning

### Principles

#### 中心投影参数

- 相机内参
   - **f**, 焦距, 中心投影(perspective projection)模型中, 假设为投影中心, 距离成像平面(光屏)为**f**
   - **dx, dy**, 光屏平面的量子化, 离散化的单位, 即像素的物理尺寸(`nx * dx == screen width`)
   - **u0, v0**, 光轴与光屏的交点的像素/图像坐标(按照习惯, 图像坐标取左下为原点), 模型中, 认为u0, v0在图像坐标矩形中心
   - `(x, y, z) => {(u, v)}` 世界坐标映射像素坐标, 默认取z轴为主轴, 即投影方向的世界坐标为z轴(与图形学规定的方向正负相反)
   - ![a1]({{site.url}}/assets/BlogImages/2018-11-14/a1.PNG)
   - `Y / y = X / x = Z / f` 且`u = u0 + x / dx, v = v0 +y / dy`
   - ![a2]({{site.url}}/assets/BlogImages/2018-11-14/a2.PNG)
   - 由此得到内参矩阵
<!--more-->

- 畸变系数

   - 产生'筒形'或'鱼眼'现象, 径向畸变(x, y, r)为实际光屏上点坐标, 径向距离屏幕中心距离 

     ```python
     # 关于r的函数使用cos(r)的麦克劳林级数
     xcorrected = x(1 + k1*r^2 + k2*r^4 + k3*r^6)
     
     ycorrected = y(1 + k1*r^2 + k2*r^4 + k3*r^6)
     ```

   - 一个矩形被投影到成像仪上时, 可能会变成一个'梯形', 光屏与光轴不完全垂直, 产生切向畸变

     ```python
     # 仍然为麦克劳林级数, 使用p1, p2参数作为权重, 模拟畸变时的'轴偏转性'
     xcorrected = x + ( 2p1*y + p2*(r2 + 2x^2) )
     
     ycorrected = y + ( 2p2*x + p1*(r2 + 2y^2) )
     ```

      

- 相机外参

   - 旋转变换
     - 旋转矩阵R, 旋转向量(rx, ry, rz), 法向θ
     - ![a3]({{site.url}}/assets/BlogImages/2018-11-14/a3.PNG)

   - 平移变换
     - 平移向量(tx, ty, tz)

#### 数学与标定 ~~太难了~~
- 对于每一个视场, 假设没有畸变 得到单应性矩阵`H`, 包括相机内参(M)和外参(r1, r2, r3), r3 = 0, Z = 0
$$
H = \left[\begin{matrix}
h_1 & h_2 & h_3 \\
\end{matrix}] = sM
\begin{matrix}[
r_1 & r_2 & t \\
\end{matrix}\right] \\
其中, h_i 是列向量(3\times1)
$$
- 旋转变换是线性空间中一种正交变换, 变换矩阵是正交矩阵, 行列向量分别可以作为相应空间的单位正交基, 约束条件
$$
r_1^Tr_2 = 0 \\
r_1^Tr_1 = r_2^Tr_2 \\
$$
- 整理得到
$$
h_1^TM^{-T}M^{-1}h_2 = 0 \\
h_1^TM^{-T}M^{-1}h_1 = h_2^TM^{-T}M^{-1}h_2 \\
设B = M^{-T}M^{-1} =
\left[\begin{matrix}
B_{11} & B_{12} & B_{13} \\
B_{21} & B_{22} & B_{23} \\
B_{31} & B_{32} & B_{33} \\
\end{matrix}\right] \\
其中 M = 
\left[\begin{matrix}
f_x & 0 & u_0 \\
0 & f_y & v_0 \\
0 & 0 & 1\\
\end{matrix}\right]
$$
- 展开矩阵乘法得到最终求解公式
$$
h_i^TBh_j = 
\left[\begin{matrix}
h_{i1}h_{j1}B_{11} & h_{i1}h_{j2}B_{12} & h_{i1}h_{j3}B_{13} \\
h_{i2}h_{j1}B_{21} & h_{i2}h_{j2}B_{22} & h_{i2}h_{j3}B_{23} \\
h_{i3}h_{j1}B_{31} & h_{i3}h_{j2}B_{32} & h_{i3}h_{j3}B_{33} \\
\end{matrix}\right] \\
B为对称矩阵, 约束条件为矩阵等式, 或恒等于0, 化简为若干等式, 并再次写为向量表示为 \\
v_{ij}^Tb = \left[\begin{matrix}
h_{i1}h_{j1} \\ h_{i1}h_{j2} + h_{i2}h_{j1} \\ h_{i2}h_{j2} \\ h_{i1}h_{j3} + h_{i3}h_{j1} \\ h_{i2}h_{j3} + h_{i3}h_{j2} \\ h_{i3}h_{j3}
\end{matrix}\right]^T
\left[\begin{matrix}
B_{11} \\ B_{12} \\ B_{22} \\ B_{13} \\ B_{23} \\ B_{33} \\
\end{matrix}\right]
$$

- 关于正交矩阵`R`真实数据不正交, 对R进行奇异值分解，R=UDVT，U，V为正交阵，D为对角阵，如果R是正交阵，那么奇异值分解后的对角阵D是单位阵，那么我们将单位阵I代替对角阵D,进而重构出满足正交条件的R
- 考虑畸变
$$
\left[\begin{matrix}
x_p \\
y_p \\
\end{matrix}\right] = 
(1 + k_1r^2 + k_2r^4 + k_3r^6)
\left[\begin{matrix}
x_d \\
y_d \\
\end{matrix}\right] +
\left[\begin{matrix}
2p_1x_dy_d + p_2(r^2 + 2x_d^2) \\
2p_2x_dy_d + p_1(r^2 + 2y_d^2) \\
\end{matrix}\right] \\
其中x_p, y_p为由角点世界坐标(X, Y, Z)求得的像素坐标, x_d, y_d为实际图片上的角点像素坐标, 可立刻求解畸变系数
$$

### API in OpenCV

- 角点提取`cv::findChessboardCorners(image, pattern, 2dcorners, flag)`

- 角点亚像素提高精度`cv::cornerSubPix(image, 2dcorners, ROI, zeroZone, criteria)`

- 角点标注`cv::drawChessboardCorners(image, pattern, 2dcorners, flag)`
- 标定开始`cv::calibrateCamera(3dcorners, 2dcorners, size, innerMatrix, distortion, rotation, translation, flag, criteria)`
- 反向投影, 评价标定结果`cv::projectPoints(3dcorners, rotation, translation, innermatrix, distortion, 2dcorners, jacobian, ratioWH)`
- 去除畸变映射矩阵获取`cv::initUndistortRectifyMap(innerMatrix, distortion, R(c1与c2之间), newInnerMatrix = innerMatrix, size, mapType, mapx, mapy)`
- 图像去畸变`cv::remap(srcImage, dstImage, mapx, mapy, interpolation, borderMode, borderValue)`



### 尝试用Eigen实现矩阵运算求解过程

