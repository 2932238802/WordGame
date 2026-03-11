ItemConfigs = {
    ["branch"] = { 
        portable = { size = 0.4 } 
    },
    ["leaves"] = { 
        portable = { size = 0.3 } 
    },
    ["sinew"] = { 
        portable = { size = 1.0 } 
    },
    ["vines"] = { 
        portable = { size = 0.6 } 
    },
    ["hide"] = { 
        portable = { size = 4.0 } 
    },
    ["flint"] = { 
        portable = { size = 2.0 } 
    },
    ["log"] = { 
        portable = { size = 2.0 } 
    },
    ["stone"] = { 
        portable = { size = 2.0 } 
    },
    ["bone"] = { 
        portable = { size = 2.0 } 
    },
    ["bamboo"] = { 
        portable = { size = 1.0 } 
    },
    ["fat"] = { 
        portable = { size = 1.0 } 
    },
    ["feather"] = { 
        portable = { size = 0.1 } 
    },
    ["glue"] = { 
        portable = { size = 0.3 } 
    },
    ["resin"] = { 
        portable = { size = 0.3 } 
    },
    ["coal"] = { 
        portable = { size = 0.5 } 
    },
    ["clay"] = { 
        portable = { size = 1.0 } 
    },

    ["silver"] = { 
        portable = { size = 1.5 } 
    },
    ["gold"] = { 
        portable = { size = 2.0 } 
    },

    ["handle"] = { 
        portable = { size = 0.5 } 
    },
    ["plank"] = { 
        portable = { size = 1.2 } 
    },
    ["rope"] = { 
        portable = { size = 0.8 } 
    },
    ["blade"] = { 
        portable = { size = 0.8 } 
    },

    ["torch"] = { 
        portable = { size = 0.6 } 
    },
    ["bamboo_spear"] = { 
        portable = { size = 1.5 } 
    },
    ["spear"] = { 
        portable = { size = 2.0 } 
    },
    ["wooden_ax"] = { 
        portable = { size = 2.5 } 
    },

    ["bag_b"] = { 
        portable = { size = 3.0 } 
    },
    ["bag_a"] = { 
        portable = { size = 5.0 } 
    },
    ["crate"] = { 
        portable = { size = 8.0 } 
    },
    ["bed"] = { 
        portable = { size = 15.0 } 
    },
}

function InitItemComponents(r, id, name)
    local cfg = ItemConfigs[name]
    if not cfg then
        P("Lua Warning: No config for " .. name)
        return
    end

    if cfg.portable then
        addPortable(r, id, cfg.portable.size)
    end
end
