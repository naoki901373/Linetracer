import cv2
import numpy as np


class Movement_matrix:
    def __init__(self):
        self.x = np.array([0])
        self.y = np.array([0])
        self.theta = np.array([0])
        self.pulse = 10 # パルス波
        self.D = 0.152 # 車体横幅 [m]
        self.r = 0.057/2 # 車輪半径 [m]
        self.L = 2 * np.pi * self.r * self.pulse / 50

    def cal_delta_theta(self):
        return self.L / self.D
    
    def turn_left(self):
        theta = self.cal_delta_theta()
        X = self.D * np.sin(theta/2) * np.cos(self.theta[-1] + theta) + self.x[-1]
        Y = self.D * np.sin(theta/2) * np.sin(self.theta[-1] + theta) + self.y[-1]
        self.x = np.append(self.x, X)
        self.y = np.append(self.y, Y)
        self.theta = np.append(self.theta, self.theta[-1] + theta)
        return self.x, self.y, self.theta

    def turn_right(self):
        theta = self.cal_delta_theta()
        X = self.D * np.sin(theta/2) * np.cos(self.theta[-1] - theta) + self.x[-1]
        Y = self.D * np.sin(theta/2) * np.sin(self.theta[-1] - theta) + self.y[-1]
        self.x = np.append(self.x, X)
        self.y = np.append(self.y, Y)
        self.theta = np.append(self.theta, self.theta[-1] - theta)
        return self.x, self.y, self.theta

    def go_straight(self):
        theta = self.theta[-1]
        X = self.L * np.cos(theta) + self.x[-1]
        Y = self.L * np.sin(theta) + self.y[-1]
        self.x = np.append(self.x, X)
        self.y = np.append(self.y, Y)
        self.theta = np.append(self.theta, theta)
        return self.x, self.y, self.theta


class Movement_Handler(Movement_matrix):
    # 左折　直進　右折に対応したクラス
    def judge_movement(self,serial_string:str):
        if serial_string == 'l':
            self.turn_left()
            print('left')

        elif serial_string == 's':
            self.go_straight()
            print('straight')


        elif serial_string == 'r':
            self.turn_right()
            print('right')


        elif serial_string == 'e':
            # self.x.pop(-1)
            # self.y.pop(-1)
            # self.theta.pop(-1)
            print('error')
            raise Exception()

        else:
            print('no')


if __name__ == '__main__':
    import serial
    import matplotlib.pyplot as plt 
    import time
    ser = serial.Serial('COM7', 9600)
    a = Movement_Handler()
    st = time.time()
    while True:
        serial_str = ser.readline()
        serial_str = serial_str.decode()[0]
        print(serial_str)

        if serial_str == 'f':
            break
        a.judge_movement(serial_string = serial_str)
        print(a.x,a.y,a.theta)
        if time.time()-st >=30:
            break
    ser.close()
    print(f'length:{len(a.x)}')
    plot_length = len(a.x)
    start_point = np.array([a.x[0], a.y[0]])
    end_point = np.array([a.x[-1], a.y[-1]])
    vector = start_point - end_point
    X = []
    Y = []
    reverse_x = a.x[::-1]
    reverse_y = a.y[::-1]
    for i in range(plot_length):
        x = reverse_x[i] + vector[0] * (plot_length-i)/plot_length
        y = reverse_y[i] + vector[1] * (plot_length-i)/plot_length
        X.append(x)
        Y.append(y)
    X.reverse()
    Y.reverse()
    plt.plot(a.x, a.y)
    plt.plot(X, Y)
    plt.show()

