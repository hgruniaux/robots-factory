function on_update(dt)
    print("-----------------------------------------------------")
    print("gyroscope: " .. get_sensor_value("gyroscope"))
    print("calfLDistance: " .. get_sensor_value("calfLDistance"))
    print("calfRDistance: " .. get_sensor_value("calfRDistance"))
end
