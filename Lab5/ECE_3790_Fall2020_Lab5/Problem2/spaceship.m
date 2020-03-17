function results = spaceshipSimulation(spaceship_angle_degrees)

%given a departure angle from earth this function calculates the minimum
%distance between a rocketship (with hardcoded parameters herein) and the
%planet saturn.

%constants
G = 6.67384e-11;
tolerance = 1; %this will ensure we don't ever get divide by zero if things collide
plot_on = 1; %do we want to see plots?

%each of the following arrays represent:
%sun, earth, mars, jupiter, saturn, uranus

m = [1.9891e30 1.9891e30/333000 639e21 1.898e27 568.3e24 8.68e25]'; %mass
x = [0 0.0333 0.2200 0.7494 1.1524 1.7955]'*1e12; %x-position
y = [0 -0.1524 0.0551 0.2077 -1.2054 2.2635]'*1e12; %y-position
vx = [0 2.7318 -0.3274 -0.3457 0.6689 -0.5301].'*1e4; %velocity in x
vy = [0 0.5615 2.3981 1.2488 0.5544 0.4279].'*1e4; %velocity in y

% setup the spaceship 
m_spaceship = 1e6; %mass of the spaceship 
x_spaceship = x(2); %earth
y_spaceship = y(2); %earth
spaceship_speed = 7.7784e3; %m/s
spaceship_angle_radians = spaceship_angle_degrees*pi/180;
vx_spaceship = spaceship_speed*cos(spaceship_angle_radians);
vy_spaceship = spaceship_speed*sin(spaceship_angle_radians);

% our time-step
days_per_step = 20;
dt = days_per_step*24*3600; %time step (delta t)

% set the maximum time based on the time for the shuttle to go past the
% final planet
max_distance = sqrt(x(end)^2 + y(end)^2)*1.2; %last planet with buffer
max_time = max_distance/spaceship_speed;
max_time_steps = ceil(max_time/dt);

%if we want to see plots
if plot_on == 1
    plot_limits_x = [-3e12 3e12];
    plot_limits_y = [-3e12 3e12];
    figure(3141593);
    scatter(x,y)
    xlim(plot_limits_x);
    ylim(plot_limits_y);
end

%start time-marching
t = 0;
for i = 1:max_time_steps
    
    %forces on each planet
    Fx = 0*x;
    Fy = 0*y;
    for j = 1:length(x)
        F = [0 0]';
        for jj = 1:length(x)
            if (j ~=jj)
                absR = sqrt((x(j) - x(jj))^2 + (y(j) - y(jj))^2 + tolerance^2);
                rhat = ([x(j); y(j)] - [x(jj); y(jj)])/absR;
                F = F - G*m(j)*m(jj)*rhat/absR^2;
            end
        end
        Fx(j) = F(1);
        Fy(j) = F(2);
    end
    
    %simplified model with no force on the spaceship
    Fx_spaceship = 0;
    Fy_spaceship = 0;
    
    %update velocities of planets (dv/dt = acceleration = F/m) 
    vx = vx + Fx./m*dt;
    vy = vy + Fy./m*dt;

    %update velocities on spaceship (dv/dt = acceleration = F/m)
    %no change because force assumed zero
    vx_spaceship = vx_spaceship + Fx_spaceship./m_spaceship*dt;
    vy_spaceship = vy_spaceship + Fy_spaceship./m_spaceship*dt;
    
    %update position of planets (dx/dt = v)
    x = x + vx*dt;
    y = y + vy*dt;
    
    %update position of spaceship (dx/dt = v)
    x_spaceship = x_spaceship + vx_spaceship*dt;
    y_spaceship = y_spaceship + vy_spaceship*dt;
    
    %plot if we want to, fill the target planet
    if plot_on == 1
        scatter(x,y)      
        hold on
        scatter(x(5), y(5), 'filled', 'go');
        scatter(x_spaceship, y_spaceship,'rx')    
        hold off
        xlim(plot_limits_x);
        ylim(plot_limits_y);
        pause(0.0001);
        refresh;
    end
    
    %extract a bunch of data in case we want to return different things
    r_earth = [x(2); y(2)]; %postion vector of earth
    r_mars = [x(3); y(3)]; %position vector of mars
    r_jupiter = [x(4); y(4)]; %position vector of jupiter
    r_saturn = [x(5); y(5)]; %position vector of saturn
    r_uranus = [x(6); y(6)]; %position vector of uranus
    r_spaceship = [x_spaceship; y_spaceship];
    
    %distances over time
    d_spaceship_mars(i) = norm(r_spaceship - r_mars);
    d_spaceship_jupiter(i) = norm(r_spaceship - r_jupiter);
    d_spaceship_saturn(i) = norm(r_spaceship - r_saturn);
    d_spaceship_uranus(i) = norm(r_spaceship - r_uranus);

    %update time
    time(i) = t;
    t = t + dt;
    
end

%minimum distances
min_d_mars = min(d_spaceship_mars);
min_d_jupiter = min(d_spaceship_jupiter);
min_d_saturn = min(d_spaceship_saturn);
min_d_uranus = min(d_spaceship_uranus);

%return min distance to saturn
results = min_d_saturn;

