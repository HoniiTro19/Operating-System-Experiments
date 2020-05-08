# README

操作系统课程实验，进程调度算法，银行家算法，磁盘调度算法的实现

## Requirements

test on

- C++（使用部分C++11特性）
- Code Blocks17.12

## Usage

package_name：进程调度算法/银行家算法/磁盘调度算法

open package_name.cbp

run main.cpp

- Especially for 银行家算法

  - Interface

    =正在加载模板=
    =数据初始化完毕=
    =初始数据存在安全序列=
    P3 P4 P1 P2 P0

    ---------------------------------------------------

    | 进程  Max矩阵 | Allocation矩阵 | Need矩阵 | Available向量 |
    ---------------------------------------------------

    | P0 | 7   5   3  | 0   1   0  | 7   4   3  |  3  3  2  |
    | P1 | 3   2   2  | 2   0   0  | 1   2   2  |           |
    | P2 | 9   0   2  | 3   0   2  | 6   0   0  |           |
    | P3 | 2   2   2  | 2   1   1  | 0   1   1  |           |
    | P4 | 4   3   3  | 0   0   2  | 4   3   1  |           |
    =请输入进程申请资源序列=
    =输入格式为(进程号,资源1,资源2,...,资源N)=

  - Input Example

    4 2 0 0
    =申请到的资源=
    进程    4
    资源向量2 0 0
    存在安全序列P3 P4 P1 P2 P0

    =当前进程资源数据=
    ---------------------------------------------------
    | 进程  Max矩阵 | Allocation矩阵 | Need矩阵 | Available向量 |
    ---------------------------------------------------
    | P0 | 7   5   3  | 0   1   0  | 7   4   3  |  1  3  2  |
    | P1 | 3   2   2  | 2   0   0  | 1   2   2  |           |
    | P2 | 9   0   2  | 3   0   2  | 6   0   0  |           |
    | P3 | 2   2   2  | 2   1   1  | 0   1   1  |           |
    | P4 | 4   3   3  | 2   0   2  | 2   3   1  |           |

## Acknowledgement

# [Qujihan](https://github.com/Qujihan)/**[OperatingSystem_Algorithm](https://github.com/Qujihan/OperatingSystem_Algorithm)**

