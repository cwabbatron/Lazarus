component = {
    name = "SecondaryComponent",
    on_update = function(self)
        print("Secondary component running")
    end,
    on_attach = function(self)
        print("attached secondary")
    end,

    on_deattach = function(self)
        print("GONE secondary")
    end,
}