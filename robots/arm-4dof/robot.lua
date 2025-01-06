require("arm")

function on_update()
    local xe = 1.0  -- t
    local ye = 1.0
    control_arm(xe, ye, "joint0", "joint1", "joint2", "joint3");
end