import cv2
import numpy as np

R = 0.29
x = np.array([0])
y = np.array([0])
theta = np.array([0])


def cal_v_and_omega(f_right,f_left):
    v = R*(f_right+f_left)/(3200*2)
    omega = R*(f_right-f_left)/(3200*2)
    return v, omega

def calculate(x,y,theta,v,omega,t):
    x_element = v*np.cos(theta[-1])*t + x[-1]
    y_element = v*np.sin(theta[-1])*t + y[-1]
    theta_element = omega*t + theta[-1]
    x = np.append(x,x_element)
    y = np.append(y,y_element)
    theta =np.append(theta,theta_element)
    return x, y, theta

if __name__ == '__main__':
    R = 0.29
    x = np.array([0])
    y = np.array([0])
    theta = np.array([0])
    t = 0.001
    while True:
        v, omega = cal_v_and_omega(100, 150)
        # print(v,omega)
        x, y, theta = calculate(x, y, theta, v, omega, t)
        print(x,y,theta)
        # print(len(x),len(y),len(theta))
