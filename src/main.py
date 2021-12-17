import numpy as np
from time import sleep



class Movement_matrix:

    # 低レイヤークラス

    def __init__(self):
        self.R = 0.29 # 車輪半径[m]
        self.d = 0.4  # 車輪と車体中心までの距離[m]
        self.pulse_num = 40 # パルス数
        self.x = np.array([0.1]) # x座標のlist
        self.y = np.array([0.1]) # y座標のlist
        self.theta = np.array([0]) # thetaのlist
        self.t = 0.01 # 時間 [s]

    def calculate_theta_l(self):
        theta_element = self.theta[-1] + 2 * np.pi * self.R * self.pulse_num / 1600
        self.theta = np.append(self.theta, theta_element)
        # print(self.theta[-1])
        return self.theta

    def calculate_theta_r(self):
        theta_element = self.theta[-1] - 2 * np.pi * self.R * self.pulse_num / 1600
        self.theta = np.append(self.theta, theta_element)
        # print(self.theta[-1])
        return self.theta

    def calculate_theta_s(self):
        theta_element = self.theta[-1]
        self.theta = np.append(self.theta, theta_element)
        # print(self.theta[-1])
        return self.theta

    def calculate_x_and_y(self):
        x_element = np.cos(self.theta[-2]) * self.x[-1] - np.sin(self.theta[-2]) * self.y[-1]
        y_element = np.sin(self.theta[-2]) * self.x[-1] + np.cos(self.theta[-2]) * self.y[-1]
        self.x = np.append(self.x,x_element)
        self.y = np.append(self.y,y_element)
        # print(self.x[-1])
        # print(self.y[-1])
        return self.x[-1], self.y[-1], self.theta[-1]



class Movement_Handler(Movement_matrix):
    # 左折　直進　右折に対応したクラス
    def turn_left(self):
        self.calculate_theta_l()
        self.calculate_x_and_y()

    def go_straight(self):
        self.calculate_theta_s()
        self.calculate_x_and_y()

    def turn_right(self):
        self.calculate_theta_r()
        self.calculate_x_and_y()

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
            pass
            print('error')


        else:
            print('no')
    

if __name__ == '__main__':
    import serial
    ser = serial.Serial('COM3', 9600)
    a = Movement_Handler()
    while True:
        serial_str = ser.read()
        print(serial_str)
        serial_str = serial_str.decode()[0]
        print(serial_str)
        a.judge_movement(serial_string = serial_str)
        sleep(0.1)
        print(a.x[-1],a.y[-1],a.theta[-1])
    ser.close()