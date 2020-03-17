import numpy as np
import math
import matplotlib as mpl
#mpl.use('TkAgg') #might be needed for clean closing of figures in OSX ...?
from matplotlib import pyplot as plt

def spaceship(spaceship_angle_degrees):

    """given a departure angle from earth this function calculates the minimum
        distance between a rocketship (with hardcoded parameters herein) and the
        planet saturn. """

    #constants
    G = 6.67384e-11;
    tolerance = 1.0; #this will ensure we don't ever get divide by zero if things collide
    plot_on = 1; #do we want to see plots?

    #each of the following arrays represent:
    #sun, earth, mars, jupiter, saturn, uranus

    m = np.array([1.9891e30, 1.9891e30/333000, 639e21, 1.898e27, 568.3e24, 8.68e25]) #mass
    x = np.array([0, 0.0333, 0.2200, 0.7494, 1.1524, 1.7955])*1e12 #x-position
    y = np.array([0, -0.1524, 0.0551, 0.2077, -1.2054, 2.2635])*1e12 #y-position
    vx = np.array([0, 2.7318, -0.3274, -0.3457, 0.6689, -0.5301])*1e4 #velocity in x
    vy = np.array([0, 0.5615, 2.3981, 1.2488, 0.5544, 0.4279])*1e4 #velocity in y

    # setup the spaceship
    m_spaceship = 1e6 #mass of the spaceship
    x_spaceship = x[2] #earth
    y_spaceship = y[2] #earth
    spaceship_speed = 7.7784e3; #m/s
    spaceship_angle_radians = spaceship_angle_degrees*math.pi/180;
    vx_spaceship = spaceship_speed*math.cos(spaceship_angle_radians);
    vy_spaceship = spaceship_speed*math.sin(spaceship_angle_radians);

    # our time-step
    days_per_step = 20;
    dt = days_per_step*24*3600; #time step (delta t)

    # set the maximum time based on the time for the shuttle to go past the
    # final planet
    max_distance = math.sqrt(x[len(x)-1]**2 + y[len(x)-1]**2)*1.2 #last planet with buffer
    max_time = max_distance/spaceship_speed;
    max_time_steps = math.ceil(max_time/dt);

    #storage allocatoon
    d_spaceship_mars = np.zeros(max_time_steps)
    d_spaceship_jupiter = np.zeros(max_time_steps)
    d_spaceship_saturn = np.zeros(max_time_steps)
    d_spaceship_uranus = np.zeros(max_time_steps)
    time = np.zeros(max_time_steps)
    
    #if we want to see plots
    if plot_on == 1:
        plot_limits_x = np.array([-3e12, 3e12]);
        plot_limits_y = np.array([-3e12, 3e12]);
    
        plt.figure(1)
        plt.scatter(x, y, edgecolors='b', facecolors='none')
        plt.scatter(x[4], y[4],  c=['g'])
        plt.pause(0.005)
        
    #start time-marching
    t = 0;
    for i in range(0, max_time_steps):
    
        #forces on each planet
        Fx = 0*x
        Fy = 0*y
        for j in range(0,len(x)):
            F = np.array([0.0, 0.0]);
            for jj in range(0, len(x)):
                if (j !=jj):
                    absR = math.sqrt((x[j] - x[jj])**2 + (y[j] - y[jj])**2 + tolerance**2)
                    rhat = (np.array([x[j], y[j]]) - np.array([x[jj], y[jj]]))/absR
                    F -= G*m[j]*m[jj]*rhat/absR**2;
            
            Fx[j] = F[0];
            Fy[j] = F[1];
    
    
        #simplified model with no force on the spaceship
        Fx_spaceship = 0;
        Fy_spaceship = 0;
    
        #update velocities of planets (dv/dt = acceleration = F/m)
        vx = vx + Fx/m*dt;
        vy = vy + Fy/m*dt;

        #update velocities on spaceship (dv/dt = acceleration = F/m)
        #no change because force assumed zero
        vx_spaceship = vx_spaceship + Fx_spaceship/m_spaceship*dt;
        vy_spaceship = vy_spaceship + Fy_spaceship/m_spaceship*dt;
    
        #update position of planets (dx/dt = v)
        x = x + vx*dt;
        y = y + vy*dt;
    
        #update position of spaceship (dx/dt = v)
        x_spaceship = x_spaceship + vx_spaceship*dt;
        y_spaceship = y_spaceship + vy_spaceship*dt;
    
        #plot if we want to, fill the target planet
        
        if plot_on == 1:
            plt.figure(1)
            plt.cla()
            plt.scatter(x, y, edgecolors='b', facecolors='none')
            plt.scatter(x[4], y[4], c='g')
            plt.scatter(x_spaceship, y_spaceship, marker='x', c='r')
            plt.xlim(plot_limits_x)
            plt.ylim(plot_limits_y)
            plt.pause(0.005)
            
        #extract a bunch of data in case we want to return different things
        r_earth = np.array([x[1], y[1]]) #postion vector of earth
        r_mars = np.array([x[2], y[2]]) #position vector of mars
        r_jupiter = np.array([x[3], y[3]]) #position vector of jupiter
        r_saturn = np.array([x[4], y[4]]); #position vector of saturn
        r_uranus = np.array([x[5], y[5]]); #position vector of uranus
        r_spaceship = np.array([x_spaceship, y_spaceship]);
    
        #distances over time
        d_spaceship_mars[i] = np.linalg.norm(r_spaceship - r_mars);
        d_spaceship_jupiter[i] = np.linalg.norm(r_spaceship - r_jupiter);
        d_spaceship_saturn[i] = np.linalg.norm(r_spaceship - r_saturn);
        d_spaceship_uranus[i] = np.linalg.norm(r_spaceship - r_uranus);

        #update time
        time[i] = t;
        t = t + dt;
        
    #show the plot
    plt.show()
    
    #minimum distances
    min_d_mars = np.min(d_spaceship_mars);
    min_d_jupiter = np.min(d_spaceship_jupiter);
    min_d_saturn = np.min(d_spaceship_saturn);
    min_d_uranus = np.min(d_spaceship_uranus);

    #return min distance to saturn
    return min_d_saturn


#main point of entry

min_d_saturn = spaceship(60)
print("minimum distance to saturn at that angle was", min_d_saturn)
plt.close("all")
