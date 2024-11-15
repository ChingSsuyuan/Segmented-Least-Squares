# Segmented Least Squares （DP）

We have an integer k, and to find the best fit for the points with exactly k line segments.

## We aim to find a line L defined by the equation y = ax+b to minimize the error function:
<img src="https://github.com/ChingSsuyuan/Segmented-Least-Squares/blob/61890db5f015f014bfef2b78469d9710cdc5042b/Photos/P2.png" width="600" height="130">

## We can solve for a and b in closed-form as：
![2](https://github.com/ChingSsuyuan/Segmented-Least-Squares/blob/61890db5f015f014bfef2b78469d9710cdc5042b/Photos/P3.png)

So, given k ≤ n, the problem is to find a partition P into exactly k segments so as to minimize the sum of the least squares error over the segments in the partition. 

Load in the points in the file test.txt(first column is x-coordinates, and second column is y-coordinates) and determine the optimal partition of the points into k = 4 partitions. 
