require("utils") -- Assuming utils.sign() is available

-- Global variables (set externally)
target_foot1_speed = 0 -- Target horizontal speed for foot 1 (m/s)
target_foot2_speed = 0 -- Target horizontal speed for foot 2 (m/s)
target_altitude = 1.0 -- Target altitude of the body's center of mass (m)

-- Robot parameters (in meters)
leg_segment_length = 0.5 -- m
body_width = 0.2 -- m
body_height = 0.1 -- m
max_torque = 25 -- Nm

-- PD controller gains (tune these values)
kp_altitude = 10.0
kd_altitude = 5.0
kp_foot_speed = 8.0
kd_foot_speed = 2.0

-- Function to get the position of a point given an angle and length (in meters)
local function get_point_from_angle(angle_deg, length)
    local rad_angle = math.rad(angle_deg)
    return length * math.cos(rad_angle), length * math.sin(rad_angle)
end

local function control_robot(dt)
    print("############### Iteration")

    -- Get sensor values (joint angles in degrees)
    local leg1_torse_angle = get_sensor_value("leg1-torse")
    local leg2_torse_angle = get_sensor_value("leg2-torse")
    local leg1_knee_angle = get_sensor_value("leg1.1-2")
    local leg2_knee_angle = get_sensor_value("leg2.1-2")

    -- Calculate foot positions (CORRECTED for local angles and using temp variables)
    local leg1_hip_x, leg1_hip_y = get_point_from_angle(leg1_torse_angle, leg_segment_length)
    local foot1_x_offset, foot1_y_offset = get_point_from_angle(leg1_torse_angle + leg1_knee_angle, leg_segment_length)
    local foot1_x = leg1_hip_x + foot1_x_offset
    local foot1_y = leg1_hip_y + foot1_y_offset

    local leg2_hip_x, leg2_hip_y = get_point_from_angle(leg2_torse_angle, leg_segment_length)
    local foot2_x_offset, foot2_y_offset = get_point_from_angle(leg2_torse_angle + leg2_knee_angle, leg_segment_length)
    local foot2_x = leg2_hip_x + foot2_x_offset
    local foot2_y = leg2_hip_y + foot2_y_offset

    -- Calculate body center (using hip positions)
    local body_center_x = (leg1_hip_x + leg2_hip_x) / 2
    local body_center_y = (leg1_hip_y + leg2_hip_y) / 2 + body_height/2

    -- Calculate foot speeds (more accurate using previous positions and dt)
    local foot1_speed = (foot1_x - (previous_foot1_x or foot1_x)) / dt
    local foot2_speed = (foot2_x - (previous_foot2_x or foot2_x)) / dt

    -- Store current values for next iteration
    previous_foot1_x = foot1_x
    previous_foot2_x = foot2_x

    -- Altitude control
    local altitude_error = target_altitude - body_center_y
    local altitude_deriv = (altitude_error - (previous_altitude_error or altitude_error)) / dt
    local altitude_control = kp_altitude * altitude_error + kd_altitude * altitude_deriv

    previous_altitude_error = altitude_error

    -- Foot speed control
    local foot1_speed_error = target_foot1_speed - foot1_speed
    local foot2_speed_error = target_foot2_speed - foot2_speed
    local foot1_speed_deriv = (foot1_speed_error - (previous_foot1_speed_error or foot1_speed_error)) / dt
    local foot2_speed_deriv = (foot2_speed_error - (previous_foot2_speed_error or foot2_speed_error)) / dt
    local foot1_speed_control = kp_foot_speed * foot1_speed_error + kd_foot_speed * foot1_speed_deriv
    local foot2_speed_control = kp_foot_speed * foot2_speed_error + kd_foot_speed * foot2_speed_deriv

    previous_foot1_speed_error = foot1_speed_error
    previous_foot2_speed_error = foot2_speed_error

    -- Combine controls and calculate desired motor speeds (in deg/s)
    local leg1_torse_speed_desired = altitude_control + foot1_speed_control
    local leg2_torse_speed_desired = altitude_control + foot2_speed_control
    local leg1_knee_speed_desired = foot1_speed_control
    local leg2_knee_speed_desired = foot2_speed_control

    -- Torque Limiting (Simplified: Proportional to speed)
    local function limit_torque(speed_desired, joint_name)
        local max_speed = get_motor_max_speed(joint_name)
        local limited_speed = speed_desired
        if math.abs(speed_desired) > max_speed then
            limited_speed = sign(speed_desired) * max_speed
            print("Torque limited for joint: " .. joint_name .. " Desired speed:" .. speed_desired .. " Limited speed:" .. limited_speed)
        end
        return limited_speed
    end

    local leg1_torse_speed = limit_torque(leg1_torse_speed_desired,"leg1-torse")
    local leg2_torse_speed = limit_torque(leg2_torse_speed_desired,"leg2-torse")
    local leg1_knee_speed = limit_torque(leg1_knee_speed_desired,"leg1.1-2")
    local leg2_knee_speed = limit_torque(leg2_knee_speed_desired,"leg2.1-2")

    -- print("Leg1 Torse Speed:", leg1_torse_speed, " Leg2 Torse Speed:", leg2_torse_speed, " Leg1 Knee Speed:", leg1_knee_speed, " Leg2 Knee Speed:", leg2_knee_speed)
    print("Foot1 x:" .. foot1_x .. " Foot1 y:" .. foot1_y .. "Foot2 x:" .. foot2_x .. " Foot2 y:" .. foot2_y)
    print("Body center y:" .. body_center_y)
    print("Altitude error:" .. altitude_error)

    set_motor_speed("leg1-torse", leg1_torse_speed)
    set_motor_speed("leg2-torse", leg2_torse_speed)
    set_motor_speed("leg1.1-2", leg1_knee_speed)
    set_motor_speed("leg2.1-2", leg2_knee_speed)
end

function on_update(dt)
    control_robot(dt)
end