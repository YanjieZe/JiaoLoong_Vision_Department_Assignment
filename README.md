# JiaoLoong_Vision_Department_Assignment

##### 迮炎杰 YanjieZe

## Assignment1（including 1.1 and 1.2) 2020.10.05
### 1.1 车牌区域识别以及提取</br>
代码效果：使用**边缘检测以及轮廓提取算法**将原图中的车牌提取，并用**透视变换**做出正视图。</br>
**车牌位置检测**结果如下图。</br>
![image](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1/carplateFind.jpg)</br>

**透视变换**结果如下图。</br>
![image](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1/plateFinnal.jpg)</br>

### 1.2 找出苹果</br>
代码效果：使用**边缘检测算法以及轮廓提取算法**将苹果位置画出来，由于苹果阴影部分较难区分，使用椭圆拟合圈出了苹果的大概位置。</br>
结果如下图。</br>
![image](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1/appleFind.jpg)

## Assignment1.3 2020.10.06

### 1.3 利用chess图片进行相机标定。</br>
角点显示图片如下。
![image](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1.3/chessCorners.jpg)
相机标定的具体结果见**Assignment1.3**中的[calibration_result_right.txt](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1.3/calibration_result_right.txt)。
具体参数如下：
```
相机内参数矩阵：

[913.1838519635979, 0, 642.675229085825;
 0, 766.0147575172565, 365.0250373802296;
 0, 0, 1]

畸变系数：

[-0.4338629777541601, 0.2967710571102112, -0.0004557785585483899, -0.0002240604911029394, -0.1410073068885603]
```
最后，使用**Matlab**的相机标定软件进行了相机标定，并将相机参数保存于**Assignment1.3**中的[cameraParameters.mat](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1.3/cameraParameters.mat),估计误差保存在**Assignment1.3**中的[estimationErrors.mat](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1.3/estimationErrors.mat)。</br>
使用Matlab进行相机标定效果如下图。
![image](https://github.com/YanjieZe/JiaoLoong_Vision_Department_Assignment/blob/main/assignment1.3/matlabCalibrator.png)

## Assignment2 2020.10.16

### 题目一（相机模型&坐标变换）
假设当前图像中有一个像素点，其像素坐标是:$\begin{pmatrix}u\\v\\1 \end{pmatrix} $

设相机内参矩阵为 $K$ 且忽略畸变;

该像素对应3d点在相机坐标系下的z坐标为$d$；

当前图像对应陀螺仪数据为$R_{ir}$；

陀螺仪坐标系和相机坐标系之间的旋转矩阵为$R_{ci}$，平移向量为$t_{ci}$；

其中$e_r$为陀螺仪参考坐标系，$e_i$为陀螺仪坐标系；$e_c$为相机坐标系。

求该像素点对应的3d点在$e_r$内的坐标t



### $$Solution:$$

$$d\begin{pmatrix}u\\v\\1 \end{pmatrix}=\begin{pmatrix}K& \vec 0\end{pmatrix}\begin{pmatrix} X_c\\Y_c\\Z_c\\1 \end{pmatrix}$$

$$格式：(3*1) = (3*4)*(4*1)$$ 



$$\begin{pmatrix} X_c\\Y_c\\Z_c\\1 \end{pmatrix}=\begin{pmatrix} R_{ci}&t_{ci}\\ \vec 0 &1\end{pmatrix}\begin{pmatrix} X_r\\Y_r\\Z_r\\1\end{pmatrix}$$

$$格式：(4*1)=(4*4)*(4*1)$$



$$Hence:$$

$$d\begin{pmatrix}u\\v\\1 \end{pmatrix}=\begin{pmatrix}K& \vec 0\end{pmatrix}\begin{pmatrix} R_{ci}&t_{ci}\\ \vec 0 &1\end{pmatrix}\begin{pmatrix} X_r\\Y_r\\Z_r\\1\end{pmatrix} =\begin{pmatrix}KR_{ci}, Kt_{ci}\end{pmatrix}\begin{pmatrix} X_r\\Y_r\\Z_r\\1\end{pmatrix} $$

