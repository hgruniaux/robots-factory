require("utils")

-- Calculate the forward kinematics of a robot arm
function calculate_forward_kinematics(joint_angles, link_lengths)
  local x = 0
  local y = 0
  local cumulative_angle = 0

  for i = 1, #joint_angles do
    cumulative_angle = cumulative_angle + joint_angles[i]
    x = x + link_lengths[i] * math.cos(cumulative_angle)
    y = y + link_lengths[i] * math.sin(cumulative_angle)
  end
  return x, y
end

-- Calculate the Jacobian of a robot arm
function calculate_jacobian(joint_angles, link_lengths)
    local num_joints = #joint_angles
    local j = {}
    local cumulative_angle = 0

    for i=1, num_joints do
       j[i] = {}
    end

    for i = 1, num_joints do
        cumulative_angle = cumulative_angle + joint_angles[i]
        for k = 1, num_joints do
             local angle_sum = 0
             for q = k, num_joints do
                angle_sum = angle_sum + joint_angles[q]
             end
           if i <= k then
            j[1][k] = j[1][k] or 0
             j[1][k] = j[1][k] - link_lengths[i]*math.sin(angle_sum)
            j[2][k] = j[2][k] or 0
              j[2][k] = j[2][k] + link_lengths[i]*math.cos(angle_sum)
           end
        end

    end

    return j
end

-- Store previous and integral errors for each arm
PREVIOUS_ERRORS_X = {}
PREVIOUS_ERRORS_Y = {}
INTEGRAL_ERRORS_X = {}
INTEGRAL_ERRORS_Y = {}

-- Reset the error storage for all arms
function reset_control_arm_errors()
    PREVIOUS_ERRORS_X = {}
    PREVIOUS_ERRORS_Y = {}
    INTEGRAL_ERRORS_X = {}
    INTEGRAL_ERRORS_Y = {}
end

-- Control the robot arm to reach a target position
function control_arm(target_x, target_y, ...)
    local joints = {...} -- List of joints
    local num_joints = #joints

     -- Time step (in seconds)
    local dt = delta_time

     -- PID gains
    local Kp = 1.0 -- Proportional gain
    local Ki = 0.2 -- Integral gain
    local Kd = 0.1 -- Derivative gain

    -- Error storage
    local previous_error_x = PREVIOUS_ERRORS_X[joints] or 0
    local previous_error_y = PREVIOUS_ERRORS_Y[joints] or 0
    local integral_error_x = INTEGRAL_ERRORS_X[joints] or 0
    local integral_error_y = INTEGRAL_ERRORS_Y[joints] or 0

    local link_lengths = {}
    for i=1, num_joints do
        -- FIXME: retrieve automatically the link length
        link_lengths[i] = 0.5 -- Each link is 0.5 meters.
    end

    -- Get local joint angles in radians
    local radian_angles = {}
    for i=1,num_joints do
       radian_angles[i] = deg2rad(get_sensor_value(joints[i]))
    end

    -- Calculate end effector position
    local x_end, y_end = calculate_forward_kinematics(radian_angles, link_lengths)

    -- Calculate position error
    local error_x = target_x - x_end
    local error_y = target_y - y_end

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
    local j =  calculate_jacobian(radian_angles, link_lengths)

    -- Calculate joint velocities with PID control
    local dtheta = {}
    for k=1,num_joints do
        dtheta[k] = Kp * (j[1][k] * error_x + j[2][k] * error_y) +
                    Ki * (j[1][k] * integral_error_x + j[2][k] * integral_error_y) +
                    Kd * (j[1][k] * derivative_error_x + j[2][k] * derivative_error_y)
    end

    -- Set motor speeds
    for i=1,num_joints do
      set_motor_speed(joints[i], rad2deg(dtheta[i]))
    end

    -- Store errors for the next iteration
    PREVIOUS_ERRORS_X[joints] = previous_error_x
    PREVIOUS_ERRORS_Y[joints] = previous_error_y
    INTEGRAL_ERRORS_X[joints] = integral_error_x
    INTEGRAL_ERRORS_Y[joints] = integral_error_y

    return x_end, y_end
end
