import cv2
import numpy as np


class Movement_matrix:
    def __init__(self):
        self.R = 0.29
        self.x = np.array([0])
        self.y = np.array([0])
        self.theta = np.array([0])
        self.t = 0.01


    def cal_v_and_omega(self,f_right,f_left):
        self.v = self.R*(f_right+f_left)/(3200*2)
        self.omega = self.R*(f_right-f_left)/(3200*2)
        return self.v, self.omega

    def calculate(self):
        x_element = self.v*np.cos(self.theta[-1])*self.t + self.x[-1]
        y_element = self.v*np.sin(self.theta[-1])*self.t + self.y[-1]
        theta_element = self.omega*self.t + self.theta[-1]
        self.x = np.append(self.x,x_element)
        self.y = np.append(self.y,y_element)
        self.theta =np.append(self.theta,theta_element)
        print(self.x)
        return self.x, self.y, self.theta

if __name__ == '__main__':
    a = Movement_matrix()
    while True:
        a.cal_v_and_omega(100,150)
        a.calculate()
