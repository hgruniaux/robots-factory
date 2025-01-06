-- Helper function to convert degrees to radians.
function deg2rad(degrees)
    return degrees * math.pi / 180
end

-- Helper function to convert radians to degrees.
function rad2deg(radians)
    return radians * 180 / math.pi
end

-- Returns the sign of a number.
function sign(x)
    if x > 0 then
        return 1
    elseif x < 0 then
        return -1
    else
        return 0
    end
end

-- Helper function to reverse a table.
function reverse_table(tab)
    for i = 1, #tab//2, 1 do
        tab[i], tab[#tab-i+1] = tab[#tab-i+1], tab[i]
    end
    return tab
end
