xe = -0.3
ye = 0.3

-- Link lengths (meters)
local L1 = 0.5
local L2 = 0.5
local L3 = 0.5

-- PID gains
local Kp = 2 -- Proportional gain
local Ki = 0 -- Integral gain
local Kd = 0 -- Derivative gain

-- Error storage
local previous_error_x = 0
local previous_error_y = 0
local integral_error_x = 0
local integral_error_y = 0

-- Helper function to convert degrees to radians
local function deg2rad(degrees)
    return degrees * math.pi / 180
end

-- Helper function to convert radians to degrees
local function rad2deg(radians)
    return radians * 180 / math.pi
end

local function calculate_forward_kinematics(theta0, theta1, theta2)
    local x = L1 * math.cos(theta0) + L2 * math.cos(theta0 + theta1) + L3 * math.cos(theta0 + theta1 + theta2)
    local y = L1 * math.sin(theta0) + L2 * math.sin(theta0 + theta1) + L3 * math.sin(theta0 + theta1 + theta2)
    return x, y
end

local function calculate_jacobian(theta0, theta1, theta2)
  local j11 = -L1 * math.sin(theta0) - L2 * math.sin(theta0 + theta1) - L3 * math.sin(theta0 + theta1 + theta2)
  local j12 =         -L2 * math.sin(theta0 + theta1) - L3 * math.sin(theta0 + theta1 + theta2)
  local j13 =                              -L3 * math.sin(theta0 + theta1 + theta2)

  local j21 =  L1 * math.cos(theta0) + L2 * math.cos(theta0 + theta1) + L3 * math.cos(theta0 + theta1 + theta2)
  local j22 =          L2 * math.cos(theta0 + theta1) + L3 * math.cos(theta0 + theta1 + theta2)
  local j23 =                              L3 * math.cos(theta0 + theta1 + theta2)

  return j11, j12, j13, j21, j22, j23
end

function on_update(dt)
    -- Get local joint angles
    local theta0 = deg2rad(get_sensor_value("joint0"))
    local theta1 = deg2rad(get_sensor_value("joint1"))
    local theta2 = deg2rad(get_sensor_value("joint2"))

    -- Calculate end effector position
    local x_end, y_end = calculate_forward_kinematics(theta0, theta1, theta2)

    -- Calculate position error
    local error_x = xe - x_end
    local error_y = ye - y_end


    -- Calculate integral term
    integral_error_x = integral_error_x + error_x * dt
    integral_error_y = integral_error_y + error_y * dt

    -- Calculate derivative term
    local derivative_error_x = (error_x - previous_error_x) / dt
    local derivative_error_y = (error_y - previous_error_y) / dt


    -- Store current error as previous for the next iteration.
    previous_error_x = error_x
    previous_error_y = error_y


    -- Calculate Jacobian
    local j11, j12, j13, j21, j22, j23 = calculate_jacobian(theta0, theta1, theta2)

    -- Calculate joint velocities with PID control
    local dtheta0 =  Kp * (j11 * error_x + j21 * error_y) +
                    Ki * (j11 * integral_error_x + j21 * integral_error_y) +
                    Kd * (j11 * derivative_error_x + j21 * derivative_error_y)
    local dtheta1 =  Kp * (j12 * error_x + j22 * error_y) +
                    Ki * (j12 * integral_error_x + j22 * integral_error_y) +
                    Kd * (j12 * derivative_error_x + j22 * derivative_error_y)
    local dtheta2 =  Kp * (j13 * error_x + j23 * error_y) +
                    Ki * (j13 * integral_error_x + j23 * integral_error_y) +
                    Kd * (j13 * derivative_error_x + j23 * derivative_error_y)

    print("dtheta0: " .. rad2deg(dtheta0) .. " dtheta1: " .. rad2deg(dtheta1) .. " dtheta2: " .. rad2deg(dtheta2))
    print("x: " .. x_end .. " y: " .. y_end)

    -- Set motor speeds directly (in deg/s)
    set_motor_speed("joint0", rad2deg(dtheta0))
    set_motor_speed("joint1", rad2deg(dtheta1))
    set_motor_speed("joint2", rad2deg(dtheta2))
end